#include "stm32f10x.h"
#include "ACS712.h"



/**
  * @brief  ADC Configuration
  * @param  None
  * @retval None
  */
void ACS712_Configuration(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

    // ����GPIO��ģʽ��ģ������
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADCʱ��Ϊ12MHz��72MHz / 6��

    // ����ADCģ�����
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;            // ����ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                 
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;           
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // �ⲿ������ֹ
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;        // ���ݶ��뷽ʽ���Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 2;
    ADC_Init(ADC1, &ADC_InitStructure);

    // ʹ��ADCģ��
    ADC_Cmd(ADC1, ENABLE);

    // У׼ADCģ��
    ADC_ResetCalibration(ADC1);                                  
    while (ADC_GetResetCalibrationStatus(ADC1));                  
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
}

uint16_t ACS712_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}


/**
  * @brief  Get Current Value
  * @param  None
  * @retval float������ֵ��A��
  */
/*
float Get_Current_Value(void)
{
    uint16_t adc_value = 0;
		uint16_t adv_f = 0;
    float current_value = 0;

    // ��ȡPA1���Ŷ�Ӧ��ADCת��ֵ
    adc_value = AD_GetValue();

    // ��ADCת��ֵת��Ϊ����ֵ
	  adv_f=adc_value*3.3/4095;//��ѹֵ
    current_value = -1*(adv_f-2.5)/0.185;
    return current_value;
}
*/