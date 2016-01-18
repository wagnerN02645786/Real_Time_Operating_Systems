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
* Filename      : bsp.c
* Version       : V1.01
* Programmer(s) : JPB
*                 AA
*                 JJL
*                 JM
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               INCLUDES
*********************************************************************************************************
*/

#include  "bsp_cfg.h"

#include  <cpu.h>
#include  <os.h>

#include  <bsp_sys.h>

#if BSP_CFG_LED_EN         > 0u
#include  <bsp_led.h>
#endif

#if BSP_CFG_CAN_EN         > 0u
#include  <bsp_can.h>
#endif

#if  BSP_CFG_FS_EN       > 0u
#endif


#if BSP_CFG_GRAPH_LCD_EN   > 0u
#include  <bsp_glcd.h>
#endif

#if  BSP_CFG_GUI_EN      > 0u
#endif


#if  BSP_CFG_MBM_EN      > 0u
#endif


#if  BSP_CFG_MBS_EN      > 0u
#endif


#if  BSP_CFG_NET_EN      > 0u
#endif


#if  BSP_CFG_OS2_EN      > 0u
#endif


#if  BSP_CFG_OS3_EN      > 0u
#endif


#if  BSP_CFG_USBD_EN     > 0u
#endif


#if  BSP_CFG_USBH_EN     > 0u
#endif


#if BSP_CFG_MISC_EN      > 0u
#include  <bsp_misc.h>
#endif

#include  <iodefine.h>


/*$PAGE*/
/*
*********************************************************************************************************
*                                         BSP INITIALIZATION
*
* Description: This function should be called by the application code before using any functions in this
*              module.
*
* Arguments  : none
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    BSP_SysInit();                                              /* Initialize system services, clocks, etc.             */

    OS_BSP_TickInit();                                          /* Init uC/OS-xx periodic time source                   */

#if BSP_CFG_LED_EN       > 0u
    BSP_LED_Init();                                             /* Initialize LEDs                                      */
#endif


#if BSP_CFG_GRAPH_LCD_EN > 0u
    BSP_GraphLCD_Init();                                        /* Initialize the on-board graphics LCD display         */
#endif


#if  BSP_CFG_CAN_EN      > 0u
#endif


#if  BSP_CFG_CAN_OPEN_EN > 0u
#endif


#if  BSP_CFG_FS_EN       > 0u
#endif


#if  BSP_CFG_GUI_EN      > 0u
#endif


#if  BSP_CFG_MBM_EN      > 0u
#endif


#if  BSP_CFG_MBS_EN      > 0u
#endif


#if  BSP_CFG_NET_EN      > 0u
#endif


#if  BSP_CFG_OS2_EN      > 0u
#endif


#if  BSP_CFG_OS3_EN      > 0u
#endif


#if  BSP_CFG_USBD_EN     > 0u
                                                                /* ---------------- USB PIN CONFIGURATION ------------- */
    PORT2.PDR.BIT.B3   = 0;                                     /* Clear USB0_DPUPE pin                                 */
    PORT2.PMR.BIT.B3   = 0;

    MPC.PWPR.BIT.B0WI  = 0;                                     /* En writing to PFS registers                          */
    MPC.PWPR.BIT.PFSWE = 1;
    MPC.P23PFS.BYTE    = 0x13;                                  /* Select USB0_DPUPE as peripheral function             */
    MPC.PWPR.BIT.PFSWE = 0;                                     /* Dis writing to PFS registers                         */
    MPC.PWPR.BIT.B0WI  = 1;

    PORT2.PMR.BIT.B3   = 1;                                     /* Set pin as I/O port for peripheral function          */
#endif


#if  BSP_CFG_USBH_EN     > 0u
#endif


#if BSP_CFG_MISC_EN      > 0u
    BSP_MiscInit();                                             /* Initialize miscellaneous services                    */
#endif
}

