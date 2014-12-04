/******************************************************************//**
* @file		lpc17xx_rtc.c
* @brief	Contains all functions support for RTC firmware library on LPC17xx
* @version	1.0
* @date		04. Dec. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/


/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup RTC
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_rtc.h"
#include "lpc17xx_clkpwr.h"


/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

#if 0
/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		RTC interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void RTC_IRQHandler(void)
{
	/* check the Alarm match */
	if (RTC_GetIntPending(LPC_RTC, RTC_INT_ALARM))
	{
		/* Clear pending interrupt */
		RTC_ClearIntPending(LPC_RTC, RTC_INT_ALARM);
	}

}
#endif

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup RTC_Public_Functions
 * @{
 */

 /* RTC Initialization Config function ---------------------------------*/
/********************************************************************//**
 * @brief		Initializes the RTC peripheral according to the specified
 *               parameters.
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void RTC_Config (void)
{
	// Init RTC module
	RTC_Init(LPC_RTC);

	/* Disable RTC interrupt */
	NVIC_DisableIRQ(RTC_IRQn);
	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(RTC_IRQn, 4);

	/* Enable rtc (starts increase the tick counter and second counter register) */
	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE);
	RTC_CalibCounterCmd(LPC_RTC, DISABLE);

	/* Set current time for RTC */
	// Current time is 8:00:00PM, 2012-12-04
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, 20);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, 12);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, 2013);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, 4);

	/* Set ALARM time for second */
//	RTC_SetAlarmTime (LPC_RTC, RTC_TIMETYPE_SECOND, 10);

	/* Setting Timer calibration
	 * Calibration value =  5s;
	 * Direction = Forward calibration
	 * So after each 5s, calibration logic can periodically adjust the time counter by
	 * incrementing the counter by 2 instead of 1
	 */
//	RTC_CalibConfig(LPC_RTC, 5, RTC_CALIB_DIR_FORWARD);
//	RTC_CalibCounterCmd(LPC_RTC, ENABLE);

	/* Set the CIIR for second counter interrupt*/
//	RTC_CntIncrIntConfig (LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
	/* Set the AMR for 10s match alarm interrupt */
//	RTC_AlarmIntConfig (LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);

    /* Enable RTC interrupt */
//    NVIC_EnableIRQ(RTC_IRQn);
}


/********************************************************************//**
 * @brief		Change RTC Time
 * @param[in]	None
 * @return 		status
 *********************************************************************/
uchar Change_Time (void)
{
	char TimeBuffer[3];
	char cp;                        /* input from keyboard */
	char RTCdata;                  /* buffer */
	uchar TimeFlag=0;               /* flag for time data is available */
	uchar count = 0;                /* input character counter */
	uchar State = 0;                /* state for hh=1, mm=2, ss=3 */

	uchar digit = 0;                /* indication for 1st and 2nd digit of hh, mm, ss */
	uchar MaxCount = 6;             /* maximum no. of digits */
	char AsciiLSB;                 /* temporary storage for lsb ascii input */
	char AsciiMSB;                 /* temporary storage for msb ascii input */

	while(1)
	{
		cp = getche(LPC_UART0, BLOCKING);                /* get input character */

		if (cp == In_ESC)           /* if ESCAPE pressed then exit */
		{
			return (cp);
		}
		else if (cp == In_CR)       /* CARRIAGE RETURN ? */
		{
			if(count==0)                /* any characters at all ? */
			{
				continue;                 /* no, so get another character */
			}

			if(TimeFlag==1)
			{                           /* update real time clock  */
			    /* Write changes to RTC */
				RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, TimeBuffer[0]);
				RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, TimeBuffer[1]);
				RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, TimeBuffer[2]);

				return(0);
			}
			else
				break;                      /* check for any character */
		}
		else if (cp == In_DELETE || cp == In_BACKSPACE) /* delete or back space */
		{
			if(count==0)                /* any characters entered */
			{
				continue;                 /* no, so get another character */
			}

			if(digit == 0)
			{
				Erase_SemiColon(LPC_UART0);
				digit = 1;                /* now its point to 2nd digit */
				State--;                  /* point to one up state */
				count--;                  /* decrement character count */
				continue;
			}
			else
			{
				if(TimeFlag == 1)
				{
					Erase_Char_With_UnderScore(LPC_UART0);
					TimeFlag = 0;          /* changed mind so not ready */
					digit = 1;             /* point to 2nd digit */
					count--;               /* decrement count */
					continue;              /* goback and get another input */
				}

				Erase_Char_With_UnderScore(LPC_UART0);

				digit = 0;               /* now its point to 1st digit */
				count--;                 /* decrement character count */
				continue;
			}
		}
		else if (cp>= '0' && cp <='9')/* is character between 0 to 9? */
		{
			RTCdata = cp;
			/*********** check hours **********/
			if(count < MaxCount)
			{
				if(State == 0)
				{
					if(digit==0)
					{
						printf(LPC_UART0, "%c",RTCdata);     /* echo 1st character */
						AsciiMSB = RTCdata;   /* store 1st byte MSB  */
						count++;              /* increment character count,will be 1 */
						digit++;              /* increment digit, will be 1 */
						continue;
					}
					else
					{
						printf(LPC_UART0,"%c", RTCdata);                     /* echo 2nd character */
						AsciiLSB = RTCdata;                   /* store 2nd byte LSB  */
						RTCdata = ((AsciiMSB & 0x0F) * 10);   /* store nibble */
						RTCdata += (AsciiLSB & 0x0F);         /* add to nibble, integer */

						if(RTCdata <=23)                      /* check hour range ( 0-23) */
						{
							TimeBuffer[2] = RTCdata;            /* store hh in time buffer */

							State++;                            /* increment to mm state */
							count++;                            /* increment character count,will be 2 */
							digit = 0;                          /* set digit to zero  */

							printf(LPC_UART0, ":");                       /* and write ':' on the screen */
							continue;                           /* continue for minutes */
						}
						else
						{
							Erase_And_RingTheBell(LPC_UART0);
							continue;
						}
					}
				}

				/********** check minutes ***********/
				if(State == 1)
				{
					if(digit==0)
					{
						printf(LPC_UART0, "%c", RTCdata);               /* echo 1st character */
						AsciiMSB = RTCdata;             /* store 1st byte MSB  */
						count++;                        /* increment character count,will be 3 */
						digit++;                        /* increment digit, will be 1  */
						continue;
					}
					else
					{
						printf(LPC_UART0, "%c", RTCdata);               /* echo 2nd character */
						AsciiLSB = RTCdata;             /* store 2nd byte LSB  */
						RTCdata = ((AsciiMSB & 0x0F) * 10);/* store nibble */
						RTCdata += (AsciiLSB & 0x0F);   /* add to nibble, integer */

						if(RTCdata <=59)                /* check minutes range ( 0-59) */
						{
							TimeBuffer[1] = RTCdata;      /* store mm in time buffer */
							State++;                      /* increment to ss state */
							count++;                      /* increment character count,will be 4 */
							digit = 0;                    /* set digit to zero  */
							printf(LPC_UART0, ":");                 /* and write ':' on the screen */

							continue;                     /* continue for seconds */
						}
						else
						{
							Erase_And_RingTheBell(LPC_UART0);
							continue;
						}
					}
				}

				/*********** check seconds ***************/
				if(State == 2)
				{
					if(digit==0)
					{
						printf(LPC_UART0, "%c", RTCdata);               /* echo 1st character */
						AsciiMSB = RTCdata;             /* store 1st byte MSB  */
						count++;                        /* increment character count,will be 5 */
						digit++;                        /* increment digit no.  */

						continue;
					}
					else
					{
						printf(LPC_UART0, "%c", RTCdata);                    /* echo 2nd character */
						AsciiLSB = RTCdata;                  /* store 2nd byte LSB  */
						RTCdata = ((AsciiMSB & 0x0F) * 10);  /* store nibble */
						RTCdata += (AsciiLSB & 0x0F);        /* add to nibble, integer */

						if(RTCdata <=59)                     /* check seconds range ( 0-59) */
						{
							TimeBuffer[0] = RTCdata;           /* store ss in time buffer */

							TimeFlag = 1;                      /* Time data is ready for RTC */
							count++;                           /* increment character count,will be 6 */

							continue;                          /* go back and find the decision  */
						}
						else
						{
							Erase_And_RingTheBell(LPC_UART0);
							continue;
						}
					}
				}

				continue;                 /* number of char is more than reqd */
				/* so go back to find the decision */
			}
			/*************************************************/
			else
			{
				printf(LPC_UART0, "%c", cp);              /* echo character */
				Erase_Char_With_UnderScore(LPC_UART0);
				printf(LPC_UART0, "\7");            /* ring the bell */

				continue;
			}
		}
	}
	return(0);
}


