/**********************************************************************
* $Id$		lpc17xx_spi.c
*//**
* @file		lpc17xx_spi.c
* @brief	Contains all functions support for SPI firmware library on LPC17xx
* @version	1.0
* @date		07. Dec. 2013
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
/** @addtogroup SPI
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_spi.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup SPI_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief 		Initialize CS pin as GPIO function to drive /CS pin
 * 				due to definition of CS_PORT_NUM and CS_PORT_NUM
 * @param		None
 * @return		None
 ***********************************************************************/
void CS_Init (void)
{
	GPIO_SetDir(0, _BIT(16), 1);
	GPIO_SetValue(0, _BIT(16));
}


/*********************************************************************//**
 * @brief 		Drive CS output pin to low/high level to select slave device
 * 				via /CS pin state
 * @param[in]	state State of CS output pin that will be driven:
 * 				- 0: Drive CS pin to low level
 * 				- 1: Drive CS pin to high level
 * @return		None
 ***********************************************************************/
void CS_Force (FunctionalState state)
{
	if (state)
	{
		GPIO_SetValue(0, _BIT(16));
	}
	else
	{
		GPIO_ClearValue(0, _BIT(16));
	}
}


/*********************************************************************//**
 * @brief		Initialize buffer
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Buffer_Init (void)
{
	uint8_t i;
#if (SPI_DATABIT_SIZE == 8)
	for (i = 0; i < BUFFER_SIZE; i++)
	{
		Tx_Buf[i] = 0;
		Rx_Buf[i] = 0;
	}
#else
	for (i = 0; i < BUFFER_SIZE/2; i++)
	{
		Tx_Buf[i] = 0;
		Rx_Buf[i] = 0;
	}
#endif
}


/*********************************************************************//**
 * @brief 		Initialize and Configure SPI device
 * @param[in] 	SPIx	SPI peripheral definition, should be LPC_SPI
 * @return 		None
 ***********************************************************************/
void SPI_Config (LPC_SPI_TypeDef *SPIx)
{
	// Pin configuration for SPI
	PINSEL_CFG_Type PinCfg;

	// SPI Configuration structure variable
	SPI_CFG_Type SPI_ConfigStruct;

	if(SPIx == LPC_SPI)
	{
		/*
		 * Initialize SPI pin connect
		 * P0.15 - SCK;
		 * P0.16 - SSEL - used as GPIO
		 * P0.17 - MISO
		 * P0.18 - MOSI
		 */
		PinCfg.Funcnum = 3;
		PinCfg.OpenDrain = 0;
		PinCfg.Pinmode = 0;
		PinCfg.Portnum = 0;
		PinCfg.Pinnum = 15;
		PINSEL_ConfigPin(&PinCfg);
		PinCfg.Pinnum = 17;
		PINSEL_ConfigPin(&PinCfg);
		PinCfg.Pinnum = 18;
		PINSEL_ConfigPin(&PinCfg);
		PinCfg.Pinnum = 16;
		PinCfg.Funcnum = 0;
		PINSEL_ConfigPin(&PinCfg);
	}
/*
	SPI_ConfigStruct.CPHA = SPI_CPHA_SECOND;
	SPI_ConfigStruct.CPOL = SPI_CPOL_LO;
	SPI_ConfigStruct.ClockRate = SPI_CLOCK;
	SPI_ConfigStruct.DataOrder = SPI_DATA_MSB_FIRST;
	SPI_ConfigStruct.Databit = SPI_DATABIT_8;
	SPI_ConfigStruct.Mode = SPI_MASTER_MODE;*/

	// initialize SPI configuration structure to default
	SPI_ConfigStructInit(&SPI_ConfigStruct);

	SPI_ConfigStruct.ClockRate = 3000000;
	SPI_ConfigStruct.Databit = SPI_DATABIT_8;

	// Initialize SPI peripheral with parameter given in structure above
	SPI_Init(LPC_SPI, &SPI_ConfigStruct);

	CS_Init();     // Chip Select Init

	Buffer_Init(); // Empty Buffer
}


