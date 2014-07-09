/*******************************************************************//**
* @file		lpc_i2c_m24256.h
* @brief	Contains all I2C based E2PROM M24256-BWMN6P Initialization code
* @version	1.0
* @date		11. Dec. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup EEPROM
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_I2C_M24256_H_
#define LPC_I2C_M24256_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_system_init.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Public Macros -------------------------------------------------------------- */
/** @defgroup EEPROM_Public_Macros
 * @{
 */
#define  E2PM24256_ID    (0xAE>>1)


/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup EEPROM_Public_Functions EEPROM Public Functions
 * @{
 */

char I2C_IEeprom_Write_Byte (uint16_t eep_address, uint8_t byte_data);
uint8_t I2C_IEeprom_Read_Byte (uint16_t eep_address);
char I2C_IEeprom_Write (uint16_t eep_address, uint8_t* byte_data, uint16_t length);
char I2C_IEeprom_Read (uint16_t eep_address, uint8_t* buf_data, uint16_t length);
void Display_IEeprom_Array (uint8_t *string, uint16_t length);
void Display_IEeprom_Loc (uint16_t mem_start_address, uint16_t mem_end_address);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_I2C_M24256_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