/********************************************************************//**
 * @brief		Check if year is leap year or not
 * @param[in]	None
 * @return 		status
 *********************************************************************/
BOOL_8 Is_Leap_Year(uchar Year, uchar Century)
{
  /* If year not a multiple of 4 then not a leap year */
  if( (Year % 4) != 0) return NO;

  /* If year a multiple of 4 and non zero then is a leap year */
  if(Year != 0) return YES;

  /* Must check the century is a multiple of 4 also */
  return ((Century % 4) == 0);
}


/********************************************************************//**
 * @brief		Change RTC Date
 * @param[in]	None
 * @return 		status
 *********************************************************************/
uchar Change_Date (void)
{
	uchar DaysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	char DateBuffer[3];
	uint16_t Year[1];
	char cp;                            /* input from keyboard */
	char RTCdata;                       /* buffer */
	uchar DateFlag=0;                    /* flag for date data is available */
	uchar count = 0;                     /* input character counter */
	uchar State = 0;                     /* state for dd=1, mm=2, ccyy=3 */

	uchar digit = 0;                     /* indication for 1st, 2nd, 3rd and 4th digit */
                                       /* for dd, mm, yycc */
	uchar MaxCount = 8;                  /* maximum no. of digits */
	char AsciiLSB;                      /* temp storage for LSB ascii input */
	char AsciiMSB;                      /* temp storage for MSB ascii input */
	uchar DaysThisMonth;                 /* number of days for the given month */

    while(1)
    {
    	cp = getche(LPC_UART0, BLOCKING);                   /* get input character */

    	if(cp == In_ESC)                 /* if ESCAPE pressed then exit */
    	{
    		return (cp);
    	}
    	else if (cp == In_CR)          /* CARRIAGE RETURN ? */
    	{
    		if(count==0)                   /* any characters at all ? */
    		{
    			continue;                    /* no, so get another character */
    		}

    		if(DateFlag==1)
    		{   /*  update real time clock  */
    			/* Read RTC */
    			Year[0] = ((DateBuffer[2] & 0xFF) * 100);
    			Year[0] += (DateBuffer[3] & 0xFF);
    			RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, DateBuffer[0]);
    			RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, DateBuffer[1]);
    			RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, Year[0]);
    			return(0);
    		}
    		else
    		{
    			break;                       /* check for any character */
    		}
    	}
    	else if (cp == In_DELETE || cp == In_BACKSPACE) /* delete or back space */
    	{
    		if(count==0)                   /* any characters entered */
    		{
    			continue;                    /* no, so get another character */
    		}

    		if(State == 2)                 /* State 2 cointains 4 digits */
    		{
    			if(digit == 0)
    			{
    				Erase_BackLash(LPC_UART0);
    			    digit = 1;                 /* now its point to 2nd digit of month */
    				State--;                   /* point to month's state */
    				count--;                   /* decrement character count */

    				continue;
    			}
    			else
    			{
    				Erase_Char_With_UnderScore(LPC_UART0);
    				digit--;                     /* point to 2nd digit */
    				count--;                     /* decrement count */

    				continue;                    /* goback and get another input */
    			}
    		}
    		else
    		{
    			if(digit ==0)
    			{
    				Erase_BackLash(LPC_UART0);
    				State--;                     /* point to month's state */
    				digit = 1;                   /* now its point to 2nd digit of month */
    				count--;                     /* decrement character count */

    				continue;
    			}
    			else
    			{
    				Erase_Char_With_UnderScore(LPC_UART0);
    				digit = 0;                   /* point to 1st digit */
    				count--;                     /* decrement count */

    				continue;                    /* goback and get another input */
    			}
    		}
    	}
    	else if (cp>= '0' && cp <='9')     /* is character between 0 to 9? */
    	{
    		RTCdata = cp;

    		/****** check date ****/
    		if(count < MaxCount)
    		{
    			if(State == 0)
    			{
    				if(digit==0)
    				{
    					printf(LPC_UART0, "%c", RTCdata);          /* echo 1st character */
    					AsciiMSB = RTCdata;        /* store 1st byte MSB  */
    					count++;                   /* increment character count,will be 1 */
    					digit++;                   /* increment digit, will be 1 */

    					continue;
    				}
    				else
    				{
    					printf(LPC_UART0, "%c", RTCdata);                   /* echo 2nd character */
    					AsciiLSB = RTCdata;                 /* store 2nd byte LSB  */
    					RTCdata = ((AsciiMSB & 0x0F) * 10); /* store nibble */
    					RTCdata += (AsciiLSB & 0x0F);       /* add to nibble, integer */

    					if(RTCdata >= 1 && RTCdata <=31)    /* check date range ( 1-31) */
    					{
    						DateBuffer[0] = RTCdata; /* store DOM in DateBuffer[0] */

    						State++;                 /* increment to mm state */
    						count++;                 /* increment character count,will be 2 */
    						digit = 0;               /* set digit to zero  */
    						printf(LPC_UART0, "/");            /* and write '/' on the screen */

    						continue;                /* continue for minutes */
    					}
    					else
    					{
    						Erase_And_RingTheBell(LPC_UART0);

    						continue;
    					}
    				}
    			}
    			/************************* check month *************************/
    			if(State == 1)
    			{
    				if(digit==0)
    				{
    					printf(LPC_UART0, "%c", RTCdata);          /* echo 1st character */
    					AsciiMSB = RTCdata;        /* store 1st byte MSB  */
    					count++;                   /* increment character count,will be 3 */
    					digit++;                   /* increment digit, will be 1  */

    					continue;
    				}
    				else
    				{
    					printf(LPC_UART0, "%c", RTCdata);                   /* echo 2nd character */
    					AsciiLSB = RTCdata;                 /* store 2nd byte LSB  */
    					RTCdata = ((AsciiMSB & 0x0F) * 10); /* store nibble */
    					RTCdata += (AsciiLSB & 0x0F);       /* add to nibble, integer */

    					if(RTCdata >= 1 && RTCdata <=12)    /* check month range ( 1-12) */
    					{
    						DateBuffer[1] = RTCdata;          /* store mm in time buffer */

    						State++;                 /* increment to ss state */
    						count++;                 /* increment character count,will be 4 */
    						digit = 0;               /* set digit to zero  */
    						printf(LPC_UART0, "/");            /* and write '/' on the screen */

    						continue;                /* continue for seconds */
    					}
    					else
    					{
    						Erase_And_RingTheBell(LPC_UART0);

    						continue;
    					}
    				}
    			}
    			/********** check year and century *********/
    			if(State == 2)
    			{
    				if(digit==0)
    				{
    					printf(LPC_UART0, "%c", RTCdata);          /* echo 1st character */
    					AsciiMSB = RTCdata;        /* store 1st byte MSB  */

    					count++;                   /* increment character count,will be 5 */
    					digit++;                   /* increment digit no.  */

    					continue;
    				}
    				if(digit == 1)
    				{
    					printf(LPC_UART0, "%c", RTCdata);   /* echo 2nd character */
    					AsciiLSB = RTCdata;                 /* store 2nd byte LSB  */
    					RTCdata = ((AsciiMSB & 0x0F) * 10); /* store nibble */
    					RTCdata += (AsciiLSB & 0x0F);       /* add to nibble, integer */

    					if(RTCdata >= 0 && RTCdata <=40)   /* check century range (0-39) */
    					{
    						DateBuffer[2] = RTCdata;          /* store in century buffer */

    						count++;                 /* increment character count,will be 6 */
    						digit++;                 /* increment digit number, will be 3  */

    						continue;
    					}
    					else
    					{
    						Erase_Char_With_UnderScore(LPC_UART0);
    						printf(LPC_UART0, "\7");          /* ring the bell */

    						continue;
    					}
    				}
    				/**********  check year  ************/
    				if(digit == 2)
    				{
    					printf(LPC_UART0, "%c", RTCdata);/* echo 1st character */
    					AsciiMSB = RTCdata;/* store 1st byte MSB  */

    					count++;/* increment character count,will be 7 */
    					digit++;/* increment digit no.  */

    					continue;
    				}
    				if(digit == 3)
    				{
    					printf(LPC_UART0, "%c", RTCdata);                    /* echo 2nd character */
    					AsciiLSB = RTCdata;                  /* store 2nd byte LSB  */
    					RTCdata = ((AsciiMSB & 0x0F) * 10);  /* store nibble */
    					RTCdata += (AsciiLSB & 0x0F);        /* add to nibble, integer */

    					if(DateBuffer[2]==40)
    					{
    						if(RTCdata <=95)                     /* check year range (00-95) */
    						{
    							DateBuffer[3] = RTCdata;           /* store in year buffer */

	    						DateFlag = 1;            /* Time data is ready for RTC */
         						count++;                 /* increment character count,will be 8 */
        						digit++;                 /* increment digit number, will be 4  */
         						continue;
    						}
    						else
    						{
    							Erase_Char_With_UnderScore(LPC_UART0);
    							printf(LPC_UART0, "\7");           /* ring the bell */

    							continue;
    						}
    					}

    					if(RTCdata <=99)                     /* check year range (00-99) */
    					{
    						DateBuffer[3] = RTCdata;           /* store in year buffer */

							/******* Check if the operator is sneakly to enter an invalid date *****/
							DaysThisMonth = DaysPerMonth[DateBuffer[1]- 1];

							if(Is_Leap_Year(DateBuffer[3], DateBuffer[2]) && (DateBuffer[1] == 2))
							{
								++DaysThisMonth;
							}

							if(DateBuffer[0] > DaysThisMonth)
							{
								DateBuffer[0] = DaysThisMonth;
							}

    						DateFlag = 1;            /* Time data is ready for RTC */
    						count++;                 /* increment character count,will be 8 */
    						digit++;                 /* increment digit number, will be 4  */

    						continue;
    					}
    					else
    					{
    						Erase_Char_With_UnderScore(LPC_UART0);
    						printf(LPC_UART0, "\7");           /* ring the bell */

    						continue;
    					}
    				}
    			}
    			continue;                      /* number of char is more than reqd */
    			/* so go back to find the decision */
    		}
    		else
    		{
    			printf(LPC_UART0, "%c", cp);                   /* echo character */
    			Erase_Char(LPC_UART0);
    			printf(LPC_UART0, "\7");                 /* ring the bell */

    			continue;
    		}
    	}
    }
    return(0);
}



