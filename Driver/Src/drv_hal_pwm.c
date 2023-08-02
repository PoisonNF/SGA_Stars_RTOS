/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_pwm.c

* ���ݼ�����PWM�����ļ�

* �ļ���ʷ��

* �汾��	   ����		  	����		  ˵��
*  2.6     	2023-06-07   �����		���Ĳ��ֺ�������

* 2.3.1 	2023-05-05   �����		��ʽ�Ż�

* 1.1.7 	2022-10-11   �����		�Ż�ִ��˳��

* 1.1.2 	2022-08-08	 �����		������ʱ��������ӳ�����

* 1.0.0a 	2020-02-22	 ���		�������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_PWM_ENABLE

/**
 * @brief PWM��ʱ��ʹ��
 * @param _tPWM-PWM�ṹ��ָ��
 * @retval Null
*/
static void S_PWM_CLKEnable(tagPWM_T *_tPWM)
{
	if(_tPWM->tPWMHandle.Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();	/* ʹ�ܶ�ʱ��1 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();	/* ʹ�ܶ�ʱ��2 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();	/* ʹ�ܶ�ʱ��3 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();	/* ʹ�ܶ�ʱ��4 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();	/* ʹ�ܶ�ʱ��5 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM8)
	{
		__HAL_RCC_TIM8_CLK_ENABLE();	/* ʹ�ܶ�ʱ��8 */
	}
}

/**
 * @brief PWM����������
 * @param _tPWM-PWM�ṹ��ָ��
 * @retval Null
*/
static void S_PWM_PramConfig(tagPWM_T *_tPWM)
{	
	_tPWM->tPWMHandle.Init.Prescaler			= SYSTEM_CLOCK - 1;         		/* ��ʱ����Ƶ */
	_tPWM->tPWMHandle.Init.CounterMode			= TIM_COUNTERMODE_UP;			    /* ���ϼ���ģʽ */
	_tPWM->tPWMHandle.Init.Period				= MAX_RELOAD /_tPWM->ulFreq - 1;	/* �Զ���װ��ֵ */
	_tPWM->tPWMHandle.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	_tPWM->tPWMHandle.Init.AutoReloadPreload 	= TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&_tPWM->tPWMHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
    
	/* ģʽѡ��PWM1 */
	_tPWM->tPWMChannel.OCMode		= TIM_OCMODE_PWM1;
    
 	/* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50% */    
	_tPWM->tPWMChannel.Pulse		= (MAX_RELOAD / _tPWM->ulFreq -1) * _tPWM->fDuty / 100;
    
    /* ����Ƚϼ���Ϊ�� */
	_tPWM->tPWMChannel.OCPolarity	= TIM_OCPOLARITY_HIGH;
	
	if(HAL_TIM_PWM_ConfigChannel(&_tPWM->tPWMHandle,&_tPWM->tPWMChannel,_tPWM->ucChannel))	/* ����ͨ�� */
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}				
	if(HAL_TIM_PWM_Start(&_tPWM->tPWMHandle,_tPWM->ucChannel))	/* ����ͨ�� */
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
}

/**
 * @brief PWM��ӦGPIO����
 * @param _tPWM-PWM�ṹ��ָ��
 * @retval Null
*/
static void S_PWM_GPIOConfig(tagPWM_T *_tPWM)
{
	/*PWM����Ĭ��I/O����*/
	_tPWM->tGPIO.tGPIOInit.Mode	 = GPIO_MODE_AF_PP;			/* ����������� */
	_tPWM->tGPIO.tGPIOInit.Pull	 = GPIO_PULLUP;				/* ���� */
	_tPWM->tGPIO.tGPIOInit.Speed = GPIO_SPEED_FREQ_HIGH;	/* ����ģʽ */

	/* ��������ģʽʱ�� */
	__HAL_RCC_AFIO_CLK_ENABLE();

	/* ���ݲ�ͬ��ʱ����ucAFMode������Ӧ����ӳ�䣬��ӳ�����drv_hal_pwm.h�� */
	if(_tPWM->tPWMHandle.Instance == TIM1)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM1_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    __HAL_AFIO_REMAP_TIM1_PARTIAL();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	while(1);
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM1_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM2)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM2_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    __HAL_AFIO_REMAP_TIM2_PARTIAL_1();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	__HAL_AFIO_REMAP_TIM2_PARTIAL_2();
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM2_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM3)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM3_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    __HAL_AFIO_REMAP_TIM3_PARTIAL();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	while(1);
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM3_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM4)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM4_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    while(1);
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	while(1);
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM4_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM5 && _tPWM->ucChannel == TIM_CHANNEL_4)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM5CH4_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP) 		__HAL_AFIO_REMAP_TIM5CH4_ENABLE();
	}


	Drv_GPIO_Init(&_tPWM->tGPIO, 1);		/* GPIO��ʼ�� */
}

/**
 * @brief PWM��ռ�ձ�����
 * @param _tPWM-PWM�ṹ��ָ��
 * @param _fDuty-ռ�ձ�(��λ��%)
 * @retval Null
*/
void Drv_PWM_DutyFactorSet(tagPWM_T *_tPWM, float _fDuty)
{
	switch(_tPWM->ucChannel)
	{
		case TIM_CHANNEL_1:
			_tPWM->tPWMHandle.Instance->CCR1 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		case TIM_CHANNEL_2:
			_tPWM->tPWMHandle.Instance->CCR2 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		case TIM_CHANNEL_3:
			_tPWM->tPWMHandle.Instance->CCR3 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		case TIM_CHANNEL_4:
			_tPWM->tPWMHandle.Instance->CCR4 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		default:
			Drv_HAL_Error(__FILE__, __LINE__);
		break;
	}
}

/**
 * @brief PWM���ߵ�ƽʱ������
 * @param _tPWM-PWM�ṹ��ָ��
 * @param _usTime-�ߵ�ƽʱ��(��λ��us)
 * @note ��PWM��Ƶ��Ϊ50HZ��ǰ����
 * @retval Null
*/
void Drv_PWM_HighLvTimeSet(tagPWM_T *_tPWM, uint16_t _usTime)
{
	if(_tPWM->ulFreq == 50)
	{
		switch(_tPWM->ucChannel)
		{
			case TIM_CHANNEL_1:
				_tPWM->tPWMHandle.Instance->CCR1 = _usTime;
			break;
			
			case TIM_CHANNEL_2:
				_tPWM->tPWMHandle.Instance->CCR2 = _usTime;
			break;
			
			case TIM_CHANNEL_3:
				_tPWM->tPWMHandle.Instance->CCR3 = _usTime;
			break;
			
			case TIM_CHANNEL_4:
				_tPWM->tPWMHandle.Instance->CCR4 = _usTime;
			break;
			
			default:
				Drv_HAL_Error(__FILE__, __LINE__);
			break;
		}
	}
}

/**
 * @brief PWM��ʼ��
 * @param _tPWM-PWM�ṹ��ָ��
 * @param _ucNum-PWM����
 * @retval Null
*/
void Drv_PWM_Init(tagPWM_T *_tPWM, uint8_t _ucNum)
{
	uint8_t index;
	
	for(index = 0;index < _ucNum;index++)
	{
		S_PWM_CLKEnable(&_tPWM[index]);
		S_PWM_GPIOConfig(&_tPWM[index]);
		S_PWM_PramConfig(&_tPWM[index]);
	}
}
#endif
