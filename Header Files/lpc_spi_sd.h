/**********************************************************************
* $Id$		lpc_spi_sd.h
*//**
* @file		lpc_spi_sd.h
* @brief	Contains all Spi based SD Initialization code
* @version	1.0
* @date		10. Dec. 2013
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
/** @defgroup SD
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_SPI_SD_H_
#define LPC_SPI_SD_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_system_init.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Public Macros -------------------------------------------------------------- */
/** @defgroup SD_Public_Macros
 * @{
 */

typedef enum _sd_connect_status
{
	SD_CONNECTED,
	SD_DISCONNECTED
}sd_connect_status;

typedef enum _sd_error
{
	SD_OK,
	SD_NG,
	SD_CMD_BAD_PARAMETER,
	SD_ERROR_TOKEN,
	SD_ERROR_TIMEOUT,
	SD_ERROR_BUS_NOT_IDLE,
	SD_ERROR_CMD0,
	SD_ERROR_CMD55,
	SD_ERROR_ACMD41,
	SD_ERROR_CMD59
}sd_error;

//SD command code
#define 	CMD0_GO_IDLE_STATE            0x00
#define		CMD1_SEND_OPCOND              0x01
#define 	CMD9_SEND_CSD                 0x09
#define 	CMD10_SEND_CID                0x0a
#define  	CMD12_STOP_TRANSMISSION       0x0b
#define 	CMD13_SEND_STATUS             0x0c
#define 	CMD16_SET_BLOCKLEN            0x10
#define 	CMD17_READ_SINGLE_BLOCK       0x11
#define 	CMD18_READ_MULTIPLE_BLOCK     0x12
#define 	CMD24_WRITE_BLOCK             0x18
#define 	CMD25_WRITE_MULTIPLE_BLOCK    0x19
#define 	CMD27_PROGRAM_CSD             0x1b
#define 	CMD28_SET_WRITE_PROT          0x1c
#define 	CMD29_CLR_WRITE_PROT          0x1d
#define 	CMD30_SEND_WRITE_PROT         0x1e
#define 	CMD32_ERASE_WR_BLK_START_ADDR 0x20
#define 	CMD33_ERASE_WR_BLK_END_ADDR   0x21
#define 	CMD38_ERASE                   0x26
#define 	CMD55_APP_CMD                 0x37
#define 	CMD56_GEN_CMD                 0x38
#define 	CMD58_READ_OCR                0x3a
#define 	CMD59_CRC_ON_OFF              0x3b
/* Application-specific commands (always prefixed with CMD55_APP_CMD) */
#define	 	ACMD13_SD_STATUS              0x0d
#define 	ACMD22_SEND_NUM_WR_BLOCKS     0x16
#define 	ACMD23_SET_WR_BLK_ERASE_COUNT 0x17
#define 	ACMD41_SEND_OP_COND           0x29
#define 	ACMD42_SET_CLR_CARD_DETECT    0x2a
#define 	ACMD51_SEND_SCR               0x33
/* R1 format responses (ORed together as a bit-field) */
#define 	R1_NOERROR   				  0x00
#define 	R1_IDLE      				  0x01
#define 	R1_ERASE     				  0x02
#define 	R1_ILLEGAL   				  0x04
#define 	R1_CRC_ERR   				  0x08
#define 	R1_ERASE_SEQ 				  0x10
#define 	R1_ADDR_ERR  				  0x20
#define 	R1_PARAM_ERR 				  0x40
/* R2 format responses - second byte only, first is identical to R1 */
#define 	R2_LOCKED      				  0x01
#define 	R2_WP_FAILED   				  0x02
#define 	R2_ERROR       				  0x04
#define 	R2_CTRL_ERR    				  0x08
#define 	R2_ECC_FAIL    				  0x10
#define 	R2_WP_VIOL     				  0x20
#define 	R2_ERASE_PARAM 				  0x40
#define 	R2_RANGE_ERR   				  0x80

#define GETBIT(in, bit) ((in & (1<<bit)) >> bit)
#define SD_CMD_BLOCK_LENGTH		6
#define SD_DATA_BLOCK_LENGTH	515
#define SD_WAIT_R1_TIMEOUT		100000


uint8_t sd_cmd_buf[SD_CMD_BLOCK_LENGTH];
uint8_t sd_data_buf[SD_DATA_BLOCK_LENGTH];

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup SD_Public_Functions SD Public Functions
 * @{
 */

sd_connect_status SD_GetCardConnectStatus (void);
uint8_t CRC_7 (uint8_t old_crc, uint8_t data);
uint8_t CRC_7Final (uint8_t old_crc);
uint32_t SD_SendReceiveData_Polling (void* tx_buf, void* rx_buf, uint32_t length);
void SD_SendCommand(uint8_t cmd, uint8_t *arg);
sd_error SD_WaitR1 (uint8_t *buffer, uint32_t length, uint32_t timeout);
sd_error SD_WaitDeviceIdle (uint32_t num_char);
sd_error SD_Init (uint8_t retries);
sd_error SD_GetCID (void);
void SD_ErrorMsg (sd_error sd_status);




/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_SPI_SD_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