/********************************************************************//**
 * @brief		Initializes the RTC peripheral.
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @return 		None
 *********************************************************************/
void RTC_Init (LPC_RTC_TypeDef *RTCx)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));

	/* Set up clock and power for RTC module */
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCRTC, ENABLE);

	// Clear all register to be default
	RTCx->ILR = 0x00;
	RTCx->CCR = 0x00;
	RTCx->CIIR = 0x00;
	RTCx->AMR = 0xFF;
	RTCx->CALIBRATION = 0x00;
}


/*********************************************************************//**
 * @brief		De-initializes the RTC peripheral registers to their
*                  default reset values.
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @return 		None
 **********************************************************************/
void RTC_DeInit(LPC_RTC_TypeDef *RTCx)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));

	RTCx->CCR = 0x00;
	// Disable power and clock for RTC module
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCRTC, DISABLE);
}

/*********************************************************************//**
 * @brief 		Reset clock tick counter in RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @return 		None
 **********************************************************************/
void RTC_ResetClockTickCounter(LPC_RTC_TypeDef *RTCx)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));

	RTCx->CCR |= RTC_CCR_CTCRST;
	RTCx->CCR &= (~RTC_CCR_CTCRST) & RTC_CCR_BITMASK;
}

/*********************************************************************//**
 * @brief 		Start/Stop RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	NewState New State of this function, should be:
 * 				- ENABLE: The time counters are enabled
 * 				- DISABLE: The time counters are disabled
 * @return 		None
 **********************************************************************/
