#include "hw_config.h"
#include "usbd_def.h"
#include "usbd_ctlreq.h"
#include "usbd_core.h"
#include "sys.h"
#include "usart.h"  
#include "string.h"	
#include "stdarg.h"		 
#include "stdio.h"	
#include "stm32f1xx_hal_gpio.h"
#include "delay.h"
extern USBD_HandleTypeDef hUsbDeviceFS;
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//USB-hw_config ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/28
//�޸�����:2019/11/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
	*Error_Handler for usb
    *��������ʱ�Ļص��������û�����������������Ϣ
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
}

/*@brief �ϵ������USB�����ߣ�������ʶ��Ϊ�Ͽ���
 * ��ʱһ��ʱ�����ͷ�ȥ��ʼ���������˸�λ����Ҫ��������ߡ�
 */
void USB_Reset()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
 
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
 
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	delay_us(700);//delay 10 ms
}
/*
	��ȡ�豸״̬�����ص�ǰ���豸״̬
	USBD_STATE_DEFAULT                              0x01U
	USBD_STATE_ADDRESSED                            0x02U
	USBD_STATE_CONFIGURED                           0x03U
	USBD_STATE_SUSPENDED                            0x04U
*/
uint8_t USB_GetStatus()
{
	return hUsbDeviceFS.dev_state;//�����豸״̬
}
