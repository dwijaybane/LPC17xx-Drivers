/**********************************************************************
* $Id$		lpc_types.h
*//**
* @file		lpc_types.h
* @brief	Contains the NXP ABL typedefs for C standard types.
*     		It is intended to be used in ISO C conforming development
*     		environments and checks for this insofar as it is possible
*     		to do so.
* @version	1.0
* @date		24. July. 2013
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

/* Type group ----------------------------------------------------------- */
/** @defgroup LPC_Types LPC_Types
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_TYPES_H
#define LPC_TYPES_H

/* Includes ------------------------------------------------------------------- */
#include <stdint.h>

/************************** DEBUG MODE DEFINITIONS *********************************/
/* Un-comment the line below to compile the library in DEBUG mode, this will expanse
   the "CHECK_PARAM" macro in the FW library code */

//#define DEBUG

/************************** GLOBAL/PUBLIC MACRO DEFINITIONS *********************************/

#ifdef  DEBUG
/*******************************************************************************
* @brief		The CHECK_PARAM macro is used for function's parameters check.
* 				It is used only if the library is compiled in DEBUG mode.
* @param[in]	expr - If expr is false, it calls check_failed() function
*                    	which reports the name of the source file and the source
*                    	line number of the call that failed.
*                    - If expr is true, it returns no value.
* @return		None
*******************************************************************************/
#define CHECK_PARAM(expr) ((expr) ? (void)0 : check_failed((uint8_t *)__FILE__, __LINE__))
#else
#define CHECK_PARAM(expr)
#endif /* DEBUG */