void RTC_Cmd (LPC_RTC_TypeDef *RTCx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		RTCx->CCR |= RTC_CCR_CLKEN;
	}
	else
	{
		RTCx->CCR &= (~RTC_CCR_CLKEN) & RTC_CCR_BITMASK;
	}
}


/*********************************************************************//**
 * @brief 		Enable/Disable Counter increment interrupt for each time type
 * 				in RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	CntIncrIntType: Counter Increment Interrupt type,
 * 				an increment of this type value below will generates
 * 				an interrupt, should be:
 * 				- RTC_TIMETYPE_SECOND
 * 				- RTC_TIMETYPE_MINUTE
 * 				- RTC_TIMETYPE_HOUR
 * 				- RTC_TIMETYPE_DAYOFWEEK
 * 				- RTC_TIMETYPE_DAYOFMONTH
 * 				- RTC_TIMETYPE_DAYOFYEAR
 * 				- RTC_TIMETYPE_MONTH
 * 				- RTC_TIMETYPE_YEAR
 * @param[in]	NewState New State of this function, should be:
 * 				- ENABLE: Counter Increment interrupt for this
 * 					time type are enabled
 * 				- DISABLE: Counter Increment interrupt for this
 * 					time type are disabled
 * @return 		None
 **********************************************************************/
