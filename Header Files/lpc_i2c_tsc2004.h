/**********************************************************************
* $Id$		lpc_i2c_tsc2004.h
*//**
* @file		lpc_i2c_tsc2004.h
* @brief	Contains all I2C based TouchScreen Initialization code
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
/** @defgroup TSC2004
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_I2C_TSC2004_H_
#define LPC_I2C_TSC2004_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_system_init.h"

#ifdef __cplusplus
extern "C"
{
#endif


/* Public Macros -------------------------------------------------------------- */
/** @defgroup TSC2004_Public_Macros
 * @{
 */
#define  TSC2004_ID    (0x90>>1)

/* Control byte 0 (Non-Conversion read/write based configuration) */
#define TSC2004_CMD0(addr, pnd, rw) ((addr<<3)|(pnd<<1)|rw)
/* Control byte 1 (Conversion related configuration) */
#define TSC2004_CMD1(cmd, mode, rst) ((1<<7)|(cmd<<4)|(mode<<2)|(rst<<1))

/* Command Bits */
#define READ_REG 1
#define WRITE_REG 0
#define SWRST_TRUE 1
#define SWRST_FALSE 0
#define PND0_TRUE 1
#define PND0_FALSE 0


/* Configuration register bit fields */
/* CFR0 */
#define PEN_STS_CTRL_MODE (1<<15)
#define ADC_STS (1<<14)
#define RES_CTRL (1<<13)
#define ADC_CLK_4MHZ (0<<11)
#define ADC_CLK_2MHZ (1<<11)
#define ADC_CLK_1MHZ (2<<11)
#define PANEL_VLTG_STB_TIME_0US (0<<8)
#define PANEL_VLTG_STB_TIME_100US (1<<8)
#define PANEL_VLTG_STB_TIME_500US (2<<8)
#define PANEL_VLTG_STB_TIME_1MS (3<<8)
#define PANEL_VLTG_STB_TIME_5MS (4<<8)
#define PANEL_VLTG_STB_TIME_10MS (5<<8)
#define PANEL_VLTG_STB_TIME_50MS (6<<8)
#define PANEL_VLTG_STB_TIME_100MS (7<<8)

/* CFR2 */
#define PINTS1 (1<<15)
#define PINTS0 (1<<14)
#define MEDIAN_VAL_FLTR_SIZE_1 (0<<12)
#define MEDIAN_VAL_FLTR_SIZE_3 (1<<12)
#define MEDIAN_VAL_FLTR_SIZE_7 (2<<12)
#define MEDIAN_VAL_FLTR_SIZE_15 (3<<12)
#define AVRG_VAL_FLTR_SIZE_1 (0<<10)
#define AVRG_VAL_FLTR_SIZE_3_4 (1<<10)
#define AVRG_VAL_FLTR_SIZE_7_8 (2<<10)
#define AVRG_VAL_FLTR_SIZE_16 (3<<10)
#define MAV_FLTR_EN_X (1<<4)
#define MAV_FLTR_EN_Y (1<<3)
#define MAV_FLTR_EN_Z (1<<2)
#define MAX_12BIT ((1 << 12) - 1)
#define MEAS_MASK 0xFFF


/**
 * @}
 */

/* Public Types --------------------------------------------------------------- */
/** @defgroup SSP_Public_Types SSP Public Types
 * @{
 */

/* Converter function mapping */
typedef enum {
	MEAS_X_Y_Z1_Z2=0, /* Measure X,Y,z1 and Z2: 0x0 */
	MEAS_X_Y, /* Measure X and Y only: 0x1 */
	MEAS_X, /* Measure X only: 0x2 */
	MEAS_Y, /* Measure Y only: 0x3 */
	MEAS_Z1_Z2, /* Measure Z1 and Z2 only: 0x4 */
	MEAS_AUX, /* Measure Auxillary input: 0x5 */
	MEAS_TEMP1, /* Measure Temparature1: 0x6 */
	MEAS_TEMP2, /* Measure Temparature2: 0x7 */
	MEAS_AUX_CONT, /* Continuously measure Auxillary input: 0x8 */
	X_DRV_TEST, /* X-Axis drivers tested 0x9 */
	Y_DRV_TEST, /* Y-Axis drivers tested 0xA */
	/*Command Reserved*/
	SHORT_CKT_TST = 0xC, /* Short circuit test: 0xC */
	XP_XN_DRV_STAT, /* X+,Y- drivers status: 0xD */
	YP_YN_DRV_STAT, /* X+,Y- drivers status: 0xE */
	YP_XN_DRV_STAT /* Y+,X- drivers status: 0xF */
}convertor_function;

/* Register address mapping */
typedef enum {
	X_REG=0, /* X register: 0x0 */
	Y_REG, /* Y register: 0x1 */
	Z1_REG, /* Z1 register: 0x2 */
	Z2_REG, /* Z2 register: 0x3 */
	AUX_REG, /* AUX register: 0x4 */
	TEMP1_REG, /* Temp1 register: 0x5 */
	TEMP2_REG, /* Temp2 register: 0x6 */
	STAT_REG, /* Status Register: 0x7 */
	AUX_HGH_TH_REG, /* AUX high threshold register: 0x8 */
	AUX_LOW_TH_REG, /* AUX low threshold register: 0x9 */
	TMP_HGH_TH_REG, /* Temp high threshold register:0xA */
	TMP_LOW_TH_REG, /* Temp low threshold register: 0xB */
	CFR0_REG, /* Configuration register 0: 0xC */
	CFR1_REG, /* Configuration register 1: 0xD */
	CFR2_REG, /* Configuration register 2: 0xE */
	CONV_FN_SEL_STAT /* Convertor function select register: 0xF */
}register_address;

/* Supported Resolution modes */
typedef enum {
	MODE_10BIT=0, /* 10 bit resolution */
	MODE_12BIT /* 12 bit resolution */
}resolution_mode;


typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t z1;
	uint16_t z2;
}ts_event;


/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup TSC2004_Public_Functions TSC2004 Public Functions
 * @{
 */
void TSC2004_Init (void);

char I2C_TSC2004_Write_Byte (uint8_t Command);
char I2C_TSC2004_Write_Word (uint8_t Command, uint16_t word_data);
uint16_t I2C_TSC2004_Read_Word (uint8_t Command);

uint16_t TSC2004_Read_Reg (register_address reg);
void TSC2004_Read_Values (ts_event *tc);

void TSC2004_Read_Value_Test (void);
void TSC2004_Draw_Test (void);
void TSC2004_Slide_Test (void);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_I2C_TSC2004_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
