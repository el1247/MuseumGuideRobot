/* Copyright (C) 2021 Duncan Lowther
 *  
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <endian.h>
#include <math.h>
#include <pigpio.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gpio_assign.h"
#include "MadgwickAHRS.h"
#include "sensors.h"

static unsigned imu, imu_bank_cur, mag;
static int has_magnet, mag_transparent;
static float hdg = 0.0f;
static float vel = 0.0f;
static float vx = 0.0f, vy = 0.0f;
static float posx = 0.0f, posy = 0.0f;

static void (*on_error)(int);

static timer_t ticker;
struct sigevent tickevt;
struct itimerspec tickspec = { .it_interval = { .tv_sec = 0, .tv_nsec = SAMPLE_TIME_NS },
	                       .it_value = { .tv_sec = 0, .tv_nsec = 500000000 }};

static int mag_read_indir(unsigned);
static int mag_write_indir(unsigned,unsigned);

float get_current_heading(void) {
    return hdg;
}

float get_current_velocity(void) {
    return vel;
}

void get_current_position(float * x, float * y) {
    *x = posx; *y = posy;
}

void imu_reset(float cx, float cy, float chdg) {
    posx = cx, posy = cy, hdg = chdg;
    q0 = cosf(chdg/2.0f);
    q3 = -sinf(chdg/2.0f);
    q1 = q2 = vx = vy = 0.0f;
}

/* High byte is register bank (0-3), low byte is register address */
enum imu_regs {
    IMUREG_WHO_AM_I             = 0x000,
    IMUREG_USER_CTRL            = 0x003,
    IMUREG_PWR_MGMT_1           = 0x006,
    IMUREG_PWR_MGMT_2           = 0x007,
    IMUREG_INT_PIN_CFG          = 0x00F,
    IMUREG_ACCEL_XOUT_H         = 0x02D,
    IMUREG_EXT_SLV_SENS_DATA_00 = 0x03B,
    IMUREG_GYRO_SMPLRT_DIV      = 0x200,
    IMUREG_GYRO_CONFIG_1        = 0x201,
    IMUREG_ACCEL_SMPLRT_DIV_1   = 0x210,
    IMUREG_ACCEL_SMPLRT_DIV_2   = 0x211,
    IMUREG_ACCEL_CONFIG         = 0x214,
    IMUREG_I2C_MST_CTRL         = 0x301,
    IMUREG_I2C_MST_DELAY_CTRL   = 0x302,
    IMUREG_I2C_SLV0_ADDR        = 0x303,
    IMUREG_I2C_SLV0_REG         = 0x304,
    IMUREG_I2C_SLV0_CTRL        = 0x305,
    IMUREG_I2C_SLV0_DO          = 0x306,
};

enum mag_regs {
    MAGREG_WIA   = 0x01,
    MAGREG_ST1   = 0x10,
    MAGREG_HXL   = 0x11,
    MAGREG_CNTL2 = 0x31,
    MAGREG_CNTL3 = 0x32,
};

#define MAG_ID 0x09


#define IMUREG_BANK_SEL 0x7F
#define IMU_ID 0xEA
#define REG_BANK(reg) (((IMUREG_##reg)&0x300)>>4)
#define REG_ADDR(reg) ((IMUREG_##reg)&0xFF)

#define SET_BANK(reg) (imu_bank_cur==REG_BANK(reg)?0:i2cWriteByteData(imu,IMUREG_BANK_SEL,imu_bank_cur=REG_BANK(reg)))
#define IMU_READ(reg)               (SET_BANK(reg) ?: i2cReadByteData(imu,REG_ADDR(reg)))
#define IMU_READ_BLOCK(reg,buf,len) (SET_BANK(reg) ?: i2cReadI2CBlockData(imu,REG_ADDR(reg),buf,len))
#define IMU_WRITE(reg,data)         (SET_BANK(reg) ?: i2cWriteByteData(imu,REG_ADDR(reg),data))


typedef uint16_t uint16_be_t, uint16_le_t; /* For clarity we write the non-host endianness explicitly  */

#define ACCEL_FS 3 
#define GYRO_FS_SEL 0 

#define ACCEL_LSB (9.81f*(1<<ACCEL_FS)/16384.0f) /* For ACCEL_FS = n (0<=n<=3), ACCEL_LSB = 2^(n-14) */
#define GYRO_LSB (1/131.0f) /* For GYRO_FS_SEL = 0,1,2,3 GYRO_LSB = 1/131, 1/65.5, 1/32.8, 1/16.4 */

