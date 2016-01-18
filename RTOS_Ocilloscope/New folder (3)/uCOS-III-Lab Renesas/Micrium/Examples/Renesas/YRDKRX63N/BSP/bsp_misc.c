/*
*********************************************************************************************************
*                                         BOARD SUPPORT PACKAGE
*
*                            (c) Copyright 2014; Micrium, Inc.; Weston, FL
*
*               All rights reserved. Protected by international copyright laws.
*
*               BSP is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
* Filename      : bsp_misc.c
* Version       : V1.00
* Programmer(s) : JPB
*                 AA
*                 JM
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              INCLUDES
*********************************************************************************************************
*/

#include  <bsp_cfg.h>

#if  BSP_CFG_MISC_EN > 0u
#include  <cpu.h>
#include  <lib_def.h>
#include  <bsp_misc.h>
#include  <iodefine.h>


/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void   BSP_MicInit   (void);
static  void   BSP_SwsInit   (void);


/*
*********************************************************************************************************
*                                  MISCELLANEOUS BSP INITIALIZATION
*
* Description: This function initializes miscellaneous BSP services
*
* Arguments  : none
*********************************************************************************************************
*/

void  BSP_MiscInit (void)
{
    BSP_MicInit();                                              /* Initialize the Microphone                            */
    BSP_SwsInit();                                              /* Initialize the Switches                              */
}


/*
*********************************************************************************************************
*                                     MICROPHONE INITIALIZATION
*
* Description: This function is used to initialize the 12-bit ADC Channel-05 that is attached
*              to the output from the microphone.
*
* Arguments  : none.
*
* Return     : none.
*********************************************************************************************************
*/

static  void  BSP_MicInit (void)
{
    SYSTEM.MSTPCRA.BIT.MSTPA17 = DEF_BIT_NONE;
    S12AD.ADCSR.BIT.EXTRG      = DEF_BIT_NONE;
    S12AD.ADCSR.BIT.TRGE       = DEF_BIT_NONE;
    S12AD.ADCSR.BIT.CKS        = DEF_BIT_NONE;
    S12AD.ADCSR.BIT.ADIE       = DEF_BIT_NONE;
    S12AD.ADCSR.BIT.ADCS       = DEF_BIT_NONE;                  /* Select continuous scan mode.                         */
    S12AD.ADCSR.BIT.ADST       = DEF_BIT_00;                    /* Stop ADC conversion process.                         */

    PORTA.PODR.BIT.B2          = DEF_ON;                        /* Turn on the microphone's amplifier.                  */
}


/*
*********************************************************************************************************
*                                          MICROPHONE READ
*
* Description: This function is used to read the 12-bit ADC Channel-05 raw count that
*              represents the sound level.
*
* Arguments  : none.
*
* Return     : 12-bit ADC raw count.
*********************************************************************************************************
*/

CPU_INT16U  BSP_MicRd (void)
{
    CPU_INT16U  cur_sound_level;


    S12AD.ADANS0.BIT.ANS0 = DEF_BIT_05;                         /* Select the Microphone at AN05                        */
    S12AD.ADCSR.BIT.ADST  = DEF_BIT_00;                         /* Start A/D Conversion process                         */
    while (S12AD.ADCSR.BIT.ADST == 0x01) {
        ;
    }
    cur_sound_level = S12AD.ADDR5;

    return (cur_sound_level);
}


/*
*********************************************************************************************************
*                                       ADC INITIALIZATION
*
* Description: This function is used to initialize the 12-bit ADC Channel-02 that is attached
*              to the output from the potentiometer.
*
* Arguments  : none.
*
* Return     : none.
*********************************************************************************************************
*/