/*********************************************************************//**
 * @brief 		Setup clock rate for SPI device
 * @param[in] 	SPIx	SPI peripheral definition, should be LPC_SPI
 * @param[in]	target_clock : clock of SPI (Hz)
 * @return 		None
 ***********************************************************************/
void SPI_SetClock (LPC_SPI_TypeDef *SPIx, uint32_t target_clock)
{
	uint32_t spi_pclk;
	uint32_t prescale, temp;

	CHECK_PARAM(PARAM_SPIx(SPIx));

	if (SPIx == LPC_SPI){
		spi_pclk =  CLKPWR_GetPCLK (CLKPWR_PCLKSEL_SPI);
	} else {
		return;
	}

	prescale = 8;
	// Find closest clock to target clock
	while (1){
		temp = target_clock * prescale;
		if (temp >= spi_pclk){
			break;
		}
		prescale += 2;
		if(prescale >= 254){
			break;
		}
	}

	// Write to register
	SPIx->SPCCR = SPI_SPCCR_COUNTER(prescale);
}


/*********************************************************************//**
 * @brief		De-initializes the SPIx peripheral registers to their
*                  default reset values.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 		None
 **********************************************************************/
void SPI_DeInit(LPC_SPI_TypeDef *SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	if (SPIx == LPC_SPI){
		/* Set up clock and power for SPI module */
		CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCSPI, DISABLE);
	}
}

/*********************************************************************//**
 * @brief		Get data bit size per transfer
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 		number of bit per transfer, could be 8-16
 **********************************************************************/
uint8_t SPI_GetDataSize (LPC_SPI_TypeDef *SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));
	return ((SPIx->SPCR)>>8 & 0xF);
}

/********************************************************************//**
 * @brief		Initializes the SPIx peripheral according to the specified
*               parameters in the UART_ConfigStruct.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @param[in]	SPI_ConfigStruct Pointer to a SPI_CFG_Type structure
*                    that contains the configuration information for the
*                    specified SPI peripheral.
 * @return 		None
 *********************************************************************/
void SPI_Init(LPC_SPI_TypeDef *SPIx, SPI_CFG_Type *SPI_ConfigStruct)
{
	uint32_t tmp;

	CHECK_PARAM(PARAM_SPIx(SPIx));

	if(SPIx == LPC_SPI){
		/* Set up clock and power for UART module */
		CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCSPI, ENABLE);
	} else {
		return;
	}

	// Configure SPI, interrupt is disable as default
	tmp = ((SPI_ConfigStruct->CPHA) | (SPI_ConfigStruct->CPOL) \
		| (SPI_ConfigStruct->DataOrder) | (SPI_ConfigStruct->Databit) \
		| (SPI_ConfigStruct->Mode) | SPI_SPCR_BIT_EN) & SPI_SPCR_BITMASK;
	// write back to SPI control register
	SPIx->SPCR = tmp;

	// Set clock rate for SPI peripheral
	SPI_SetClock(SPIx, SPI_ConfigStruct->ClockRate);

	// If interrupt flag is set, Write '1' to Clear interrupt flag
	if (SPIx->SPINT & SPI_SPINT_INTFLAG){
		SPIx->SPINT = SPI_SPINT_INTFLAG;
	}
}



/*****************************************************************************//**
* @brief		Fills each SPI_InitStruct member with its default value:
* 				- CPHA = SPI_CPHA_FIRST
* 				- CPOL = SPI_CPOL_HI
* 				- ClockRate = 1000000
* 				- DataOrder = SPI_DATA_MSB_FIRST
* 				- Databit = SPI_DATABIT_8
* 				- Mode = SPI_MASTER_MODE
* @param[in]	SPI_InitStruct Pointer to a SPI_CFG_Type structure
*                    which will be initialized.
* @return		None
*******************************************************************************/
void SPI_ConfigStructInit(SPI_CFG_Type *SPI_InitStruct)
{
	SPI_InitStruct->CPHA = SPI_CPHA_FIRST;
	SPI_InitStruct->CPOL = SPI_CPOL_HI;
	SPI_InitStruct->ClockRate = 1000000;
	SPI_InitStruct->DataOrder = SPI_DATA_MSB_FIRST;
	SPI_InitStruct->Databit = SPI_DATABIT_8;
	SPI_InitStruct->Mode = SPI_MASTER_MODE;
}

