#include "sys.h"
#include "hw_config.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ϵͳ�жϷ������û�
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2019/9/17
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************

//ʱ��ϵͳ���ú���
//PLL:ѡ��ı�Ƶ����RCC_PLL_MUL2~RCC_PLL_MUL16
void Stm32_Clock_Init(u32 PLL)
{
    RCC_OscInitTypeDef RCC_OscInitStructure = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStructure = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit  = {0};
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    	//ʱ��ԴΪHSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      	//��HSE
    RCC_OscInitStructure.HSEPredivValue=RCC_HSE_PREDIV_DIV1;		//HSEԤ��Ƶ
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;					//��PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;			//PLLʱ��Դѡ��HSE
    RCC_OscInitStructure.PLL.PLLMUL=PLL; 							//��PLL��Ƶ����
    if (HAL_RCC_OscConfig(&RCC_OscInitStructure) != HAL_OK)
    {
        Error_Handler();
    }
	/** Initializes the CPU, AHB and APB busses clocks 
	*/
    //ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;		//����ϵͳʱ��ʱ��ԴΪPLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;				//AHB��Ƶϵ��Ϊ1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV2; 				//APB1��Ƶϵ��Ϊ2
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV1; 				//APB2��Ƶϵ��Ϊ1
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStructure, FLASH_LATENCY_2) != HAL_OK)//ͬʱ����FLASH��ʱ����Ϊ2WS��Ҳ����3��CPU���ڡ�
	{
		Error_Handler();
	}
	//USB��ʼ��
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
}

#ifdef  USE_FULL_ASSERT
//��������ʾ�����ʱ��˺����������������ļ���������
//file��ָ��Դ�ļ�
//line��ָ�����ļ��е�����
void assert_failed(uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}
#endif
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI
void WFI_SET(void)
{
    __ASM volatile("wfi");
}
//�ر������ж�
void INTX_DISABLE(void)
{
    __ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr)
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
