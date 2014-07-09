/******************************************************************//**
* @file		lpc_i2c_at24c16.h
* @brief	Contains all I2C based E2PROM AT24C16 Initialization code
* @version	1.0
* @date		11. Dec. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup EEPROM
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_I2C_AT24C16_H_
#define LPC_I2C_AT24C16_H_

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
#define  E2P24C16_ID    (0xA0>>1)


/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup EEPROM_Public_Functions EEPROM Public Functions
 * @{
 */

char I2C_Eeprom_Write_Byte (uint16 eep_address, uint8_t byte_data);
uint8_t I2C_Eeprom_Read_Byte (uint16_t eep_address);
char I2C_Eeprom_Write (uint16_t eep_address, uint8_t* byte_data, uint16_t length);
char I2C_Eeprom_Read (uint16_t eep_address, uint8_t* buf_data, uint16_t length);
void Display_Eeprom_Array (uint8_t *string, uint16_t length);
void Display_Eeprom_Loc (uint16 mem_start_address, uint16 mem_end_address);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_I2C_AT24C16_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
