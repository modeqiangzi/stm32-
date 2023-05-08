#ifndef __ACS712_H__
#define __ACS712_H__


void ACS712_Configuration(void);
//float Get_Current_Value(void);
uint16_t ACS712_GetValue(uint8_t ADC_Channel);

#endif /* __ACS712_H__ */

