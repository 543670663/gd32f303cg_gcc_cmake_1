#ifndef __DRV_NCS8803_H_
#define __DRV_NCS8803_H_

#ifdef __cplusplus
extern "C" {
#endif


void Ncs8803_1920_1080_Write(void);
uint8_t *Ncs8803_1920_1080_Read(void);
void Hdmi_Det();
void FRC();
#ifdef __cplusplus
}
#endif

#endif