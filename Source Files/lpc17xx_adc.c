/**********************************************************************
* $Id$		lpc17xx_adc.c
*//**
* @file		lpc17xx_adc.c
* @brief	Contains all functions support for ADC firmware library on LPC17xx
* @version	1.0
* @date		06. Dec. 2013
* @author	Dwijay.Edutech Learning Solutions
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup ADC
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_adc.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		ADC interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void ADC_IRQHandler(void)
{
/*	if (ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_4,ADC_DATA_DONE))
	{
		adc_value =  ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_4);
		NVIC_DisableIRQ(ADC_IRQn);
	}*/
}


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup ADC_Public_Functions
 * @{
 */

/********************************************************************//**
 * @brief		Initializes the ADCx Peripheral according to the specified
 *               parameters.
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void ADC_Config (void)
{
	/** Configuration for ADC :
	 *  ADC conversion rate = 200Khz
	 */
	ADC_Init(LPC_ADC, 200000);

	/* ADC Channel Config */
	ADC_Channel_Config (LPC_ADC, ADC_CHANNEL_4, DISABLE);
	ADC_Channel_Config (LPC_ADC, ADC_CHANNEL_0, DISABLE);
}


/********************************************************************//**
 * @brief		Initializes the ADCx channels according to the specified
 *               parameters.
 * @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
 * @return 		None
 *********************************************************************/
void ADC_Channel_Config(LPC_ADC_TypeDef *ADCx, ADC_CHANNEL_SELECTION PCfg, FunctionalState IntState)
{
	// Pin configuration for ADC
	PINSEL_CFG_Type PinCfg;

	if(ADCx == LPC_ADC)
	{
		switch (PCfg)
		{
		 case ADC_CHANNEL_0:
			 // Configure P0.23 as CH0
			 PinCfg.Funcnum = 1;
			 PinCfg.OpenDrain = 0;
			 PinCfg.Pinmode = 0;
			 PinCfg.Pinnum = 23;
			 PinCfg.Portnum = 0;
			 PINSEL_ConfigPin(&PinCfg);

			 ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, IntState);

			 break;

		 case ADC_CHANNEL_1:
			 // Configure P0.24 as CH1
			 PinCfg.Funcnum = 1;
			 PinCfg.OpenDrain = 0;
			 PinCfg.Pinmode = 0;
			 PinCfg.Pinnum = 24;
			 PinCfg.Portnum = 0;
			 PINSEL_ConfigPin(&PinCfg);

			 ADC_IntConfig(LPC_ADC, ADC_ADINTEN1, IntState);

			 break;

		 case ADC_CHANNEL_2:
			 // Configure P0.25 as CH2
			 PinCfg.Funcnum = 1;
			 PinCfg.OpenDrain = 0;
			 PinCfg.Pinmode = 0;
			 PinCfg.Pinnum = 25;
			 PinCfg.Portnum = 0;
			 PINSEL_ConfigPin(&PinCfg);

			 ADC_IntConfig(LPC_ADC, ADC_ADINTEN2, IntState);

			 break;

		 case ADC_CHANNEL_3:
			 // Configure P0.26 as CH3
			 PinCfg.Funcnum = 1;
			 PinCfg.OpenDrain = 0;
			 PinCfg.Pinmode = 0;
			 PinCfg.Pinnum = 23;
			 PinCfg.Portnum = 0;
			 PINSEL_ConfigPin(&PinCfg);

			 ADC_IntConfig(LPC_ADC, ADC_ADINTEN3, IntState);

			 break;

		 case ADC_CHANNEL_4:
			 // Configure P1.30 as CH4
			 PinCfg.Funcnum = 3;
			 PinCfg.OpenDrain = 0;
			 PinCfg.Pinmode = 0;
			 PinCfg.Pinnum = 30;
			 PinCfg.Portnum = 1;
			 PINSEL_ConfigPin(&PinCfg);

			 ADC_IntConfig(LPC_ADC, ADC_ADINTEN4, IntState);

			 break;

		 case ADC_CHANNEL_5:
			 // Configure P1.31 as CH5
			 PinCfg.Funcnum = 3;
			 PinCfg.OpenDrain = 0;
			 PinCfg.Pinmode = 0;
			 PinCfg.Pinnum = 31;
			 PinCfg.Portnum = 1;
			 PINSEL_ConfigPin(&PinCfg);

			 ADC_IntConfig(LPC_ADC, ADC_ADINTEN5, IntState);

			 break;
		}
	}

	if(IntState == ENABLE)
	{
		/* preemption = 1, sub-priority = 1 */
		NVIC_SetPriority(ADC_IRQn, 2);
		/* Enable ADC in NVIC */
		NVIC_EnableIRQ(ADC_IRQn);
		ADC_EdgeStartConfig(LPC_ADC,ADC_START_ON_FALLING);
	}

	ADC_ChannelCmd(LPC_ADC, PCfg, ENABLE);
}


