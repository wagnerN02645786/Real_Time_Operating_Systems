/*
*********************************************************************************************************
*                                           BOARD SUPPORT PACKAGE
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
* Filename      : bsp_led.h
* Version       : V1.00
* Programmer(s) : JPB
*                 AA
*                 JJL
*                 JM
*********************************************************************************************************
*/

#ifndef  BSP_LED_H_
#define  BSP_LED_H_


/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define  BSP_LED4                   PORTD.PODR.BIT.B5
#define  BSP_LED5                   PORTE.PODR.BIT.B3
#define  BSP_LED6                   PORTD.PODR.BIT.B2
#define  BSP_LED7                   PORTE.PODR.BIT.B0
#define  BSP_LED8                   PORTD.PODR.BIT.B4
#define  BSP_LED9                   PORTE.PODR.BIT.B2
#define  BSP_LED10                  PORTD.PODR.BIT.B1
#define  BSP_LED11                  PORTD.PODR.BIT.B7
#define  BSP_LED12                  PORTD.PODR.BIT.B3
#define  BSP_LED13                  PORTE.PODR.BIT.B1
#define  BSP_LED14                  PORTD.PODR.BIT.B0
#define  BSP_LED15                  PORTD.PODR.BIT.B6


/*
*********************************************************************************************************
*                                            PROTOTYPES
*********************************************************************************************************
*/

void    BSP_LED_Init    (void);
void    BSP_LED_On      (CPU_INT08U  led);
void    BSP_LED_Off     (CPU_INT08U  led);
void    BSP_LED_Toggle  (CPU_INT08U  led);

#endif