void RTC_CntIncrIntConfig (LPC_RTC_TypeDef *RTCx, uint32_t CntIncrIntType, \
								FunctionalState NewState)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));
	CHECK_PARAM(PARAM_RTC_TIMETYPE(CntIncrIntType));

	if (NewState ==  ENABLE)
	{
		switch (CntIncrIntType)
		{
		case RTC_TIMETYPE_SECOND:
			RTCx->CIIR |= RTC_CIIR_IMSEC;
			break;
		case RTC_TIMETYPE_MINUTE:
			RTCx->CIIR |= RTC_CIIR_IMMIN;
			break;
		case RTC_TIMETYPE_HOUR:
			RTCx->CIIR |= RTC_CIIR_IMHOUR;
			break;
		case RTC_TIMETYPE_DAYOFWEEK:
			RTCx->CIIR |= RTC_CIIR_IMDOW;
			break;
		case RTC_TIMETYPE_DAYOFMONTH:
			RTCx->CIIR |= RTC_CIIR_IMDOM;
			break;
		case RTC_TIMETYPE_DAYOFYEAR:
			RTCx->CIIR |= RTC_CIIR_IMDOY;
			break;
		case RTC_TIMETYPE_MONTH:
			RTCx->CIIR |= RTC_CIIR_IMMON;
			break;
		case RTC_TIMETYPE_YEAR:
			RTCx->CIIR |= RTC_CIIR_IMYEAR;
			break;
		}
	}
	else
	{
		switch (CntIncrIntType)
		{
		case RTC_TIMETYPE_SECOND:
			RTCx->CIIR &= (~RTC_CIIR_IMSEC) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_MINUTE:
			RTCx->CIIR &= (~RTC_CIIR_IMMIN) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_HOUR:
			RTCx->CIIR &= (~RTC_CIIR_IMHOUR) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFWEEK:
			RTCx->CIIR &= (~RTC_CIIR_IMDOW) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFMONTH:
			RTCx->CIIR &= (~RTC_CIIR_IMDOM) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFYEAR:
			RTCx->CIIR &= (~RTC_CIIR_IMDOY) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_MONTH:
			RTCx->CIIR &= (~RTC_CIIR_IMMON) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_YEAR:
			RTCx->CIIR &= (~RTC_CIIR_IMYEAR) & RTC_CIIR_BITMASK;
			break;
		}
	}
}


/*********************************************************************//**
 * @brief 		Enable/Disable Alarm interrupt for each time type
 * 				in RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	AlarmTimeType: Alarm Time Interrupt type,
 * 				an matching of this type value below with current time
 * 				in RTC will generates an interrupt, should be:
 * 				- RTC_TIMETYPE_SECOND
 * 				- RTC_TIMETYPE_MINUTE
 * 				- RTC_TIMETYPE_HOUR
 * 				- RTC_TIMETYPE_DAYOFWEEK
 * 				- RTC_TIMETYPE_DAYOFMONTH
 * 				- RTC_TIMETYPE_DAYOFYEAR
 * 				- RTC_TIMETYPE_MONTH
 * 				- RTC_TIMETYPE_YEAR
 * @param[in]	NewState New State of this function, should be:
 * 				- ENABLE: Alarm interrupt for this
 * 					time type are enabled
 * 				- DISABLE: Alarm interrupt for this
 * 					time type are disabled
 * @return 		None
 **********************************************************************/
void RTC_AlarmIntConfig (LPC_RTC_TypeDef *RTCx, uint32_t AlarmTimeType, \
								FunctionalState NewState)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));
	CHECK_PARAM(PARAM_RTC_TIMETYPE(AlarmTimeType));

	if (NewState == ENABLE)
	{
		switch (AlarmTimeType)
		{
		case RTC_TIMETYPE_SECOND:
			RTCx->AMR &= (~RTC_AMR_AMRSEC) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_MINUTE:
			RTCx->AMR &= (~RTC_AMR_AMRMIN) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_HOUR:
			RTCx->AMR &= (~RTC_AMR_AMRHOUR) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFWEEK:
			RTCx->AMR &= (~RTC_AMR_AMRDOW) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFMONTH:
			RTCx->AMR &= (~RTC_AMR_AMRDOM) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFYEAR:
			RTCx->AMR &= (~RTC_AMR_AMRDOY) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_MONTH:
			RTCx->AMR &= (~RTC_AMR_AMRMON) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_YEAR:
			RTCx->AMR &= (~RTC_AMR_AMRYEAR) & RTC_AMR_BITMASK;
			break;
		}
	}
	else
	{
		switch (AlarmTimeType)
		{
		case RTC_TIMETYPE_SECOND:
			RTCx->AMR |= (RTC_AMR_AMRSEC);
			break;
		case RTC_TIMETYPE_MINUTE:
			RTCx->AMR |= (RTC_AMR_AMRMIN);
			break;
		case RTC_TIMETYPE_HOUR:
			RTCx->AMR |= (RTC_AMR_AMRHOUR);
			break;
		case RTC_TIMETYPE_DAYOFWEEK:
			RTCx->AMR |= (RTC_AMR_AMRDOW);
			break;
		case RTC_TIMETYPE_DAYOFMONTH:
			RTCx->AMR |= (RTC_AMR_AMRDOM);
			break;
		case RTC_TIMETYPE_DAYOFYEAR:
			RTCx->AMR |= (RTC_AMR_AMRDOY);
			break;
		case RTC_TIMETYPE_MONTH:
			RTCx->AMR |= (RTC_AMR_AMRMON);
			break;
		case RTC_TIMETYPE_YEAR:
			RTCx->AMR |= (RTC_AMR_AMRYEAR);
			break;
		}
	}
}


/*********************************************************************//**
 * @brief 		Set current time value for each time type in RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	Timetype: Time Type, should be:
 * 				- RTC_TIMETYPE_SECOND
 * 				- RTC_TIMETYPE_MINUTE
 * 				- RTC_TIMETYPE_HOUR
 * 				- RTC_TIMETYPE_DAYOFWEEK
 * 				- RTC_TIMETYPE_DAYOFMONTH
 * 				- RTC_TIMETYPE_DAYOFYEAR
 * 				- RTC_TIMETYPE_MONTH
 * 				- RTC_TIMETYPE_YEAR
 * @param[in]	TimeValue Time value to set
 * @return 		None
 **********************************************************************/
