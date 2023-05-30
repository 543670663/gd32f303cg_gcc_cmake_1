#ifndef __DRV_NCS8803_H_
#define __DRV_NCS8803_H_

#ifdef __cplusplus
extern "C" {
#endif
void Ncs8803Check(bool *hdmi_ok, bool *hdmi_begin);
void LcdInit(bool *lcd_write_allow_state);
bool Hdmi_Det();
void Ncs8803_Init(void);
void Ncs8803_1920_1080_Write(void);
void Ncs8803_1920_1080_Write_For_Common(void);
#ifdef __cplusplus
}
#endif

#endif