/*********************************************************************//**
 * @brief 		Initial for ADC
 * 					+ Set bit PCADC
 * 					+ Set clock for ADC
 * 					+ Set Clock Frequency
 * @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
 * @param[in]	rate ADC conversion rate, should be <=200KHz
 * @return 		None
 **********************************************************************/
void ADC_Init(LPC_ADC_TypeDef *ADCx, uint32_t rate)
{
	uint32_t temp, tmp;

	CHECK_PARAM(PARAM_ADCx(ADCx));
	CHECK_PARAM(PARAM_ADC_RATE(rate));

	// Turn on power and clock
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCAD, ENABLE);

	ADCx->ADCR = 0;

	//Enable PDN bit
	tmp = ADC_CR_PDN;
	// Set clock frequency
	temp = CLKPWR_GetPCLK(CLKPWR_PCLKSEL_ADC);
	/* The APB clock (PCLK_ADC0) is divided by (CLKDIV+1) to produce the clock for
	 * A/D converter, which should be less than or equal to 13MHz.
	 * A fully conversion requires 65 of these clocks.
	 * ADC clock = PCLK_ADC0 / (CLKDIV + 1);
	 * ADC rate = ADC clock / 65;
	 */
	temp = (temp /(rate * 65)) - 1;
	tmp |=  ADC_CR_CLKDIV(temp);

	ADCx->ADCR = tmp;
}


/*********************************************************************//**
* @brief 		Close ADC
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @return 		None
**********************************************************************/
void ADC_DeInit(LPC_ADC_TypeDef *ADCx)
{
	CHECK_PARAM(PARAM_ADCx(ADCx));

	// Clear PDN bit
	ADCx->ADCR &= ~ADC_CR_PDN;
	// Turn on power and clock
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCAD, DISABLE);
}


/*********************************************************************//**
* @brief 		Get Result conversion from A/D data register
* @param[in]	channel number which want to read back the result
* @return 		Result of conversion
*********************************************************************/
uint32_t ADC_GetData(uint32_t channel)
{
	uint32_t adc_value;

	CHECK_PARAM(PARAM_ADC_CHANNEL_SELECTION(channel));

	adc_value = *(uint32_t *)((&LPC_ADC->ADDR0) + channel);
	return ADC_GDR_RESULT(adc_value);
}

/*********************************************************************//**
* @brief 		Set start mode for ADC
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	start_mode Start mode choose one of modes in
* 				'ADC_START_OPT' enumeration type definition, should be:
* 				- ADC_START_CONTINUOUS
* 				- ADC_START_NOW
* 				- ADC_START_ON_EINT0
* 				- ADC_START_ON_CAP01
*				- ADC_START_ON_MAT01
*				- ADC_START_ON_MAT03
*				- ADC_START_ON_MAT10
*				- ADC_START_ON_MAT11
* @return 		None
*********************************************************************/
void ADC_StartCmd(LPC_ADC_TypeDef *ADCx, uint8_t start_mode)
{
	CHECK_PARAM(PARAM_ADCx(ADCx));
	CHECK_PARAM(PARAM_ADC_START_OPT(start_mode));

	ADCx->ADCR &= ~ADC_CR_START_MASK;
	ADCx->ADCR |=ADC_CR_START_MODE_SEL((uint32_t)start_mode);
}


/*********************************************************************//**
* @brief 		ADC Burst mode setting
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	NewState
* 				-	1: Set Burst mode
* 				-	0: reset Burst mode
* @return 		None
**********************************************************************/
void ADC_BurstCmd(LPC_ADC_TypeDef *ADCx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_ADCx(ADCx));

	ADCx->ADCR &= ~ADC_CR_BURST;
	if (NewState){
		ADCx->ADCR |= ADC_CR_BURST;
	}
}

/*********************************************************************//**
* @brief 		Set AD conversion in power mode
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	NewState
* 				-	1: AD converter is optional
* 				-	0: AD Converter is in power down mode
* @return 		None
**********************************************************************/
void ADC_PowerdownCmd(LPC_ADC_TypeDef *ADCx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_ADCx(ADCx));

	ADCx->ADCR &= ~ADC_CR_PDN;
	if (NewState){
		ADCx->ADCR |= ADC_CR_PDN;
	}
}

/*********************************************************************//**
* @brief 		Set Edge start configuration
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	EdgeOption is ADC_START_ON_RISING and ADC_START_ON_FALLING
* 					0:ADC_START_ON_RISING
* 					1:ADC_START_ON_FALLING
* @return 		None
**********************************************************************/
void ADC_EdgeStartConfig(LPC_ADC_TypeDef *ADCx, uint8_t EdgeOption)
{
	CHECK_PARAM(PARAM_ADCx(ADCx));
	CHECK_PARAM(PARAM_ADC_START_ON_EDGE_OPT(EdgeOption));

	ADCx->ADCR &= ~ADC_CR_EDGE;
	if (EdgeOption){
		ADCx->ADCR |= ADC_CR_EDGE;
	}
}

