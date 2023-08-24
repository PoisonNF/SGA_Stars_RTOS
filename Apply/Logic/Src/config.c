#include "config.h"

/* ���������õ�����IO
	Thruster
		  PWM1	PE9
		  PWM2	PE11
		  PWM3	PE13
		  PWM4	PE14
		  PWM5	PB6
		  PWM6	PB7
		  PWM7	PB
		  PWM8	PB9
	JY901S
		PA2	PA3 Uart2

	Uart1			Uart2			Uart3			Uart4			Uart5
		TX PA9 			TX PA2			TX PB10			TX PC10			TX PC12
		RX PA10			RX PA3			RX PB11			RX PC11			RX PD2
*/
/* ��Ȼ�PID */
tagPID_T DepthPID = 
{
	.fKp = 1,
	.fKi = 0.1,
	.fKd = 0,
};

/* ����PID */
tagPID_T YawPID = 
{
	.fKp = 1,
	.fKi = 0.2,
	.fKd = 0.1,
};

/* �ǶȻ�PID */
tagPID_T AngleLoopPID = 
{
	.fKp = 1,
	.fKi = 0,
	.fKd = 0,
};

/* λ�û�PID */
tagPID_T PositionLoopPID = 
{
	.fKp = 1,
	.fKi = 0,
	.fKd = 0,
};

/* ƽ��PID */
tagPID_T BalancePID = 
{
	.fKp = 3,
	.fKi = 0,
	.fKd = 0.1,
};

/* ����1��ʼ����� */
tagUART_T Uart1 = 
{	
	//����DMA���ղ�������
	.tUartDMA.bRxEnable					= true,						/* DMA����ʹ�� */
	.tRxInfo.usDMARxMAXSize             = 100,              		/* �������ݳ��� ���ȱ�����Э����ֽ�*2���ϣ�ȷ�������һ���ܹ��ȶ�����һ������������֡*/
};

/* ����3��ʼ����� */
tagUART_T Uart3 = 
{
	.tUARTHandle.Instance				= USART3,
	//����DMA���ղ�������
	.tUartDMA.bRxEnable					= true,						/* DMA����ʹ�� */
	.tRxInfo.usDMARxMAXSize             = 100,              		/* �������ݳ��� ���ȱ�����Э����ֽ�*2���ϣ�ȷ�������һ���ܹ��ȶ�����һ������������֡*/
};

/* JY901S�������� */
tagJY901_T JY901S = 
{
	.tConfig.ucBaud 	= JY901_RXBAUD_9600,
	.tConfig.ucRate		= JY901_RX_2HZ,
	.tConfig.usType		= JY901_OUTPUT_ANGLE,

	.tUART.tRxInfo.usDMARxMAXSize             	= 100,                 /* �������ݳ��� ���ȱ�����Э����ֽ�*2���ϣ�ȷ�������һ���ܹ��ȶ�����һ������������֡*/

    .tUART.tUartDMA.bRxEnable					= true,					/* DMA����ʹ�� */
};

/* MS5837ʾ�� */
tagMS5837_T MS5837 = 
{
    /* �����ֱ������� */
	.setOSR = MS5837_OSR4096,

	/* SCL�� */
	.tIIC.tIICSoft[0].tGPIOInit.Pin 		= GPIO_PIN_6,				/* GPIO���� */
	.tIIC.tIICSoft[0].tGPIOInit.Mode 		= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
	.tIIC.tIICSoft[0].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tIIC.tIICSoft[0].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tIIC.tIICSoft[0].tGPIOPort 			= GPIOC,					/* GPIO���� */

	/* SDA�� */
	.tIIC.tIICSoft[1].tGPIOInit.Pin 		= GPIO_PIN_7,				/* GPIO���� */
	.tIIC.tIICSoft[1].tGPIOInit.Mode		= GPIO_MODE_INPUT,			/* GPIOģʽ */
	.tIIC.tIICSoft[1].tGPIOInit.Pull		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tIIC.tIICSoft[1].tGPIOInit.Speed		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tIIC.tIICSoft[1].tGPIOPort 			= GPIOC,					/* GPIO���� */
};

/* PWM�������� */
tagPWM_T PWM[] =
{
	//Ĭ��ʹ��ǰ4��IO�����PWM
	[0] =
	{
		.tPWMHandle.Instance	= TIM1,         	/* ��ʱ��1 */
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
		.ucChannel				= TIM_CHANNEL_2,	/* ͨ�� */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_11,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOE,			/* IO��ӳ�� */
		.tGPIO.ucAFMode			= FULL_REMAP,		/* IO��ӳ��ģʽ */
	},
	[1] =
	{
		.tPWMHandle.Instance	= TIM1,         	/* ��ʱ��1 */
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
		.ucChannel				= TIM_CHANNEL_4,	/* ͨ�� */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_14,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOE,			/* IO��ӳ�� */
		.tGPIO.ucAFMode			= FULL_REMAP,		/* IO��ӳ��ģʽ */
	},
	[2] =
	{
		.tPWMHandle.Instance	= TIM4,         	/* ��ʱ��4 */
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
		.ucChannel				= TIM_CHANNEL_1,	/* ͨ�� */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_6,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOB,			/* IO��ӳ�� */
		.tGPIO.ucAFMode			= NO_REMAP,			/* IO��ӳ��ģʽ */
	},	
	[3] =
	{
		.tPWMHandle.Instance	= TIM4,         	/* ��ʱ��4 */
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
		.ucChannel				= TIM_CHANNEL_3,	/* ͨ�� */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_8,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOB,			/* IO��ӳ�� */
		.tGPIO.ucAFMode			= NO_REMAP,			/* IO��ӳ��ģʽ */
	},	
	//����4��IO��Ϊ���ÿ�  
	[4] =
	{
		.tPWMHandle.Instance	= TIM1,         	/* ��ʱ��1 */
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
		.ucChannel				= TIM_CHANNEL_1,	/* ͨ�� */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_9,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOE,			/* IO��ӳ�� */
		.tGPIO.ucAFMode			= FULL_REMAP,		/* IO��ӳ��ģʽ */
	},
	[5] =
	{
		.tPWMHandle.Instance	= TIM4,         	/* ��ʱ��4 */
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
		.ucChannel				= TIM_CHANNEL_2,	/* ͨ�� */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_7,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOB,			/* IO��ӳ�� */
		.tGPIO.ucAFMode			= NO_REMAP,			/* IO��ӳ��ģʽ */
	},	
	[6] =
	{
		.tPWMHandle.Instance	= TIM1,         	/* ��ʱ��1 */
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
		.ucChannel				= TIM_CHANNEL_3,	/* ͨ�� */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_13,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOE,			/* IO��ӳ�� */
		.tGPIO.ucAFMode			= FULL_REMAP,		/* IO��ӳ��ģʽ */
	},
	[7] =
	{
		.tPWMHandle.Instance	= TIM4,         	/* ��ʱ��4 */
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
		.ucChannel				= TIM_CHANNEL_4,	/* ͨ�� */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_9,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOB,			/* IO��ӳ�� */
		.tGPIO.ucAFMode			= NO_REMAP,			/* IO��ӳ��ģʽ */
	},
};