void RTC_SetTime (LPC_RTC_TypeDef *RTCx, uint32_t Timetype, uint32_t TimeValue)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));
	CHECK_PARAM(PARAM_RTC_TIMETYPE(Timetype));

	switch ( Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		CHECK_PARAM(TimeValue <= RTC_SECOND_MAX);

		RTCx->SEC = TimeValue & RTC_SEC_MASK;
		break;

	case RTC_TIMETYPE_MINUTE:
		CHECK_PARAM(TimeValue <= RTC_MINUTE_MAX);

		RTCx->MIN = TimeValue & RTC_MIN_MASK;
		break;

	case RTC_TIMETYPE_HOUR:
		CHECK_PARAM(TimeValue <= RTC_HOUR_MAX);

		RTCx->HOUR = TimeValue & RTC_HOUR_MASK;
		break;

	case RTC_TIMETYPE_DAYOFWEEK:
		CHECK_PARAM(TimeValue <= RTC_DAYOFWEEK_MAX);

		RTCx->DOW = TimeValue & RTC_DOW_MASK;
		break;

	case RTC_TIMETYPE_DAYOFMONTH:
		CHECK_PARAM((TimeValue <= RTC_DAYOFMONTH_MAX) \
				&& (TimeValue >= RTC_DAYOFMONTH_MIN));

		RTCx->DOM = TimeValue & RTC_DOM_MASK;
		break;

	case RTC_TIMETYPE_DAYOFYEAR:
		CHECK_PARAM((TimeValue >= RTC_DAYOFYEAR_MIN) \
				&& (TimeValue <= RTC_DAYOFYEAR_MAX));

		RTCx->DOY = TimeValue & RTC_DOY_MASK;
		break;

	case RTC_TIMETYPE_MONTH:
		CHECK_PARAM((TimeValue >= RTC_MONTH_MIN) \
				&& (TimeValue <= RTC_MONTH_MAX));

		RTCx->MONTH = TimeValue & RTC_MONTH_MASK;
		break;

	case RTC_TIMETYPE_YEAR:
		CHECK_PARAM(TimeValue <= RTC_YEAR_MAX);

		RTCx->YEAR = TimeValue & RTC_YEAR_MASK;
		break;
	}
}

/*********************************************************************//**
 * @brief 		Get current time value for each type time type
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	Timetype: Time Type, should be:
 * 				- RTC_TIMETYPE_SECOND
 * 				- RTC_TIMETYPE_MINUTE
 * 				- RTC_TIMETYPE_HOUR
 * 				- RTC_TIMETYPE_DAYOFWEEK
 * 				- RTC_TIMETYPE_DAYOFMONTH
 * 				- RTC_TIMETYPE_DAYOFYEAR
 * 				- RTC_TIMETYPE_MONTH
 * 				- RTC_TIMETYPE_YEAR
 * @return 		Value of time according to specified time type
 **********************************************************************/
uint32_t RTC_GetTime(LPC_RTC_TypeDef *RTCx, uint32_t Timetype)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));
	CHECK_PARAM(PARAM_RTC_TIMETYPE(Timetype));

	switch (Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		return (RTCx->SEC & RTC_SEC_MASK);
	case RTC_TIMETYPE_MINUTE:
		return (RTCx->MIN & RTC_MIN_MASK);
	case RTC_TIMETYPE_HOUR:
		return (RTCx->HOUR & RTC_HOUR_MASK);
	case RTC_TIMETYPE_DAYOFWEEK:
		return (RTCx->DOW & RTC_DOW_MASK);
	case RTC_TIMETYPE_DAYOFMONTH:
		return (RTCx->DOM & RTC_DOM_MASK);
	case RTC_TIMETYPE_DAYOFYEAR:
		return (RTCx->DOY & RTC_DOY_MASK);
	case RTC_TIMETYPE_MONTH:
		return (RTCx->MONTH & RTC_MONTH_MASK);
	case RTC_TIMETYPE_YEAR:
		return (RTCx->YEAR & RTC_YEAR_MASK);
	default:
		return (0);
	}
}


/*********************************************************************//**
 * @brief 		Set full of time in RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	pFullTime Pointer to a RTC_TIME_Type structure that
 * 				contains time value in full.
 * @return 		None
 **********************************************************************/
void RTC_SetFullTime (LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));

	RTCx->DOM = pFullTime->DOM & RTC_DOM_MASK;
	RTCx->DOW = pFullTime->DOW & RTC_DOW_MASK;
	RTCx->DOY = pFullTime->DOY & RTC_DOY_MASK;
	RTCx->HOUR = pFullTime->HOUR & RTC_HOUR_MASK;
	RTCx->MIN = pFullTime->MIN & RTC_MIN_MASK;
	RTCx->SEC = pFullTime->SEC & RTC_SEC_MASK;
	RTCx->MONTH = pFullTime->MONTH & RTC_MONTH_MASK;
	RTCx->YEAR = pFullTime->YEAR & RTC_YEAR_MASK;
}


/*********************************************************************//**
 * @brief 		Get full of time in RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	pFullTime Pointer to a RTC_TIME_Type structure that
 * 				will be stored time in full.
 * @return 		None
 **********************************************************************/
void RTC_GetFullTime (LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));

	pFullTime->DOM = RTCx->DOM & RTC_DOM_MASK;
	pFullTime->DOW = RTCx->DOW & RTC_DOW_MASK;
	pFullTime->DOY = RTCx->DOY & RTC_DOY_MASK;
	pFullTime->HOUR = RTCx->HOUR & RTC_HOUR_MASK;
	pFullTime->MIN = RTCx->MIN & RTC_MIN_MASK;
	pFullTime->SEC = RTCx->SEC & RTC_SEC_MASK;
	pFullTime->MONTH = RTCx->MONTH & RTC_MONTH_MASK;
	pFullTime->YEAR = RTCx->YEAR & RTC_YEAR_MASK;
}


