/**********************************************************************
* $Id$		lpc_buzzer.h
*//**
* @file		lpc_buzzer.h
* @brief	Contains all macro definitions and function prototypes
* 			support for Buzzer firmware library on LPC17xx
* @version	1.0
* @date		25. Nov. 2013
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup BUZZER
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef __LPC_BUZZER_H
#define __LPC_BUZZER_H

/* Includes ------------------------------------------------------------------- */
#include "lpc_system_init.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup BUZZER_Public_Macros
 * @{
 */
/*
*****************************************************************************************
                                  BUZZER MELODY
*****************************************************************************************
*/

#define MELODY_LENGTH 95

/* Key at 20kHz interrupt service routine Timer2 */
#define SILENT_NOTE  255   //   00  Hz

/* Middle 4th C 1-line Octave */
#define C4           76    //  262  Hz
#define C_SHARP4     72    //  278  Hz  Db4
#define D4           68    //  294  Hz
#define D_SHARP4     64    //  312  Hz  Eb4
#define E4           61    //  330  Hz
#define F4           57    //  350  Hz
#define F_SHARP4     54    //  370  Hz
#define G4           51    //  392  Hz
#define G_SHARP4     48    //  416  Hz
#define A4           45    //  440  Hz
#define A_SHARP4     43    //  467  Hz
#define B4           41    //  494  Hz

/* C 2-line Octave */
#define C5           38    //  524  Hz
#define C_SHARP5     36    //  555  Hz
#define D5           34    //  588  Hz
#define D_SHARP5     32    //  623  Hz
#define E5           30    //  660  Hz
#define F5           29    //  699  Hz
#define F_SHARP5     27    //  740  Hz
#define G5           26    //  784  Hz
#define G_SHARP5     24    //  831  Hz
#define A5           23    //  880  Hz
#define A_SHARP5     22    //  933  Hz
#define B5           20    //  988  Hz

/* C 3-line Octave */
#define C6           19    // 1047  Hz
#define C_SHARP6     18    // 1109  Hz
#define D6           17    // 1175  Hz
#define D_SHARP6     16    // 1245  Hz
#define E6           15    // 1317  Hz
#define F6           14    // 1397  Hz
#define F_SHARP6     13    // 1480  Hz
#define G6           13    // 1568  Hz
#define G_SHARP6     12    // 1662  Hz
#define A6           11    // 1760  Hz
#define A_SHARP6     11    // 1865  Hz
#define B6           10    // 1976  Hz

/* C 4-line Octave */
#define C7           9     // 2093  Hz
#define D_SHARP7     8     // 2500  Hz
#define F7           7     // 2857  Hz
#define G_SHARP7     6     // 3333  Hz
#define B7           5     // 4000  Hz

#define D_SHARP8     4     // 5000  Hz



/*
 * Variables
 */
/* Buzzer */
extern uint16 timeout;     /* timeout for note */
extern uint16 on_time;
extern uint16 off_time;

extern uint16 on_timer;
extern uint16 off_timer;

extern Bool play_note;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup BUZZER_Public_Functions BUZZER Public Functions
 * @{
 */

void Play_Frequency(uint16_t freq, uint16_t dur);
void Play_Melody(void);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* __LPC_BUZEER_H */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