#define A2F(x) (ACCEL_LSB*(float)(int16_t)be16toh(x))
#define G2F(x) (GYRO_LSB*(float)(int16_t)be16toh(x))
#define M2F(x) ((float)(int16_t)le16toh(x))

#define USE_MAGNET 0

#define ASSUME_TAKEN(x) __builtin_expect(!!(x),1)
#define ASSUME_NOT_TAKEN(x) __builtin_expect(!!(x),0)

#define MAG_BLOCK_READ_H6(buf) (mag_transparent ? i2cReadI2CBlockData(mag,MAGREG_HXL,buf,6) \
	                                        : IMU_READ_BLOCK(EXT_SLV_SENS_DATA_00,buf,6))
#define MAG_READ(reg)          (mag_transparent ? i2cReadByteData(mag,MAGREG_##reg) \
		                                : mag_read_indir(MAGREG_##reg))
#define MAG_WRITE(reg,data)    (mag_transparent ? i2cWriteByteData(mag,MAGREG_##reg,data) \
		                                : mag_write_indir(MAGREG_##reg,data))


void imu_tick(__sigval_t _) {
    int err;
    uint16_be_t buf[6];
    err = IMU_READ_BLOCK(ACCEL_XOUT_H, (char*)buf, 12);
    if(ASSUME_NOT_TAKEN(err < 0)) goto error;
    const float ax = A2F(buf[0]), ay = A2F(buf[1]), az = A2F(buf[2]);
    const float gx = G2F(buf[3]), gy = G2F(buf[4]), gz = G2F(buf[5]);
    
    if(USE_MAGNET && has_magnet) {
        uint16_le_t mbuf[3];
        if(ASSUME_TAKEN(MAG_BLOCK_READ_H6((char*)mbuf) >= 0)) {
            MadgwickAHRSupdate(gx, gy, gz, ax, ay, az, M2F(mbuf[0]), M2F(mbuf[1]), M2F(mbuf[2]));
	} else {
            MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az);
	    has_magnet = 0;
	}
    } else {
        MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az);
    }
    const float qy = (q1*q2-q0*q3), qx = (q2*q2+q3*q3) - 0.5f;
    hdg = atan2f(qy, qx);
    const float tqinvnorm = invSqrt(qy*qy+qx*qx) * (SAMPLE_TIME_NS/1000000000.0f); 
    const float tsinhdg = qy*tqinvnorm, tcoshdg = qx*tqinvnorm;
    vx += ax*tcoshdg - ay*tsinhdg;
    vy += ay*tcoshdg + ay*tsinhdg;
    vel = sqrt(vx*vx+vy*vy);
    posx += vx*tcoshdg - vy*tsinhdg;
    posy += vy*tcoshdg + vy*tsinhdg;
    return;
error:
    timer_delete(ticker);
    if(on_error) (*on_error)(err);
}

#define GYRO_RATE_DIV 4 /* TODO */
#define ACCEL_RATE_DIV 8 /* TODO */