void  BSP_ADC_Init (void)
{
    PORT4.PMR.BIT.B1     = 1;                                   /* Enable the input buffer for the pot's pin            */
   //   PORT4.PODR.BIT.B1 = 0;    /* Clear I/O pin data register to low output. */
   // PORT4.PDR.BIT.B1  = 0;    /* Set I/O pin direction to input. */
   // PORT4.PMR.BIT.B1  = 0;    /* First set I/O pin mode register to GPIO mode. */    
    SYSTEM.PRCR.WORD     = 0xA50B;                              /* Protect off                                          */		
    MSTP(S12AD) = 0;                                            /* Enable the 12-bit ADC                                */
    SYSTEM.PRCR.WORD     = 0xA500;                              /* Protect on                                           */  	
	
	                                                        /* Set up conversions on TMR0 match                     */
    S12AD.ADSTRGR.BYTE   = 0x09;
	                                                        /* Clear value after each conversion                    */
    S12AD.ADCER.WORD     = 0x0020;	
	                                                        /* Select ADC channel 4                                 */
    S12AD.ADANS0.WORD    = 0x0002;
    S12AD.ADADS0.WORD    = 0x0000;			
	                                                        /* Set up clock and trigger                             */		 
    S12AD.ADCSR.BYTE     = 0x12;
#ifdef OS3_ADC_LAB
    IR(S12AD0, S12ADI0)  = 0;                                   /* Clear the ADC's interrupt flag                       */
    IPR(S12AD0, S12ADI0) = 5;                                   /* Set the priority for ADC interrupts                  */
    IEN(S12AD0, S12ADI0) = 1;                                   /* Enable ADC interrupts                                */	
#endif	

    SYSTEM.PRCR.WORD     = 0xA50B;                              /* Protect off                                          */    
    MSTP(TMR0)           = 0;
    SYSTEM.PRCR.WORD     = 0xA500;                              /* Protect on                                           */  
    
	                                                        /* Set timer to be cleared on match                     */ 
    TMR0.TCR.BYTE        = 0x08;	
	                                                        /* Set timer to trigger conversions on match            */
    TMR0.TCSR.BYTE       = 0x10;
	                                                        /* Specify match value                                  */
    TMR0.TCORA           = 0x01;
	                                                        /* Specify clock speed of PCLK/8192                     */
    TMR0.TCCR.BYTE       = 0x0E;  
}


/*
*********************************************************************************************************
*                                         ADC READ
*
* Description: This function is used to read a converted value from the 12-bit ADC 
*
* Arguments  : none.
*
* Return     : 12-bit ADC raw count.
*********************************************************************************************************
*/

CPU_INT16U  BSP_ADC_Rd (void)
{
    CPU_INT16U  adc_level;


    adc_level = S12AD.ADDR2;                                    /* Read the result register for AN2                    */
    return (adc_level);
}


/*
*********************************************************************************************************
*                                     SWITCHES INITIALIZATION
*
* Description: This function is used to initialize the switches attached to the GPIO.
*
* Arguments  : none.
*
* Return     : none.
*********************************************************************************************************
*/

static  void  BSP_SwsInit (void)
{
    PORT4.PDR.BIT.B0 = 0;                                       /* Switch 1 as an input.                                */
    PORT4.PDR.BIT.B1 = 0;                                       /* Switch 2 as an input.                                */
    PORT4.PDR.BIT.B4 = 0;                                       /* Switch 3 as an input.                                */
}


/*
*********************************************************************************************************
*                                           SWITCHES READ
*
* Description: This function is used to read the switches attached to the GPIO.
*
* Arguments  : none.
*
* Return     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_SwsRd (CPU_INT08U  sw_nbr)
{
    CPU_BOOLEAN  status;


    switch (sw_nbr) {
        case 1:
             status = !(PORT4.PIDR.BIT.B0);
             break;

        case 2:
             status = !(PORT4.PIDR.BIT.B1);
             break;

        case 3:
             status = !(PORT4.PIDR.BIT.B4);
             break;

        default:
             status = DEF_OFF;
             break;
    }

    return (status);
}
#endif