/************************** GLOBAL/PUBLIC FUNCTION DECLARATION *********************************/
#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line);
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup LPC_Types_Public_Types LPC_Types Public Types
 * @{
 */

/**
 * @brief Boolean Type definition
 */
typedef enum {FALSE = 0, TRUE = !FALSE} Bool;

/**
 * @brief Flag Status and Interrupt Flag Status type definition
 */
typedef enum {RESET = 0, SET = !RESET} FlagStatus, IntStatus, SetState;
#define PARAM_SETSTATE(State) ((State==RESET) || (State==SET))

/**
 * @brief Functional State Definition
 */
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define PARAM_FUNCTIONALSTATE(State) ((State==DISABLE) || (State==ENABLE))

/**
 * @ Status type definition
 */
typedef enum {ERROR = 0, SUCCESS = !ERROR} Status;


/**
 * Read/Write transfer type mode (Block or non-block)
 */
typedef enum
{
	NONE_BLOCKING = 0,		/**< None Blocking type */
	TIME_BLOCKING,			/**< Time Blocking type*/
	BLOCKING				/**< Blocking type */
} TRANSFER_BLOCK_Type;


/** Pointer to Function returning Void (any number of parameters) */
typedef void (*PFV)();

/** Pointer to Function returning int32_t (any number of parameters) */
typedef int32_t(*PFI)();

/**
 * @}
 */


/* Public Macros -------------------------------------------------------------- */
/** @defgroup LPC_Types_Public_Macros  LPC_Types Public Macros
 * @{
 */

/* _BIT(n) sets the bit at position "n"
 * _BIT(n) is intended to be used in "OR" and "AND" expressions:
 * e.g., "(_BIT(3) | _BIT(7))".
 */
#undef _BIT
/* Set bit macro */
#define _BIT(n)	(1<<n)

/* _SBF(f,v) sets the bit field starting at position "f" to value "v".
 * _SBF(f,v) is intended to be used in "OR" and "AND" expressions:
 * e.g., "((_SBF(5,7) | _SBF(12,0xF)) & 0xFFFF)"
 */
#undef _SBF
/* Set bit field macro */
#define _SBF(f,v) (v<<f)

/* _BITMASK constructs a symbol with 'field_width' least significant
 * bits set.
 * e.g., _BITMASK(5) constructs '0x1F', _BITMASK(16) == 0xFFFF
 * The symbol is intended to be used to limit the bit field width
 * thusly:
 * <a_register> = (any_expression) & _BITMASK(x), where 0 < x <= 32.
 * If "any_expression" results in a value that is larger than can be
 * contained in 'x' bits, the bits above 'x - 1' are masked off.  When
 * used with the _SBF example above, the example would be written:
 * a_reg = ((_SBF(5,7) | _SBF(12,0xF)) & _BITMASK(16))
 * This ensures that the value written to a_reg is no wider than
 * 16 bits, and makes the code easier to read and understand.
 */
#undef _BITMASK
/* Bitmask creation macro */
#define _BITMASK(field_width) ( _BIT(field_width) - 1)

/* NULL pointer */
#ifndef NULL
#define NULL ((void*) 0)
#endif

/* FALSE pointer */
#ifndef FALSE
#define FALSE   (0)
#endif

/* TRUE pointer */
#ifndef TRUE
#define TRUE    (1)
#endif

/* YES pointer */
#ifndef YES
#define YES     (1)
#endif

/* NO pointer */
#ifndef NO
#define NO      (0)
#endif

/* ON pointer */
#ifndef ON
#define ON      (1)
#endif

/* OFF pointer */
#ifndef OFF
#define OFF     (0)
#endif

/* TOP pointer */
#ifndef TOP_LINE
#define TOP_LINE    (0)
#endif

/* BOTTOM pointer */
#ifndef BOTTOM_LINE
#define BOTTOM_LINE (1)
#endif

/* FORWARD pointer */
#ifndef FORWARD
#define FORWARD     (1)
#endif

/* REVERSE pointer */
#ifndef REVERSE
#define REVERSE     (0)
#endif

/* CAPITALISE MACRO */
#ifndef	to_upper
#define	to_upper(c)		('a' <= (c) && (c) <= 'z' ? (c)-32 : (c))
#endif

/* Number of elements in an array */
#define NELEMENTS(array)  (sizeof (array) / sizeof (array[0]))

/* Static data/function define */
#define STATIC static
/* External data/function define */
#define EXTERN extern

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/******************************************************************************/
/*                         Bit Banding declarations                           */
/******************************************************************************/
/* The processor memory map includes two bit-band regions. These occupy the lowest
 * 1MB of the SRAM and peripheral memory regions respectively.
 * + SRAM: 	Bit-band region: 	0x20000000 - 0x20100000
 * 			Bit-band alias:		0x22000000 - 0x23FFFFFF
 * + PERI:	Bit-band region:	0x40000000 - 0x40100000
 * 			Bit-band alias:		0x42000000 - 0x43FFFFFF
 * The mapping formula:
 * 		bit_word_offset = (byte_offset * 32) + (bit_number * 4)
 * 		bit_word_address = bit_band_base + bit_word_offset
 * where:
 * 	+ bit_word_offset: the position of the target bit in the bit-band memory region
 * 	+ bit_word_addr: the address of the word in the alias memory region that maps to the target bit
 *  + bit_band_base: the starting address of the alias region
 *  + byte_offset: the number of byte in the bit-band region that contains the targeted bit
 *  + bit_number: is the bit position (0-7) of the targeted bit
 *
 * Note: The fact, the SRAM on LPC1768 just available in two ranges:
 * 	+ 0x2007C000 - 0x2007FFFF: for SRAM - bank 0
 *  + 0x20080000 - 0x20083FFF: for SRAM - bank 1
 *  (And one range for GPIO peripheral but assigned in SRAM ranges: 0x2009C000 - 0x2009FFFF)
 */

/* Bit band SRAM definitions */
#define BITBAND_SRAM_REF   0x20000000
#define BITBAND_SRAM_BASE  0x22000000

#define BITBAND_SRAM(a,b) ((BITBAND_SRAM_BASE + ((a-BITBAND_SRAM_REF)<<5) + (b<<2)))  // Convert SRAM address

/* Bit band PERIPHERAL definitions */
#define BITBAND_PERI_REF   0x40000000
#define BITBAND_PERI_BASE  0x42000000

#define BITBAND_PERI(a,b) ((BITBAND_PERI_BASE + ((a-BITBAND_PERI_REF)<<5) + (b<<2)))  // Convert PERI address

/* Basic bit band function definitions */
#define BITBAND_SRAM_ClearBit(a,b)	(*(volatile uint32_t *) (BITBAND_SRAM(a,b)) = 0)
#define BITBAND_SRAM_SetBit(a,b)	(*(volatile uint32_t *) (BITBAND_SRAM(a,b)) = 1)
#define BITBAND_SRAM_GetBit(a,b)	(*(volatile uint32_t *) (BITBAND_SRAM(a,b)))

#define BITBAND_PERI_ClearBit(a,b)	(*(volatile uint32_t *) (BITBAND_PERI(a,b)) = 0)
#define BITBAND_PERI_SetBit(a,b)	(*(volatile uint32_t *) (BITBAND_PERI(a,b)) = 1)
#define BITBAND_PERI_GetBit(a,b)	(*(volatile uint32_t *) (BITBAND_PERI(a,b)))

/* Variable address in SRAM
 * should be in one of two range:
 * 	+ 0x2007C000 - 0x20083FFF: for SRAM
 *  + 0x20080000 - 0x20083FFF: for GPIO
 */
//#define VAR_ADDRESS		0x2007C000
//#define VAR_BIT			3 //Bit 3

/* Peripheral address
 * should be in range: 0x40000000 - 0x40100000
 */
//#define PERI_ADDRESS	0x40020000 // SPI Control Register (S0SPCR)
//#define PERI_BIT		5 //bit 5 - Master mode select
//or instead of PERI_ADDRESS use LPC_SPI_BASE


/**
 * @}
 */


/* Old Type Definition compatibility ------------------------------------------ */
/** @addtogroup LPC_Types_Public_Types LPC_Types Public Types
 * @{
 */

/** SMA type for character type */
typedef char CHAR;

/** SMA type for 8 bit unsigned value */
typedef uint8_t uchar;
typedef uint8_t UNS_8;

/** SMA type for 8 bit signed value */
typedef int8_t schar;
typedef int8_t INT_8;

/** SMA type for 16 bit unsigned value */
typedef uint16_t uint16;
typedef	uint16_t UNS_16;

/** SMA type for 16 bit signed value */
typedef int16_t int16;
typedef	int16_t INT_16;

/** SMA type for 32 bit unsigned value */
typedef uint32_t ulong32;
typedef	uint32_t UNS_32;

/** SMA type for 32 bit signed value */
typedef int32_t long32;
typedef	int32_t INT_32;

/** SMA type for 64 bit signed value */
typedef int64_t double64;
typedef int64_t INT_64;

/** SMA type for 64 bit unsigned value */
typedef uint64_t udouble64;
typedef uint64_t UNS_64;

/** 32 bit boolean type */
typedef Bool BOOL_32;

/** 16 bit boolean type */
typedef Bool BOOL_16;

/** 8 bit boolean type */
typedef Bool BOOL_8;

/**
 * @}
 */

#endif /* LPC_TYPES_H */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