void imu_init(float startx, float starty, float starthdg, void (*on_err)(int)) {
    int tmp;
    on_error = on_err;
    posx = startx;
    posy = starty;
    q0 = cosf(starthdg/2.0f);
    q1 = q2 = 0.0f;
    q3 = -sinf(starthdg/2.0f);
    tmp = i2cOpen(1, I2CADDR_IMU, 0); 
    if(tmp < 0) goto error;
    imu = (unsigned) tmp;
    if((tmp = IMU_READ(WHO_AM_I)) != IMU_ID) goto error;
    if((tmp = IMU_WRITE(PWR_MGMT_1, 0x80))) goto error; /* DEVICE_RESET */
    usleep(10000);
    if((tmp = IMU_WRITE(PWR_MGMT_1, 0x01) /* CLKSEL=AUTO */
           ?: IMU_WRITE(PWR_MGMT_2, 0x00) /* ¬DISABLE_ACCEL, ¬DISABLE_GYRO */
           ?: IMU_WRITE(GYRO_SMPLRT_DIV, GYRO_RATE_DIV)
           ?: IMU_WRITE(GYRO_CONFIG_1, 0x19|(GYRO_FS_SEL<<1))
           ?: IMU_WRITE(ACCEL_SMPLRT_DIV_1, (ACCEL_RATE_DIV>>8)&0xFF)
           ?: IMU_WRITE(ACCEL_SMPLRT_DIV_2, ACCEL_RATE_DIV&0xFF)
           ?: IMU_WRITE(ACCEL_CONFIG, 0x19|(ACCEL_FS<<1)))) goto error;
#ifndef NO_MAGNET
    mag_transparent = has_magnet = 1;
    tmp = i2cOpen(1, I2CADDR_MAG, 0);
    if(tmp > 0) {
        mag = (unsigned) tmp;
	if((MAG_READ(WIA) != MAG_ID) || MAG_WRITE(CNTL3, 0x01)) mag_transparent = 0;
	else {
            has_magnet = !MAG_WRITE(CNTL2, 0x08); /* Continuous 4 (100 Hz) */
	}
    }
    if(!mag_transparent) {
        has_magnet = !IMU_WRITE(I2C_MST_CTRL, 0x4D)
                  && !IMU_WRITE(I2C_MST_DELAY_CTRL, 0x01)
	          && (MAG_READ(WIA) == MAG_ID)
                  && !MAG_WRITE(CNTL3, 0x01);
        if(has_magnet) {
            while(MAG_READ(CNTL3) == 0x01) usleep(1000);
	    /* Setup magnetometer for 100Hz reading and prime registers for block reads */
            tmp = MAG_WRITE(CNTL2, 0x08) /* Continuous 4 (100 Hz) */
               ?: IMU_WRITE(I2C_SLV0_ADDR, I2CADDR_MAG | 0x80) /* High bit = RD/¬WR */
               ?: IMU_WRITE(I2C_SLV0_REG, MAGREG_HXL)
               ?: IMU_WRITE(I2C_SLV0_CTRL, 0x86) /* EN_READ, len=6 */
               ?: IMU_READ(USER_CTRL);
    	    has_magnet = (tmp > 0) && !IMU_WRITE(USER_CTRL, tmp | 0x20);
        }
    }
#endif
    beta = 0.0755749735f; /* Calibrated as per Madgwick for 5 degrees per second error */
    
    tickevt.sigev_notify = SIGEV_THREAD;
    tickevt.sigev_notify_function = imu_tick;
    tickevt.sigev_notify_attributes = NULL; /* XXX Possibly increase priority? */

    if(timer_create(CLOCK_MONOTONIC, &tickevt, &ticker)
        || timer_settime(ticker, 0, &tickspec, NULL)) {
        perror("Error starting imu update timer: ");
        i2cClose(imu);
        if(on_error) (*on_error)(0);
	return;
    }

    atexit(imu_fini);
    return;
  error:
    if(on_error) (*on_error)(tmp);
}

static int mag_read_indir(unsigned reg) {
    int ret;
    int user = IMU_WRITE(I2C_SLV0_ADDR, I2CADDR_MAG | 0x80)/* High bit = RD/¬WR */
	    ?: IMU_WRITE(I2C_SLV0_REG, reg)
            ?: IMU_WRITE(I2C_SLV0_CTRL, 0x81) /* EN_READ */
            ?: IMU_READ(USER_CTRL);
    if(ASSUME_NOT_TAKEN(user < 0)) return user;
    if(ASSUME_NOT_TAKEN(ret = IMU_WRITE(USER_CTRL, user | 0x20))) return ret;
    usleep(5000);
    return IMU_WRITE(USER_CTRL, user)
	?: IMU_READ(EXT_SLV_SENS_DATA_00);
}

static int mag_write_indir(unsigned reg, unsigned data) {
    int ret;
    int user = IMU_WRITE(I2C_SLV0_ADDR, I2CADDR_MAG) /* High bit = RD/¬WR */
	    ?: IMU_WRITE(I2C_SLV0_REG, reg)
            ?: IMU_WRITE(I2C_SLV0_DO, data) 
            ?: IMU_READ(USER_CTRL);
    if(ASSUME_NOT_TAKEN(user < 0)) return user;
    if(ASSUME_NOT_TAKEN(ret = IMU_WRITE(USER_CTRL, user | 0x20))) return ret;
    usleep(5000);
    return IMU_WRITE(USER_CTRL, user);
}


void imu_fini(void) {
    timer_delete(ticker);
    i2cClose(imu);
}