/*********************************************************************//**
 * @brief 		Set alarm time value for each time type
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	Timetype: Time Type, should be:
 * 				- RTC_TIMETYPE_SECOND
 * 				- RTC_TIMETYPE_MINUTE
 * 				- RTC_TIMETYPE_HOUR
 * 				- RTC_TIMETYPE_DAYOFWEEK
 * 				- RTC_TIMETYPE_DAYOFMONTH
 * 				- RTC_TIMETYPE_DAYOFYEAR
 * 				- RTC_TIMETYPE_MONTH
 * 				- RTC_TIMETYPE_YEAR
 * @param[in]	ALValue Alarm time value to set
 * @return 		None
 **********************************************************************/
void RTC_SetAlarmTime (LPC_RTC_TypeDef *RTCx, uint32_t Timetype, uint32_t ALValue)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));

	switch (Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		CHECK_PARAM(ALValue <= RTC_SECOND_MAX);

		RTCx->ALSEC = ALValue & RTC_SEC_MASK;
		break;

	case RTC_TIMETYPE_MINUTE:
		CHECK_PARAM(ALValue <= RTC_MINUTE_MAX);

		RTCx->ALMIN = ALValue & RTC_MIN_MASK;
		break;

	case RTC_TIMETYPE_HOUR:
		CHECK_PARAM(ALValue <= RTC_HOUR_MAX);

		RTCx->ALHOUR = ALValue & RTC_HOUR_MASK;
		break;

	case RTC_TIMETYPE_DAYOFWEEK:
		CHECK_PARAM(ALValue <= RTC_DAYOFWEEK_MAX);

		RTCx->ALDOW = ALValue & RTC_DOW_MASK;
		break;

	case RTC_TIMETYPE_DAYOFMONTH:
		CHECK_PARAM((ALValue <= RTC_DAYOFMONTH_MAX) \
				&& (ALValue >= RTC_DAYOFMONTH_MIN));

		RTCx->ALDOM = ALValue & RTC_DOM_MASK;
		break;

	case RTC_TIMETYPE_DAYOFYEAR:
		CHECK_PARAM((ALValue >= RTC_DAYOFYEAR_MIN) \
				&& (ALValue <= RTC_DAYOFYEAR_MAX));

		RTCx->ALDOY = ALValue & RTC_DOY_MASK;
		break;

	case RTC_TIMETYPE_MONTH:
		CHECK_PARAM((ALValue >= RTC_MONTH_MIN) \
				&& (ALValue <= RTC_MONTH_MAX));

		RTCx->ALMON = ALValue & RTC_MONTH_MASK;
		break;

	case RTC_TIMETYPE_YEAR:
		CHECK_PARAM(ALValue <= RTC_YEAR_MAX);

		RTCx->ALYEAR = ALValue & RTC_YEAR_MASK;
		break;
	}
}



/*********************************************************************//**
 * @brief 		Get alarm time value for each time type
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	Timetype: Time Type, should be:
 * 				- RTC_TIMETYPE_SECOND
 * 				- RTC_TIMETYPE_MINUTE
 * 				- RTC_TIMETYPE_HOUR
 * 				- RTC_TIMETYPE_DAYOFWEEK
 * 				- RTC_TIMETYPE_DAYOFMONTH
 * 				- RTC_TIMETYPE_DAYOFYEAR
 * 				- RTC_TIMETYPE_MONTH
 * 				- RTC_TIMETYPE_YEAR
  * @return 	Value of Alarm time according to specified time type
 **********************************************************************/
uint32_t RTC_GetAlarmTime (LPC_RTC_TypeDef *RTCx, uint32_t Timetype)
{
	switch (Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		return (RTCx->ALSEC & RTC_SEC_MASK);
	case RTC_TIMETYPE_MINUTE:
		return (RTCx->ALMIN & RTC_MIN_MASK);
	case RTC_TIMETYPE_HOUR:
		return (RTCx->ALHOUR & RTC_HOUR_MASK);
	case RTC_TIMETYPE_DAYOFWEEK:
		return (RTCx->ALDOW & RTC_DOW_MASK);
	case RTC_TIMETYPE_DAYOFMONTH:
		return (RTCx->ALDOM & RTC_DOM_MASK);
	case RTC_TIMETYPE_DAYOFYEAR:
		return (RTCx->ALDOY & RTC_DOY_MASK);
	case RTC_TIMETYPE_MONTH:
		return (RTCx->ALMON & RTC_MONTH_MASK);
	case RTC_TIMETYPE_YEAR:
		return (RTCx->ALYEAR & RTC_YEAR_MASK);
	default:
		return (0);
	}
}


/*********************************************************************//**
 * @brief 		Set full of alarm time in RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	pFullTime Pointer to a RTC_TIME_Type structure that
 * 				contains alarm time value in full.
 * @return 		None
 **********************************************************************/
void RTC_SetFullAlarmTime (LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));

	RTCx->ALDOM = pFullTime->DOM & RTC_DOM_MASK;
	RTCx->ALDOW = pFullTime->DOW & RTC_DOW_MASK;
	RTCx->ALDOY = pFullTime->DOY & RTC_DOY_MASK;
	RTCx->ALHOUR = pFullTime->HOUR & RTC_HOUR_MASK;
	RTCx->ALMIN = pFullTime->MIN & RTC_MIN_MASK;
	RTCx->ALSEC = pFullTime->SEC & RTC_SEC_MASK;
	RTCx->ALMON = pFullTime->MONTH & RTC_MONTH_MASK;
	RTCx->ALYEAR = pFullTime->YEAR & RTC_YEAR_MASK;
}