/*********************************************************************//**
 * @brief		Transmit a single data through SPIx peripheral
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @param[in]	Data	Data to transmit (must be 16 or 8-bit long,
 * 						this depend on SPI data bit number configured)
 * @return 		none
 **********************************************************************/
void SPI_SendData(LPC_SPI_TypeDef* SPIx, uint16_t Data)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	SPIx->SPDR = Data & SPI_SPDR_BITMASK;
}



/*********************************************************************//**
 * @brief		Receive a single data from SPIx peripheral
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 		Data received (16-bit long)
 **********************************************************************/
uint16_t SPI_ReceiveData(LPC_SPI_TypeDef* SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	return ((uint16_t) (SPIx->SPDR & SPI_SPDR_BITMASK));
}

/*********************************************************************//**
 * @brief 		SPI 	Read write data function
 * @param[in]	SPIx 	Pointer to SPI peripheral, should be LPC_SPI
 * @param[in]	dataCfg	Pointer to a SPI_DATA_SETUP_Type structure that
 * 						contains specified information about transmit
 * 						data configuration.
 * @param[in]	xfType	Transfer type, should be:
 * 						- SPI_TRANSFER_POLLING: Polling mode
 * 						- SPI_TRANSFER_INTERRUPT: Interrupt mode
 * @return 		Actual Data length has been transferred in polling mode.
 * 				In interrupt mode, always return (0)
 * 				Return (-1) if error.
 * Note: This function can be used in both master and slave mode.
 ***********************************************************************/
int32_t SPI_ReadWrite (LPC_SPI_TypeDef *SPIx, SPI_DATA_SETUP_Type *dataCfg, \
						SPI_TRANSFER_Type xfType)
{
	uint8_t *rdata8;
    uint8_t *wdata8;
	uint16_t *rdata16;
    uint16_t *wdata16;
    uint32_t stat;
    uint32_t temp;
    uint8_t dataword;

	//read for empty buffer
	temp = SPIx->SPDR;
	//dummy to clear status
	temp = SPIx->SPSR;
	dataCfg->counter = 0;
	dataCfg->status = 0;

	if(SPI_GetDataSize (SPIx) == 8)
		dataword = 0;
	else dataword = 1;
	if (xfType == SPI_TRANSFER_POLLING)
	{
		if (dataword == 0)
		{
			rdata8 = (uint8_t *)dataCfg->rx_data;
			wdata8 = (uint8_t *)dataCfg->tx_data;
		}
		else
		{
			rdata16 = (uint16_t *)dataCfg->rx_data;
			wdata16 = (uint16_t *)dataCfg->tx_data;
		}

		while(dataCfg->counter < dataCfg->length)
		{
			// Write data to buffer
			if(dataCfg->tx_data == NULL)
			{
				if (dataword == 0)
				{
					SPI_SendData(SPIx, 0xFF);
				}
				else
				{
					SPI_SendData(SPIx, 0xFFFF);
				}
			}
			else
			{
				if (dataword == 0)
				{
					SPI_SendData(SPIx, *wdata8);
					wdata8++;
				}
				else
				{
					SPI_SendData(SPIx, *wdata16);
					wdata16++;
				}
			}
			// Wait for transfer complete
			while (!((stat = SPIx->SPSR) & SPI_SPSR_SPIF));
			// Check for error
			if (stat & (SPI_SPSR_ABRT | SPI_SPSR_MODF | SPI_SPSR_ROVR | SPI_SPSR_WCOL))
			{
				// save status
				dataCfg->status = stat | SPI_STAT_ERROR;
				return (dataCfg->counter);
			}
			// Read data from SPI dat
			temp = (uint32_t) SPI_ReceiveData(SPIx);

			// Store data to destination
			if (dataCfg->rx_data != NULL)
			{
				if (dataword == 0)
				{
					*(rdata8) = (uint8_t) temp;
					rdata8++;
				}
				else
				{
					*(rdata16) = (uint16_t) temp;
					rdata16++;
				}
			}
			// Increase counter
			if (dataword == 0)
			{
				dataCfg->counter++;
			} else
			{
				dataCfg->counter += 2;
			}
		}

		// Return length of actual data transferred
		// save status
		dataCfg->status = stat | SPI_STAT_DONE;
		return (dataCfg->counter);
	}
	// Interrupt mode
	else
	{
		// Check if interrupt flag is already set
		if(SPIx->SPINT & SPI_SPINT_INTFLAG)
		{
			SPIx->SPINT = SPI_SPINT_INTFLAG;
		}
		if (dataCfg->counter < dataCfg->length)
		{
			// Write data to buffer
			if(dataCfg->tx_data == NULL)
			{
				if (dataword == 0)
				{
					SPI_SendData(SPIx, 0xFF);
				}
				else
				{
					SPI_SendData(SPIx, 0xFFFF);
				}
			}
			else
			{
				if (dataword == 0)
				{
					SPI_SendData(SPIx, (*(uint8_t *)dataCfg->tx_data));
				}
				else
				{
					SPI_SendData(SPIx, (*(uint16_t *)dataCfg->tx_data));
				}
			}
			SPI_IntCmd(SPIx, ENABLE);
		}
		else
		{
			// Save status
			dataCfg->status = SPI_STAT_DONE;
		}
		return (0);
	}
}


