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
* Filename      : bsp_led.c
* Version       : V1.00
* Programmer(s) : JPB
*                 AA
*                 JJL
*                 JM
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDES
*********************************************************************************************************
*/

#include  <bsp_cfg.h>

#if BSP_CFG_LED_EN > 0u
#include  <cpu.h>
#include  <lib_def.h>
#include  <bsp_led.h>
#include  <iodefine.h>


/*
*********************************************************************************************************
*                                       LED INITIALIZATION
*
* Description: This function is used to initialize the LEDs on the board.
*
* Arguments  : none
*********************************************************************************************************
*/

void  BSP_LED_Init (void)
{
    PORTD.PDR.BIT.B0 = 1;                                       /* Select port function.                                */
    PORTD.PDR.BIT.B1 = 1;
    PORTD.PDR.BIT.B2 = 1;
    PORTD.PDR.BIT.B3 = 1;
    PORTD.PDR.BIT.B4 = 1;
    PORTD.PDR.BIT.B5 = 1;
    PORTD.PDR.BIT.B6 = 1;
    PORTD.PDR.BIT.B7 = 1;
    PORTE.PDR.BIT.B0 = 1;
    PORTE.PDR.BIT.B1 = 1;
    PORTE.PDR.BIT.B2 = 1;
    PORTE.PDR.BIT.B3 = 1;

    BSP_LED_Off(0);                                             /* Turn OFF all LEDs.                                   */
}


/*
*********************************************************************************************************
*                                               LED ON
*
* Description: This function is used to control any or all the LEDs on the board.
*
* Arguments  : led    is the number of the LED to control
*                     0    indicates that you want ALL the LEDs to be ON
*                     1    turns ON LED1 on the board
*                     .
*                     .
*                     3    turns ON LED3 on the board
*********************************************************************************************************
*/

void  BSP_LED_On (CPU_INT08U  led)
{
    switch (led) {
        case 0:
            BSP_LED4  = DEF_OFF;
            BSP_LED5  = DEF_OFF;
            BSP_LED6  = DEF_OFF;
            BSP_LED7  = DEF_OFF;
            BSP_LED8  = DEF_OFF;
            BSP_LED9  = DEF_OFF;
            BSP_LED10 = DEF_OFF;
            BSP_LED11 = DEF_OFF;
            BSP_LED12 = DEF_OFF;
            BSP_LED13 = DEF_OFF;
            BSP_LED14 = DEF_OFF;
            BSP_LED15 = DEF_OFF;
            break;

        case 4:
            BSP_LED4  = DEF_OFF;
            break;

        case 5:
            BSP_LED5  = DEF_OFF;
            break;

        case 6:
            BSP_LED6  = DEF_OFF;
            break;

        case 7:
            BSP_LED7  = DEF_OFF;
            break;

        case 8:
            BSP_LED8  = DEF_OFF;
            break;

        case 9:
            BSP_LED9  = DEF_OFF;
            break;

        case 10:
            BSP_LED10 = DEF_OFF;
            break;

        case 11:
            BSP_LED11 = DEF_OFF;
            break;

        case 12:
            BSP_LED12 = DEF_OFF;
            break;

       case 13:
            BSP_LED13 = DEF_OFF;
            break;

       case 14:
            BSP_LED14 = DEF_OFF;
            break;

       case 15:
            BSP_LED15 = DEF_OFF;
            break;

       default:
            break;
    }
}


/*
*********************************************************************************************************
*                                               LED OFF
*
* Description: This function is used to control any or all the LEDs on the board.
*
* Arguments  : led    is the number of the LED to turn OFF
*                     0    indicates that you want ALL the LEDs to be OFF
*                     1    turns OFF LED1 on the board
*                     .
*                     .
*                     3    turns OFF LED3 on the board
*********************************************************************************************************
*/

void  BSP_LED_Off (CPU_INT08U  led)
{
    switch (led) {
        case 0:
            BSP_LED4  = DEF_ON;
            BSP_LED5  = DEF_ON;
            BSP_LED6  = DEF_ON;
            BSP_LED7  = DEF_ON;
            BSP_LED8  = DEF_ON;
            BSP_LED9  = DEF_ON;
            BSP_LED10 = DEF_ON;
            BSP_LED11 = DEF_ON;
            BSP_LED12 = DEF_ON;
            BSP_LED13 = DEF_ON;
            BSP_LED14 = DEF_ON;
            BSP_LED15 = DEF_ON;
            break;

        case 4:
            BSP_LED4  = DEF_ON;
            break;

        case 5:
            BSP_LED5  = DEF_ON;
            break;

        case 6:
            BSP_LED6  = DEF_ON;
            break;

        case 7:
            BSP_LED7  = DEF_ON;
            break;

        case 8:
            BSP_LED8  = DEF_ON;
            break;

        case 9:
            BSP_LED9  = DEF_ON;
            break;

        case 10:
            BSP_LED10 = DEF_ON;
            break;

        case 11:
            BSP_LED11 = DEF_ON;
            break;

        case 12:
            BSP_LED12 = DEF_ON;
            break;

       case 13:
            BSP_LED13 = DEF_ON;
            break;

       case 14:
            BSP_LED14 = DEF_ON;
            break;

       case 15:
            BSP_LED15 = DEF_ON;
            break;

       default:
            break;
    }
}


/*
*********************************************************************************************************
*                                             LED TOGGLE
*
* Description: This function is used to toggle the state of any or all the LEDs on the board.
*
* Arguments  : led    is the number of the LED to toggle
*                     0    indicates that you want ALL the LEDs to toggle
*                     1    Toggle LED1 on the board
*                     .
*                     .
*                     3    Toggle LED3 on the board
*********************************************************************************************************
*/

void  BSP_LED_Toggle (CPU_INT08U  led)
{
    switch (led) {
        case 0:
            BSP_LED4  = ~BSP_LED4;
            BSP_LED5  = ~BSP_LED5;
            BSP_LED6  = ~BSP_LED6;
            BSP_LED7  = ~BSP_LED7;
            BSP_LED8  = ~BSP_LED8;
            BSP_LED9  = ~BSP_LED9;
            BSP_LED10 = ~BSP_LED10;
            BSP_LED11 = ~BSP_LED11;
            BSP_LED12 = ~BSP_LED12;
            BSP_LED13 = ~BSP_LED13;
            BSP_LED14 = ~BSP_LED14;
            BSP_LED15 = ~BSP_LED15;
            break;

        case 4:
            BSP_LED4  = ~BSP_LED4;
            break;

        case 5:
            BSP_LED5  = ~BSP_LED5;
            break;

        case 6:
            BSP_LED6  = ~BSP_LED6;
            break;

        case 7:
            BSP_LED7  = ~BSP_LED7;
            break;

        case 8:
            BSP_LED8  = ~BSP_LED8;
            break;

        case 9:
            BSP_LED9  = ~BSP_LED9;
            break;

        case 10:
            BSP_LED10 = ~BSP_LED10;
            break;

        case 11:
            BSP_LED11 = ~BSP_LED11;
            break;

        case 12:
            BSP_LED12 = ~BSP_LED12;
            break;

       case 13:
            BSP_LED13 = ~BSP_LED13;
            break;

       case 14:
            BSP_LED14 = ~BSP_LED14;
            break;

       case 15:
            BSP_LED15 = ~BSP_LED15;
            break;

       default:
            break;
    }
}
#endif