/*********************************************************************//**
 * @brief 		Get full of alarm time in RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	pFullTime Pointer to a RTC_TIME_Type structure that
 * 				will be stored alarm time in full.
 * @return 		None
 **********************************************************************/
void RTC_GetFullAlarmTime (LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));

	pFullTime->DOM = RTCx->ALDOM & RTC_DOM_MASK;
	pFullTime->DOW = RTCx->ALDOW & RTC_DOW_MASK;
	pFullTime->DOY = RTCx->ALDOY & RTC_DOY_MASK;
	pFullTime->HOUR = RTCx->ALHOUR & RTC_HOUR_MASK;
	pFullTime->MIN = RTCx->ALMIN & RTC_MIN_MASK;
	pFullTime->SEC = RTCx->ALSEC & RTC_SEC_MASK;
	pFullTime->MONTH = RTCx->ALMON & RTC_MONTH_MASK;
	pFullTime->YEAR = RTCx->ALYEAR & RTC_YEAR_MASK;
}


/*********************************************************************//**
 * @brief 		Check whether if specified Location interrupt in
 * 				RTC peripheral is set or not
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	IntType Interrupt location type, should be:
 * 						- RTC_INT_COUNTER_INCREASE: Counter Increment Interrupt
 * 							block generated an interrupt.
 * 						- RTC_INT_ALARM: Alarm generated an
 * 							interrupt.
 * @return 		New state of specified Location interrupt in RTC peripheral
 * 				(SET or RESET)
 **********************************************************************/
IntStatus RTC_GetIntPending (LPC_RTC_TypeDef *RTCx, uint32_t IntType)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));
	CHECK_PARAM(PARAM_RTC_INT(IntType));

	return ((RTCx->ILR & IntType) ? SET : RESET);
}


/*********************************************************************//**
 * @brief 		Clear specified Location interrupt pending in
 * 				RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	IntType Interrupt location type, should be:
 * 						- RTC_INT_COUNTER_INCREASE: Clear Counter Increment
 * 						Interrupt pending.
 * 						- RTC_INT_ALARM: Clear alarm interrupt pending
 * @return 		None
 **********************************************************************/
void RTC_ClearIntPending (LPC_RTC_TypeDef *RTCx, uint32_t IntType)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));
	CHECK_PARAM(PARAM_RTC_INT(IntType));

	RTCx->ILR |= IntType;
}

/*********************************************************************//**
 * @brief 		Enable/Disable calibration counter in RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	NewState New State of this function, should be:
 * 				- ENABLE: The calibration counter is enabled and counting
 * 				- DISABLE: The calibration counter is disabled and reset to zero
 * @return 		None
 **********************************************************************/
void RTC_CalibCounterCmd(LPC_RTC_TypeDef *RTCx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		RTCx->CCR &= (~RTC_CCR_CCALEN) & RTC_CCR_BITMASK;
	}
	else
	{
		RTCx->CCR |= RTC_CCR_CCALEN;
	}
}


/*********************************************************************//**
 * @brief 		Configures Calibration in RTC peripheral
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	CalibValue Calibration value, should be in range from
 * 					0 to 131,072
 * @param[in]	CalibDir Calibration Direction, should be:
 * 					- RTC_CALIB_DIR_FORWARD: Forward calibration
 * 					- RTC_CALIB_DIR_BACKWARD: Backward calibration
 * @return 		None
 **********************************************************************/
void RTC_CalibConfig(LPC_RTC_TypeDef *RTCx, uint32_t CalibValue, uint8_t CalibDir)
{
	CHECK_PARAM(PARAM_RTCx(RTCx));
	CHECK_PARAM(PARAM_RTC_CALIB_DIR(CalibDir));
	CHECK_PARAM(CalibValue < RTC_CALIBRATION_MAX);

	RTCx->CALIBRATION = ((CalibValue - 1) & RTC_CALIBRATION_CALVAL_MASK) \
			| ((CalibDir == RTC_CALIB_DIR_BACKWARD) ? RTC_CALIBRATION_LIBDIR : 0);
}


/*********************************************************************//**
 * @brief 		Write value to General purpose registers
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	Channel General purpose registers Channel number,
 * 				should be in range from 0 to 4.
 * @param[in]	Value Value to write
 * @return 		None
 * Note: These General purpose registers can be used to store important
 * information when the main power supply is off. The value in these
 * registers is not affected by chip reset.
 **********************************************************************/
void RTC_WriteGPREG (LPC_RTC_TypeDef *RTCx, uint8_t Channel, uint32_t Value)
{
	uint32_t *preg;

	CHECK_PARAM(PARAM_RTCx(RTCx));
	CHECK_PARAM(PARAM_RTC_GPREG_CH(Channel));

	preg = (uint32_t *)&RTCx->GPREG0;
	preg += Channel;
	*preg = Value;
}


/*********************************************************************//**
 * @brief 		Read value from General purpose registers
 * @param[in]	RTCx	RTC peripheral selected, should be LPC_RTC
 * @param[in]	Channel General purpose registers Channel number,
 * 				should be in range from 0 to 4.
 * @return 		Read Value
 * Note: These General purpose registers can be used to store important
 * information when the main power supply is off. The value in these
 * registers is not affected by chip reset.
 **********************************************************************/
uint32_t RTC_ReadGPREG (LPC_RTC_TypeDef *RTCx, uint8_t Channel)
{
	uint32_t *preg;
	uint32_t value;

	CHECK_PARAM(PARAM_RTCx(RTCx));
	CHECK_PARAM(PARAM_RTC_GPREG_CH(Channel));

	preg = (uint32_t *)&RTCx->GPREG0;
	preg += Channel;
	value = *preg;
	return (value);
}

/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

