/**********************************************************************
* $Id$		lpc_i2c_tsc2004.c
*//**
* @file		lpc_i2c_tsc2004.c
* @brief	Contains all functions support for I2C based TouchScreen
*           library on LPC17xx
* @version	1.0
* @date		27. Dec. 2013
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
/** @addtogroup TSC2004
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_i2c_tsc2004.h"

/* GLCD Include-----------------------------------------------------------------*/
//#include "mario.h"
//#include "cham.h"
//#include "ubuntu.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */



/** @addtogroup TSC2004_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */

/*********************************************************************//**
 * @brief	    Touch Screen Initialize
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TSC2004_Init (void)
{
	uint16_t cmd,data;

	 /* Reset the TSC, configure for 12 bit */
	cmd = TSC2004_CMD1(MEAS_X_Y_Z1_Z2, MODE_12BIT, SWRST_TRUE);
	I2C_TSC2004_Write_Byte(cmd);

	/* Enable interrupt for PENIRQ and DAV */
	cmd = TSC2004_CMD0(CFR2_REG, PND0_FALSE, WRITE_REG);
	data = MEDIAN_VAL_FLTR_SIZE_1 |AVRG_VAL_FLTR_SIZE_7_8;
	I2C_TSC2004_Write_Word(cmd, data);

	/* Configure the TSC in TSMode 1 */
	cmd = TSC2004_CMD0(CFR0_REG, PND0_FALSE, WRITE_REG);
	data = PEN_STS_CTRL_MODE | ADC_CLK_2MHZ;
	I2C_TSC2004_Write_Word(cmd, data);

	/* Enable x, y, z1 and z2 conversion functions */
	cmd = TSC2004_CMD1(MEAS_X_Y_Z1_Z2, MODE_12BIT, SWRST_FALSE);
	I2C_TSC2004_Write_Byte(cmd);
}


/*********************************************************************//**
 * @brief	    Writes Command Byte
 * @param[in]	Command  Write Command Byte
 * @return 		status
 **********************************************************************/
char I2C_TSC2004_Write_Byte (uint8_t Command)
{
	/* Transmit setup */
	I2C_M_SETUP_Type txsetup;

	txsetup.sl_addr7bit = TSC2004_ID;
	txsetup.tx_data = &Command;
	txsetup.tx_length = 1;
	txsetup.rx_data = NULL;
	txsetup.rx_length = 0;
	txsetup.retransmissions_max = 3;

	/* write byte to addr  */
	if(I2C_MasterTransferData(LPC_I2C0, &txsetup, I2C_TRANSFER_POLLING)==SUCCESS) //return status
	{
		return (0);
	}
	else
	{
		return (-1);
	}
}


/*********************************************************************//**
 * @brief	    Writes Word Command
 * @param[in]	Command      Configuration Commands
 * @param[in]   word_data    Configuration values
 * @return 		status
 **********************************************************************/
char I2C_TSC2004_Write_Word (uint8_t Command, uint16_t word_data)
{
	/* Transmit setup */
	I2C_M_SETUP_Type txsetup;

	I2C_Tx_Buf[0] = Command;
	I2C_Tx_Buf[1] = (uint8_t)(word_data>>8);
	I2C_Tx_Buf[2] = (uint8_t)(word_data);

	txsetup.sl_addr7bit = TSC2004_ID;
	txsetup.tx_data = I2C_Tx_Buf;
	txsetup.tx_length = 3;
	txsetup.rx_data = NULL;
	txsetup.rx_length = 0;
	txsetup.retransmissions_max = 3;

	/* write byte to addr  */
	if(I2C_MasterTransferData(LPC_I2C0, &txsetup, I2C_TRANSFER_POLLING)==SUCCESS) //return status
	{
		return (0);
	}
	else
	{
		return (-1);
	}
}


/*********************************************************************//**
 * @brief	    Reads Word data
 * @param[in]	Command    Read Register
 * @return 		Word value
 **********************************************************************/
