/**********************************************************************
* $Id$		lpc_spi_25aa160a.h
*//**
* @file		lpc_spi_25aa160a.h
* @brief	Contains all Spi based E2PROM 25aa160a Initialization code
* @version	1.0
* @date		09. Dec. 2013
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
/** @defgroup EEPROM
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_SPI_25AA160A_H_
#define LPC_SPI_25AA160A_H_

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

#define EEP_WRITE  0x02;  /* Write bit of IR     */
#define EEP_READ   0x03;  /* Read bit of IR      */
#define EEP_WREN   0x06;  /* Write Enable Latch  */
#define EEP_WRDI   0x04;  /* Write Disable Latch */
#define EEP_RDSR   0x05;  /* Read Status Reg     */
#define EEP_WRSR   0x01;  /* Write Status Reg    */

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup EEPROM_Public_Functions EEPROM Public Functions
 * @{
 */

uint8_t Spi_Eeprom_Read_Status_Reg (void);
uchar Spi_Eeprom_Write_Status_Reg (uint8_t status_reg);
uchar Spi_Eeprom_Write_Byte (uint16 eep_address, uint8_t byte_data);
uint8_t Spi_Eeprom_Read_Byte (uint16 eep_address);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_SPI_25AA160A_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
