/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name     : wdt.c
* Version       : 1.0
* Device(s)     : RX6xx
* Description   : WDT timer code
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.02.2011     1.00        First release
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <platform.h>

/******************************************************************************
* Function name: wdt_feed_watchdog
* Description  : Feed the watchdog by writing 00h and then writing FFh to WDTRR
*                within the refresh-permitted period.
*                The watchdog timer's registers are intentionally more difficult
*                to write to than most other processor registers. This makes it
*                harder for a runaway rogue program to disable the watchdog.
* Arguments    : none
* Return value : none
******************************************************************************/
void wdt_feed_watchdog(void)
{
	WDT.WDTRR = 0x00;
	WDT.WDTRR = 0xFF;
} /* End of function wdt_feed_watchdog(). */

/******************************************************************************
* Function name: wdt_init
* Description  : Sets up the WDT
*                The watchdog timer's registers are intentionally more difficult
*                to write to than most other processor registers. This makes it
*                harder for a runaway rogue program to disable the watchdog.
* Arguments    : none
* Return value : none
******************************************************************************/
void wdt_init(void)
{
	/* Note that OFS0.WDTSTRT is 1 at power up, so the WDT is in register start mode */
	
    /* WDT Reset Control Register (WDTRCR)
       Writing to this register is only possible between processor reset and the
	   very first watchdog refresh operation. 
	   This makes it hard for a runaway rogue program to disable the watchdog.
	b7      RSTIRQS:  Reset Interrupt Request Selection
	b6:b0   Reserved: These bits are read as 0 and cannot be modified.
    */
  	WDT.WDTRCR.BYTE = 0x00; /* 0x80 = Reset (and not NMI) output is enabled. */
	
	/* If you want to use the Non-Maskable Interrupt NMI with the watchdog, you must 
	   unmask it by using the Non-Maskable Interrupt Enable Register NMIER. */
	ICU.NMIER.BIT.WDTEN = 1;
	
    /* WDT Control Register (WDTCR)
       Writing to this register is only possible between processor reset and the
	   very first watchdog refresh operation. 
	   This makes it hard for a runaway rogue program to disable the watchdog.
	b15:b14 Reserved: These bits are read as 0 and cannot be modified.
	b13:b12 RPSS:     Window Start Position Selection
	b11:b10 Reserved: These bits are read as 0 and cannot be modified.
	b9:b8   RPES:     Window End Position Selection
	b7:b4   CKS:      Clock Division Ratio Selection
	b3:b2   Reserved: These bits are read as 0 and cannot be modified.
    b1:b0   TOPS:     Time-Out Period Selection
    */
	WDT.WDTCR.WORD = 0x3383; /* Timeout period without feedings is 134,217,728 PCLKs */
	                         /* PCLK = 48 MHz */
					   	     /* Timeout period = 134,217,728 / 48,000,000 = 2.796 seconds */
						     /* The watchdog may be fed at any time. */
						 
    /* WDT Status Register (WDTSR)
	b15     REFEF:    Refresh Error Flag
	b14     UNDFF:    Underflow Flag
	b13:b0  CNTVAL:   Down-Counter Value.
    */
	WDT.WDTSR.WORD = 0x0000; /* Clear the refresh error and underflow flags */
	
	wdt_feed_watchdog();     /* The very first watchdog reset operation */
} /* End of function wdt_init(). */

/*******************************************************************************
End of file: wdt.c
*******************************************************************************/
