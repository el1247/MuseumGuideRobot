#ifndef MOVEMENT_H
#define MOVEMENT_H
#ifdef __cplusplus
extern "C" {
#endif

void m_init(void);
int m_drive(double,unsigned);
void m_stop(void);
void m_fini(void);

#ifdef __Cplusplus
}
#endif
#endif

