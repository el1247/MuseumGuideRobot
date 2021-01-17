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
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "gpio_assign.h"
#include "MadgwickAHRS.h"
#include "sensors.h"

static unsigned imu, imu_bank_cur;

/* High byte is register bank (0-3), low byte is register address */
enum imu_regs {
    IMUREG_WHO_AM_I           = 0x000,
    IMUREG_PWR_MGMT_1         = 0x006,
    IMUREG_PWR_MGMT_2         = 0x007,
    IMUREG_INT_PIN_CFG        = 0x00F,
    IMUREG_ACCEL_XOUT_H       = 0x02D,
    IMUREG_GYRO_SMPLRT_DIV    = 0x200,
    IMUREG_GYRO_CONFIG_1      = 0x201,
    IMUREG_ACCEL_SMPLRT_DIV_1 = 0x210,
    IMUREG_ACCEL_SMPLRT_DIV_2 = 0x211,
    IMUREG_ACCEL_CONFIG       = 0x214,
    IMUREG_MST_CTRL           = 0x301,
    IMUREG_MST_DELAY_CTRL     = 0x302,
};

#define IMUREG_BANK_SEL 0x7F
#define IMU_ID 0xEA
#define REG_BANK(reg) (((IMUREG_##reg)&0x300)>>4)
#define REG_ADDR(reg) ((IMUREG_##reg)&0xFF)

#define IMU_READ(reg)               ((imu_bank_cur==REG_BANK(reg) ? 0 \
			                 : i2cWriteByteData(imu,IMUREG_BANK_SEL,imu_bank_cur=REG_BANK(reg))) \
			                ?: i2cReadByteData(imu,REG_ADDR(reg)))
#define IMU_READ_BLOCK(reg,buf,len) ((imu_bank_cur==REG_BANK(reg) ? 0 \
			                 : i2cWriteByteData(imu,IMUREG_BANK_SEL,imu_bank_cur=REG_BANK(reg))) \
			                ?: i2cReadI2CBlockData(imu,REG_ADDR(reg),buf,len))
#define IMU_WRITE(reg,data)         ((imu_bank_cur==REG_BANK(reg) ? 0 \
 			                 : i2cWriteByteData(imu,IMUREG_BANK_SEL,imu_bank_cur=REG_BANK(reg))) \
			                ?: i2cWriteByteData(imu,REG_ADDR(reg),data))

static unsigned hdg = 0;
static float vel = 0.0f;

unsigned get_current_heading(void) {
    return hdg;
}

double get_current_velocity(void) {
    return vel;
}


typedef uint16_t uint16_be_t, uint16_le_t; /* For clarity we write the non-host endianness explicitly  */

#define R2F(x) ((float)(int16_t)be16toh(x))
#define M2F(x) ((float)(int16_t)le16toh(x))

#define USE_MAGNET 0
#define MAGNET_READ(buf,len) /* TODO */

void imu_tick(void) {
    int err;
    uint16_be_t buf[6];
    err = IMU_READ_BLOCK(ACCEL_XOUT_H, (char*)buf, 12);
    
    if(USE_MAGNET) {
        uint16_le_t mbuf[3];
        MAGNET_READ((char*)mbuf, 6); /* XxX TODO figure out */
        MadgwickAHRSupdate(R2F(buf[3]), R2F(buf[4]), R2F(buf[5]), R2F(buf[0]), R2F(buf[1]), R2F(buf[2]),
                           M2F(mbuf[0]), M2F(mbuf[1]), M2F(mbuf[2]));
    } else {
        MadgwickAHRSupdateIMU(R2F(buf[3]), R2F(buf[4]), R2F(buf[5]), R2F(buf[0]), R2F(buf[1]), R2F(buf[2]));
    }
    hdg = (unsigned)atan2f(2*(q1*q2-q0*q3), 2*(q2*q2+q3*q3) - 1);
    /* TODO update velocity */
}

#define GYRO_RATE_DIV 0 /* TODO */
#define ACCEL_RATE_DIV 0 /* TODO */

void imu_init(void) {
    int tmp;
    tmp = i2cOpen(1, I2CADDR_IMU, 0); 
    if(tmp < 0) goto error;
    imu = (unsigned) tmp;
    if((tmp = IMU_READ(WHO_AM_I)) != IMU_ID) goto error;
    if((tmp = IMU_WRITE(PWR_MGMT_1, 0x80))) goto error; /* DEVICE_RESET */
    usleep(10000);
    if((tmp = IMU_WRITE(PWR_MGMT_1, 0x01))) goto error; /* CLKSEL=AUTO */
    if((tmp = IMU_WRITE(PWR_MGMT_2, 0x00))) goto error; /* ¬DISABLE_ACCEL, ¬DISABLE_GYRO */
    if((tmp = IMU_WRITE(GYRO_SMPLRT_DIV, GYRO_RATE_DIV))) goto error;
    tmp = IMU_READ(GYRO_CONFIG_1);
    if(tmp < 0 || (tmp = IMU_WRITE(GYRO_CONFIG_1, (tmp&0x88)|0x51))) goto error;
    if((tmp = IMU_WRITE(ACCEL_SMPLRT_DIV_1, (ACCEL_RATE_DIV>>8)&0xFF))) goto error;
    if((tmp = IMU_WRITE(ACCEL_SMPLRT_DIV_2, ACCEL_RATE_DIV&0xFF))) goto error;
    tmp = IMU_READ(ACCEL_CONFIG);
    if(tmp < 0 || (tmp = IMU_WRITE(ACCEL_CONFIG, (tmp&0x88)|0x57))) goto error;
    if((tmp = IMU_WRITE(INT_PIN_CFG, 0x30))) goto error; /* XXX ? INT1_LATCH__EN, INT_ANYRD_2CLEAR */
    
    if((tmp = IMU_WRITE(MST_CTRL, 0x4D))) goto error;
    if((tmp = IMU_WRITE(MST_DELAY_CTRL, 0x01))) goto error;
    /* XXX slave magnetometer??? */

    beta = 0 /* XXX TODO */;

    atexit(imu_fini);
    return;
  error:;
    /* TODO */
}


void imu_fini(void) {
    i2cClose(imu);
}

