#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "sys.h"
#include "OLED.h"
#include "DS18B20.h"
#include "ACS712.h"
#include "ADC.h"
#include "Motor.h"
#include "Key.h"

short temperature;


uint16_t ACS712_ADValue;
float ACS712_Voltage;
float Electric = 0.0;

uint16_t ADValue;
float Voltage;

uint8_t KeyNum;
int8_t Speed;

int main(void)
{
	OLED_Init();
	Motor_Init();
	Key_Init();
 	while(DS18B20_Init())	//DS18B20初始化	
	{
    OLED_ShowString(2,2,"error");
	}								   
	ACS712_Configuration();
	AD_Init();
	
	OLED_ShowString(1, 1, "ADValue:");
	OLED_ShowString(3, 1, "Volatge:0.00V");
	OLED_ShowString(4, 1, "Electri:0.00A");
	while(1)
	{
		
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			Speed += 20;
			if (Speed > 100)
			{
				Speed = -100;
			}
		}
		Motor_SetSpeed(Speed);
    //读取温度		
		temperature=DS18B20_Get_Temp();	
		if(temperature<0)
		{		
			OLED_ShowString(2,4,"-");     //显示负号
			temperature=-temperature;					//转为正数
		}
		else 
		OLED_ShowString(2,4," ");

		OLED_ShowString(2,3,":");
		OLED_ShowNum(2,5,temperature/10,2);     //显示整数部分
		OLED_ShowString(2,7,".");
   	OLED_ShowNum(2,8,temperature%10,1);	//显示小数部分 
		  
		//电流显示
		
		ACS712_ADValue = ACS712_GetValue(ADC_Channel_1);
		ACS712_Voltage = (float)ACS712_ADValue / 4095 * 3.3;
		
		//OLED_ShowNum(1, 9, ACS712_ADValue, 4);
		//OLED_ShowNum(3, 9, ACS712_Voltage, 1);
		//OLED_ShowNum(3, 11, (uint16_t)(ACS712_Voltage * 100) % 100, 2);
		Electric = -1*(ACS712_Voltage-2.5)/0.185;	
		OLED_ShowNum(4, 9, Electric, 1);
		OLED_ShowNum(4, 11, (uint16_t)(Electric * 100) % 100, 2);
		
		//电压显示
		
		ADValue = AD_GetValue(ADC_Channel_7);
		Voltage = (float)ADValue / 4095 * 3.3 * 5;
		OLED_ShowNum(1, 9, ADValue, 4);
		OLED_ShowNum(3, 9, Voltage, 1);
		OLED_ShowNum(3, 11, (uint16_t)(Voltage * 100) % 100, 2);
		
		
    Delay_ms(150);
	}				   	
}
