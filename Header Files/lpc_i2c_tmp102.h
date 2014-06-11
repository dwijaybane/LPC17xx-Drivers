/**********************************************************************
* $Id$		lpc_i2c_tmp102.h
*//**
* @file		lpc_i2c_tmp102.h
* @brief	Contains all I2C based TMP102 temperature sensor declarations
* @version	1.0
* @date		16. Mar. 2014
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
/** @defgroup TMP
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_I2C_TMP102_H_
#define LPC_I2C_TMP102_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_system_init.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Public Macros -------------------------------------------------------------- */
/** @defgroup TMP_Public_Macros
 * @{
 */
#define  TMP102_ID    (0x92>>1)

/* Pointer Register Address */
#define TMP_REG   0x00
#define CFG_REG   0x01
#define TLOW_REG  0x02
#define THIGH_REG 0x03

/************************** Configuration Register Default ****************************
 *    BYTE                      1                       2                             *
 *                 D7 D6 D5 D4 D3 D2  D1 D0 | D7  D6  D5 D4 D3 D2 D1 D0               *
 *                 OS R1 R0 F1 F0 POL TM SD   CR1 CR0 AL EM 0  0  0  0                *
 *    Default      0  1  1  0  0  0   0  0    1   0   1  0  0  0  0  0                *
 *************************************************************************************/

/***************** Configuration Register Macros ******************/
/* configuration register byte2 parameters */
#define EXT_MODE          0x10          /* Extended Mode else use DISABLE for normal mode */
#define READ_ALT          0x20          /* Read Only function Alert Bit */
#define CONV_RATE_0_25HZ  0
#define CONV_RATE_1HZ     0x40
#define CONV_RATE_4HZ     0x80          /* default Conversion Rate */
#define CONV_RATE_8Hz     0xC0

/* configuration register byte1 parameters */
#define SD_MODE           0x100         /* Shutdown mode */
#define TM_MODE           0x200         /* Thermostat mode */
#define ALT_POL           0x400         /* Alert Polarity High */
#define FAULT_QUE1        0             /* Consecutive Faults */
#define FAULT_QUE2        0x800
#define FAULT_QUE4        0x1000
#define FAULT_QUE6        0x1800
#define READ_CONV_RES1    0x2000        /* Read only Conversion Resolution bits */
#define READ_CONV_RES2    0x4000
#define OS_CONV           0x8000        /* One Shot Conversion */
#define DEFAULT           0x60A0        /* Default Value */


/**
 * @brief Bit mode type definitions
 */
typedef enum {
	TMP102_12B = 0,		/**< 12 Bit Resolution */
	TMP102_13B			/**< 13 Bit Resolution */
} BIT_Type;


/**
 * @brief Threshold type definitions
 */
typedef enum {
	THIGH_VAL = 0,		/**< High Threshold value */
	TLOW_VAL			/**< Low Threshold value */
} THRES_Type;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup TMP_Public_Functions TMP Public Functions
 * @{
 */

uchar TMP102_Read_Config(void);
uchar TMP102_Set_Config(uint16_t val);
uchar TMP102_Set_Threshold_Value(THRES_Type limit, int16_t deg, BIT_Type res);
uchar TMP102_Read_Threshold_Value(THRES_Type limit, BIT_Type res);
uchar TMP102_Read_Temp(BIT_Type res);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_I2C_TMP102_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