uint16_t I2C_TSC2004_Read_Word (uint8_t Command)
{
	/* Receive setup */
	I2C_M_SETUP_Type rxsetup;
	uint16_t word_data=0;

	rxsetup.sl_addr7bit = TSC2004_ID;
	rxsetup.tx_data = &Command;	// Get address to read at writing address
	rxsetup.tx_length = 1;
	rxsetup.rx_data = I2C_Rx_Buf;
	rxsetup.rx_length = 2;
	rxsetup.retransmissions_max = 3;

	/* The protocol and raw data format from i2c interface:
	 * * S Addr Wr [A] Comm [A] S Addr Rd [A] [DataHigh] A [DataLow] NA P
	 * * Data are in Right Justified format.
	 * */
	word_data |= (I2C_Rx_Buf[0]&0x0F)<<8;
	word_data |= (I2C_Rx_Buf[1]&0xFF);

	if (I2C_MasterTransferData(LPC_I2C0, &rxsetup, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		return (word_data);
	}
	else
	{
		return (-1);
	}
}


/*********************************************************************//**
 * @brief	    Reads Any Register value
 * @param[in]	reg    Register to access
 * @return 		Word Value
 **********************************************************************/
uint16_t TSC2004_Read_Reg (register_address reg)
{
	uint16_t val;
	uint8_t cmd;

	TSC2004_Init ();					// Initialize Touch Screen

	 // Prepare val Measurement
	cmd = TSC2004_CMD0(reg, PND0_FALSE, READ_REG);
	val = I2C_TSC2004_Read_Word(cmd);

	 // Read val Measurement
	cmd = TSC2004_CMD0(reg, PND0_FALSE, READ_REG);
	val = I2C_TSC2004_Read_Word(cmd);

	val &= MEAS_MASK;

	return (val);
}


/*********************************************************************//**
 * @brief	    Read X,Y,Z1,Z2 Values
 * @param[in]	*tc    store values in structure
 * @return 		None
 **********************************************************************/
void TSC2004_Read_Values (ts_event *tc)
{
	uint8_t cmd;

	TSC2004_Init ();					// Initialize Touch Screen

	 // Prepare X Measurement
	cmd = TSC2004_CMD0(X_REG, PND0_FALSE, READ_REG);
	tc->x = I2C_TSC2004_Read_Word(cmd);

	 // Read X Measurement
	cmd = TSC2004_CMD0(X_REG, PND0_FALSE, READ_REG);
	tc->x = I2C_TSC2004_Read_Word(cmd);

	// Prepare Y Measurement
	cmd = TSC2004_CMD0(Y_REG, PND0_FALSE, READ_REG);
	tc->y = I2C_TSC2004_Read_Word(cmd);

	// Read Y Measurement
	cmd = TSC2004_CMD0(Y_REG, PND0_FALSE, READ_REG);
	tc->y = I2C_TSC2004_Read_Word(cmd);

	// Prepare Z1 Measurement
	cmd = TSC2004_CMD0(Z1_REG, PND0_FALSE, READ_REG);
	tc->z1 = I2C_TSC2004_Read_Word(cmd);

	// Read Z1 Measurement
	cmd = TSC2004_CMD0(Z1_REG, PND0_FALSE, READ_REG);
	tc->z1 = I2C_TSC2004_Read_Word(cmd);

	// Prepare Z2 Measurement
	cmd = TSC2004_CMD0(Z2_REG, PND0_FALSE, READ_REG);
	tc->z2 = I2C_TSC2004_Read_Word(cmd);

	// Read Z2 Measurement
	cmd = TSC2004_CMD0(Z2_REG, PND0_FALSE, READ_REG);
	tc->z2 = I2C_TSC2004_Read_Word(cmd);

	tc->x &= MEAS_MASK;
	tc->y &= MEAS_MASK;
	tc->z1 &= MEAS_MASK;
	tc->z2 &= MEAS_MASK;
}


/*********************************************************************//**
 * @brief	    Read X,Y,Z1,Z2 Values and Display on Terminal
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TSC2004_Read_Value_Test (void)
{
	ts_event ts;
	TSC2004_Read_Values (&ts);

	printf(LPC_UART0,"\x1b[1;1HMeasure X    = %d05",ts.x);
	printf(LPC_UART0,"\x1b[2;1HMeasure Y    = %d05",ts.y);
	printf(LPC_UART0,"\x1b[3;1HMeasure Z1   = %d05",ts.z1);
	printf(LPC_UART0,"\x1b[4;1HMeasure Z2   = %d05",ts.z2);
}


/*********************************************************************//**
 * @brief	    Read Touchscreen and PutPixel accordingly
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TSC2004_Draw_Test (void)
{
	ts_event ts;

	TSC2004_Read_Values (&ts);
	GLCD_PutPixel (((ts.x)/11)-24,((ts.y)/13)-36,Black);
}


/*********************************************************************//**
 * @brief	    Photo Album with sliding switch of images (Comment incase
 *              not used)
 * @param[in]	None
 * @return 		None
 **********************************************************************/
/*void TSC2004_Slide_Test (void)
{
	uint16_t val,past=0,count=0;
	int8_t pic=0;
	Bool flag=0,fb=0;

	GLCD_Bitmap (0,0,320,240,image);
    while(1)
    {
    	val = TSC2004_Read_Reg(X_REG);
   // 	printf(LPC_UART0,"\x1b[1;1HMeasured Value:  %d05",val);
    	// Slide
    	if ((past>val) && flag)
    	{
    		if (fb)
    		{
    			count = 0;
    		}
    		count++;
    		fb = DISABLE;
    	}
    	else if ((past<val)&& flag)
    	{
    		if (!fb)
    		{
    			count = 0;
    		}
    		count++;
    		fb = ENABLE;
    	}
    	else
    	{
    		past = val;
    		flag = ENABLE;
    	}
    	past = val;

    	if (count == 10)
    	{
    		if (fb)
    		{
    //			printf(LPC_UART0,"\x1b[2;1HForward Slide ");
    			pic++;
    			if (pic > 2)
    			{
    				pic = 0;
    			}
    		}
    		else
    		{
   // 			printf(LPC_UART0,"\x1b[2;1HBackward Slide");
    			pic--;
    			if (pic < 0)
    			{
    				pic = 2;
    			}
    		}

        	switch (pic)
        	{
        	case 1:
   //     		printf(LPC_UART0,"\x1b[3;1HPIC2");
        		GLCD_Bitmap (0,0,320,240,cham);
        		break;

        	case 2:
  //      		printf(LPC_UART0,"\x1b[3;1HPIC3");
        		GLCD_Bitmap (0,0,320,240,ubuntu);
        		break;

        	default:
    //    		printf(LPC_UART0,"\x1b[3;1HPIC1");
        		GLCD_Bitmap (0,0,320,240,image);
        		break;
        	}
    	}
    }
}*/

/**
 * @}
 */

/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