/********************************************************************//**
 * @brief 		Enable or disable SPIx interrupt.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @param[in]	NewState New state of specified UART interrupt type,
 * 				should be:
 * 				- ENALBE: Enable this SPI interrupt.
* 				- DISALBE: Disable this SPI interrupt.
 * @return 		None
 *********************************************************************/
void SPI_IntCmd(LPC_SPI_TypeDef *SPIx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		SPIx->SPCR |= SPI_SPCR_SPIE;
	}
	else
	{
		SPIx->SPCR &= (~SPI_SPCR_SPIE) & SPI_SPCR_BITMASK;
	}
}


/********************************************************************//**
 * @brief 		Checks whether the SPI interrupt flag is set or not.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 		The new state of SPI Interrupt Flag (SET or RESET)
 *********************************************************************/
IntStatus SPI_GetIntStatus (LPC_SPI_TypeDef *SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	return ((SPIx->SPINT & SPI_SPINT_INTFLAG) ? SET : RESET);
}

/********************************************************************//**
 * @brief 		Clear SPI interrupt flag.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 		None
 *********************************************************************/
void SPI_ClearIntPending(LPC_SPI_TypeDef *SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	SPIx->SPINT = SPI_SPINT_INTFLAG;
}

/********************************************************************//**
 * @brief 		Get current value of SPI Status register in SPIx peripheral.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return		Current value of SPI Status register in SPI peripheral.
 * Note:	The return value of this function must be used with
 * 			SPI_CheckStatus() to determine current flag status
 * 			corresponding to each SPI status type. Because some flags in
 * 			SPI Status register will be cleared after reading, the next reading
 * 			SPI Status register could not be correct. So this function used to
 * 			read SPI status register in one time only, then the return value
 * 			used to check all flags.
 *********************************************************************/
uint32_t SPI_GetStatus(LPC_SPI_TypeDef* SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	return (SPIx->SPSR & SPI_SPSR_BITMASK);
}

/********************************************************************//**
 * @brief 		Checks whether the specified SPI Status flag is set or not
 * 				via inputSPIStatus parameter.
 * @param[in]	inputSPIStatus Value to check status of each flag type.
 * 				This value is the return value from SPI_GetStatus().
 * @param[in]	SPIStatus	Specifies the SPI status flag to check,
 * 				should be one of the following:
				- SPI_STAT_ABRT: Slave abort.
				- SPI_STAT_MODF: Mode fault.
				- SPI_STAT_ROVR: Read overrun.
				- SPI_STAT_WCOL: Write collision.
				- SPI_STAT_SPIF: SPI transfer complete.
 * @return 		The new state of SPIStatus (SET or RESET)
 *********************************************************************/
FlagStatus SPI_CheckStatus (uint32_t inputSPIStatus,  uint8_t SPIStatus)
{
	CHECK_PARAM(PARAM_SPI_STAT(SPIStatus));

	return ((inputSPIStatus & SPIStatus) ? SET : RESET);
}


/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
