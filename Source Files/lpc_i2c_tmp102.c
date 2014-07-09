/******************************************************************//**
* @file		lpc_i2c_tmp102.c
* @brief	Contains all functions support for I2C based TMP102
*           Temperature Sensor library on LPC17xx
* @version	1.0
* @date		11. Mar. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup TMP
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_i2c_tmp102.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

/** @addtogroup TMP_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */

/*********************************************************************//**
 * @brief	    Reads Configuration Register
 * @param[in]	None
 * @return 		status
 **********************************************************************/
uchar TMP102_Read_Config(void)
{
	uint32_t cfg_val;
	/* transmit setup */
	I2C_M_SETUP_Type txsetup;

	I2C_Tx_Buf[0] = CFG_REG;    /* Select Configuration Register */

	txsetup.sl_addr7bit = TMP102_ID;
	txsetup.tx_data = I2C_Tx_Buf;
	txsetup.tx_length = 1;
	txsetup.rx_data = I2C_Rx_Buf;
	txsetup.rx_length = 2;
	txsetup.retransmissions_max = 50;

	if (I2C_MasterTransferData(LPC_I2C0, &txsetup, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		cfg_val = (I2C_Rx_Buf[0]<<4)|(I2C_Rx_Buf[1]>>4);
		printf(LPC_UART0,"\x1b[06;01HConfiguration value : %x03",cfg_val);
		return (0);
	}
	else
	{
		return (-1);
	}
}


/*********************************************************************//**
 * @brief	    Sets Configuration Register value
 * @param[in]	None
 * @return 		status
 **********************************************************************/
uchar TMP102_Set_Config(uint16_t val)
{
	/* transmit setup */
	I2C_M_SETUP_Type txsetup;

	I2C_Tx_Buf[0] = CFG_REG;           /* Select Configuration Register */
	I2C_Tx_Buf[1] =  (val&0xFF00)>>8;  /* Byte1 MSB */
	I2C_Tx_Buf[2] =  (uint8_t)val;     /* Byte2 LSB */

	txsetup.sl_addr7bit = TMP102_ID;
	txsetup.tx_data = I2C_Tx_Buf;
	txsetup.tx_length = 3;
	txsetup.rx_data = NULL;
	txsetup.rx_length = 0;
	txsetup.retransmissions_max = 50;

	if (I2C_MasterTransferData(LPC_I2C0, &txsetup, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		return (0);
	}
	else
	{
		return (-1);
	}
}


/*********************************************************************//**
 * @brief	    Sets Threshold Register value
 * @param[in]   limit  - THIGH_VAL
 *                     - TLOW_VAL
 * @param[in]   deg    value in degC (for temp > 128 degC use 13bit RES)
 * @param[in]   res    - TMP102_12B
 *                     - TMP102_13B
 * @return 		status
 **********************************************************************/
uchar TMP102_Set_Threshold_Value(THRES_Type limit, int16_t deg, BIT_Type res)
{
	uint32_t val;
	Bool flag=FALSE;
	/* transmit setup */
	I2C_M_SETUP_Type txsetup;

	if(deg<0)                         /* Modulus the Output */
	{
		deg=-deg;
		flag=TRUE;
	}
	val =(deg*10000)/625;             /* Convert degrees into Value */

    if(flag==TRUE)                    /* If val in negative add sign bit for 12 bit mode*/
    {
    	val=~val+1;
    }

    if(limit==THIGH_VAL)
    {
    	I2C_Tx_Buf[0] = THIGH_REG;           /* Select Configuration Register */
    }
    else if(limit==TLOW_VAL)
    {
    	I2C_Tx_Buf[0] = TLOW_REG;           /* Select Configuration Register */
    }

	if(res==TMP102_12B)
	{
		I2C_Tx_Buf[1] =  (uint8_t)(val>>4);             /* Byte1 MSB */
		I2C_Tx_Buf[2] =  (uint8_t)((val&0xF)<<4);       /* Byte2 LSB */
	}
	else if(res==TMP102_13B)
	{
		I2C_Tx_Buf[1] =  (uint8_t)(val>>5);             /* Byte1 MSB */
		I2C_Tx_Buf[2] =  (uint8_t)((val&0x1F)<<3);       /* Byte2 LSB */
	}

	txsetup.sl_addr7bit = TMP102_ID;
	txsetup.tx_data = I2C_Tx_Buf;	     /* Get address to read at writing address */
	txsetup.tx_length = 3;
	txsetup.rx_data = NULL;
	txsetup.rx_length = 0;
	txsetup.retransmissions_max = 50;

	if (I2C_MasterTransferData(LPC_I2C0, &txsetup, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		return (0);
	}
	else
	{
		return (-1);
	}
}


/*********************************************************************//**
 * @brief	    Reads High Threshold Register
 * @param[in]	None
 * @return 		status
 **********************************************************************/
uchar TMP102_Read_Threshold_Value(THRES_Type limit, BIT_Type res)
{
	uint32_t temp_val, cal,dec;
	/* transmit setup */
	I2C_M_SETUP_Type txsetup;

	if(limit==THIGH_VAL)
	{
		I2C_Tx_Buf[0] = THIGH_REG;    /* Select Configuration Register */
	}
	else if(limit==TLOW_VAL)
	{
		I2C_Tx_Buf[0] = TLOW_REG;    /* Select Configuration Register */
	}

	txsetup.sl_addr7bit = TMP102_ID;
	txsetup.tx_data = I2C_Tx_Buf;	// Get address to read at writing address
	txsetup.tx_length = 1;
	txsetup.rx_data = I2C_Rx_Buf;
	txsetup.rx_length = 2;
	txsetup.retransmissions_max = 50;

	if (I2C_MasterTransferData(LPC_I2C0, &txsetup, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		if(res==TMP102_12B)
		{
			temp_val = (I2C_Rx_Buf[0]<<4)|(I2C_Rx_Buf[1]>>4);
		}
		else if(res==TMP102_13B)
		{
			temp_val = (I2C_Rx_Buf[0]<<5)|(I2C_Rx_Buf[1]>>3);
		}

		if((0x800&temp_val)&&(res==TMP102_12B))                    /* If negative?            */
		{
			temp_val = ((~temp_val)&0xFFF) + 1;           /* 2s compliment the value */
			cal = (temp_val*625)/10000;         /* Convert in deg C        */
			dec = ((temp_val*625)/100)%100;     /* get decimal point value */
			if(limit==THIGH_VAL)
			{
				printf(LPC_UART0,"\x1b[03;01HTHigh Limit in degC: -%d03.%d02",(uint16)cal,(uint16)dec);
			}
			else if(limit==TLOW_VAL)
			{
				printf(LPC_UART0,"\x1b[04;01HTLow Limit in degC: -%d03.%d02",(uint16)cal,(uint16)dec);
			}
		}
		else if((0x1000&temp_val)&&(res==TMP102_13B))                    /* If negative?            */
		{
			temp_val = ((~temp_val)&0x1FFF) + 1;           /* 2s compliment the value */
			cal = (temp_val*625)/10000;         /* Convert in deg C        */
			dec = ((temp_val*625)/100)%100;     /* get decimal point value */
			if(limit==THIGH_VAL)
			{
				printf(LPC_UART0,"\x1b[03;01HTHigh Limit in degC: -%d03.%d02",(uint16)cal,(uint16)dec);
			}
			else if(limit==TLOW_VAL)
			{
				printf(LPC_UART0,"\x1b[04;01HTLow Limit in degC: -%d03.%d02",(uint16)cal,(uint16)dec);
			}
		}
		else
		{
			cal = (temp_val*625)/10000;         /* Convert in deg C        */
			dec = ((temp_val*625)/100)%100;     /* get decimal point value */
			if(limit==THIGH_VAL)
			{
				printf(LPC_UART0,"\x1b[03;01HTHigh Limit in degC: %d03.%d02",(uint16)cal,(uint16)dec);
			}
			else if(limit==TLOW_VAL)
			{
				printf(LPC_UART0,"\x1b[04;01HTLow Limit in degC: %d03.%d02",(uint16)cal,(uint16)dec);
			}
		}

		return (0);
	}
	else
	{
		return (-1);
	}
}


/*********************************************************************//**
 * @brief	    Reads Temperature value in decimal
 *              for 13bit mode use TMP102_Set_Config(EXT_MODE);
 * @param[in]	res  -TMP102_12B
 *                   -TMP102_13B
 * @return 		status
 **********************************************************************/
uchar TMP102_Read_Temp(BIT_Type res)
{
	uint32_t temp_val, cal,dec;
	/* Receive setup */
	I2C_M_SETUP_Type rxsetup;

	I2C_Tx_Buf[0] = TMP_REG;    /* Select Configuration Register */

	rxsetup.sl_addr7bit = TMP102_ID;
	rxsetup.tx_data = I2C_Tx_Buf;	// Get address to read at writing address
	rxsetup.tx_length = 1;
	rxsetup.rx_data = I2C_Rx_Buf;
	rxsetup.rx_length = 2;
	rxsetup.retransmissions_max = 50;

	if (I2C_MasterTransferData(LPC_I2C0, &rxsetup, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		if(res==TMP102_12B)
		{
			temp_val = (I2C_Rx_Buf[0]<<4)|(I2C_Rx_Buf[1]>>4);
			printf(LPC_UART0,"\x1b[01;01HTemp Val    : %x03\n\r",(uint16_t)temp_val);
		}
		else if(res==TMP102_13B)
		{
			temp_val = (I2C_Rx_Buf[0]<<5)|(I2C_Rx_Buf[1]>>3);
			printf(LPC_UART0,"\x1b[01;01HTemp Val    : %x04\n\r",(uint16_t)temp_val);
		}

		if((0x800&temp_val)&&(res==TMP102_12B))                    /* If negative?            */
		{
			temp_val = ((~temp_val)&0xFFF) + 1;           /* 2s compliment the value */
			cal = (temp_val*625)/10000;         /* Convert in deg C        */
			dec = ((temp_val*625)/100)%100;     /* get decimal point value */
			printf(LPC_UART0,"\x1b[02;01HTemp in degC: -%d03.%d02",(uint16)cal,(uint16)dec);
		}
		else if((0x1000&temp_val)&&(res==TMP102_13B))                    /* If negative?            */
		{
			temp_val = ((~temp_val)&0x1FFF) + 1;           /* 2s compliment the value */
			cal = (temp_val*625)/10000;         /* Convert in deg C        */
			dec = ((temp_val*625)/100)%100;     /* get decimal point value */
			printf(LPC_UART0,"\x1b[02;01HTemp in degC: -%d03.%d02",(uint16)cal,(uint16)dec);
		}
		else
		{
			cal = (temp_val*625)/10000;         /* Convert in deg C        */
			dec = ((temp_val*625)/100)%100;     /* get decimal point value */
			printf(LPC_UART0,"\x1b[02;01HTemp in degC: %d03.%d02",(uint16)cal,(uint16)dec);
		}

		return (0);
	}
	else
	{
		return (-1);
	}
}


/**
 * @}
 */

/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