/*********************************************************************//**
* @brief 		ADC interrupt configuration
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	IntType: type of interrupt, should be:
* 				- ADC_ADINTEN0: Interrupt channel 0
* 				- ADC_ADINTEN1: Interrupt channel 1
* 				...
* 				- ADC_ADINTEN7: Interrupt channel 7
* 				- ADC_ADGINTEN: Individual channel/global flag done generate an interrupt
* @param[in]	NewState:
* 					- SET : enable ADC interrupt
* 					- RESET: disable ADC interrupt
* @return 		None
**********************************************************************/
void ADC_IntConfig (LPC_ADC_TypeDef *ADCx, ADC_TYPE_INT_OPT IntType, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_ADCx(ADCx));
	CHECK_PARAM(PARAM_ADC_TYPE_INT_OPT(IntType));

	ADCx->ADINTEN &= ~ADC_INTEN_CH(IntType);
	if (NewState){
		ADCx->ADINTEN |= ADC_INTEN_CH(IntType);
	}
}

/*********************************************************************//**
* @brief 		Enable/Disable ADC channel number
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	Channel channel number
* @param[in]	NewState Enable or Disable
*
* @return 		None
**********************************************************************/
void ADC_ChannelCmd (LPC_ADC_TypeDef *ADCx, uint8_t Channel, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_ADCx(ADCx));
	CHECK_PARAM(PARAM_ADC_CHANNEL_SELECTION(Channel));

	if (NewState == ENABLE) {
		ADCx->ADCR |= ADC_CR_CH_SEL(Channel);
	} else {
		ADCx->ADCR &= ~ADC_CR_CH_SEL(Channel);
	}
}

/*********************************************************************//**
* @brief 		Get ADC result
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	channel: channel number, should be 0...7
* @return 		Data conversion
**********************************************************************/
uint16_t ADC_ChannelGetData(LPC_ADC_TypeDef *ADCx, uint8_t channel)
{
	uint32_t adc_value;

	CHECK_PARAM(PARAM_ADCx(ADCx));
	CHECK_PARAM(PARAM_ADC_CHANNEL_SELECTION(channel));

	adc_value = *(uint32_t *) ((&ADCx->ADDR0) + channel);
	return ADC_DR_RESULT(adc_value);
}

/*********************************************************************//**
* @brief 		Get ADC Chanel status from ADC data register
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	channel: channel number, should be 0..7
* @param[in]  	StatusType
*              		 	0:Burst status
*               		1:Done 	status
* @return 		SET / RESET
**********************************************************************/
FlagStatus ADC_ChannelGetStatus(LPC_ADC_TypeDef *ADCx, uint8_t channel, uint32_t StatusType)
{
	uint32_t temp;

	CHECK_PARAM(PARAM_ADCx(ADCx));
	CHECK_PARAM(PARAM_ADC_CHANNEL_SELECTION(channel));
	CHECK_PARAM(PARAM_ADC_DATA_STATUS(StatusType));

	temp =  *(uint32_t *) ((&ADCx->ADDR0) + channel);
	if (StatusType) {
		temp &= ADC_DR_DONE_FLAG;
	}else{
		temp &= ADC_DR_OVERRUN_FLAG;
	}
	if (temp) {
		return SET;
	} else {
		return RESET;
	}

}

/*********************************************************************//**
* @brief 		Get ADC Data from AD Global register
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @return 		Result of conversion
**********************************************************************/
uint32_t ADC_GlobalGetData(LPC_ADC_TypeDef *ADCx)
{
	CHECK_PARAM(PARAM_ADCx(ADCx));

	return ((uint32_t)(ADCx->ADGDR));
}

/*********************************************************************//**
* @brief 		Get ADC Chanel status from AD global data register
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]  	StatusType
*              		 	0:Burst status
*               		1:Done 	status
* @return 		SET / RESET
**********************************************************************/
FlagStatus	ADC_GlobalGetStatus(LPC_ADC_TypeDef *ADCx, uint32_t StatusType)
{
	uint32_t temp;

	CHECK_PARAM(PARAM_ADCx(ADCx));
	CHECK_PARAM(PARAM_ADC_DATA_STATUS(StatusType));

	temp =  ADCx->ADGDR;
	if (StatusType){
		temp &= ADC_DR_DONE_FLAG;
	}else{
		temp &= ADC_DR_OVERRUN_FLAG;
	}
	if (temp){
		return SET;
	}else{
		return RESET;
	}
}

/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

