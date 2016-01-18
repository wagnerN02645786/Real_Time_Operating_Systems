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
* Filename      : bsp_sys.c
* Version       : V1.00
* Programmer(s) : JPB
*                 AA
*                 JJL
*                 JM
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDES
*********************************************************************************************************
*/

#include  <bsp_cfg.h>

#include  <cpu.h>
#include  <cpu_core.h>
#include  <lib_def.h>
#include  <bsp_sys.h>

#include  <iodefine.h>


/*
*********************************************************************************************************
*                                         BSP SYSTEM INITIALIZATION
*
* Description: This function should be called early in the BSP initialization process.
*
* Arguments  : none
*********************************************************************************************************
*/

void  BSP_SysInit (void)
{
    CPU_INT32U  i;


    SYSTEM.PRCR.WORD     = 0xA50B;                              /* protect off                                          */
    SYSTEM.SOSCCR.BYTE   =   0x01;                              /* Dis Sub-Clock oscillation                            */
    SYSTEM.MOSCWTCR.BYTE =   0x0D;                              /* 131072 state                                         */
                                                                /* wait over 10ms  @12.5MHz                             */
    SYSTEM.PLLWTCR.BYTE  =   0x0F;                              /* 4194304 state (default)                              */
                                                                /* wait over 12ms  @PLL=200MHz(12.5MHz*16)              */

                                                                /* ----------------- PLL CONFIGURATION ---------------- */
                                                                /* ... PLIDIV = 1                                       */
                                                                /* ... STC    = 16                                      */
                                                                /* ... PLL    = (XTAL_FREQ * PLIDIV) * STC              */
                                                                /* ...        = (12000000 * 1) * 16                     */
                                                                /* ...        = 192Mhz                                  */
    SYSTEM.PLLCR.WORD    = 0x0F00;
    SYSTEM.MOSCCR.BYTE   =   0x00;                              /* En main clock oscillation                            */
    SYSTEM.PLLCR2.BYTE   =   0x00;                              /* En PLL oscillation                                   */

    for (i = 0; i < 0x168; i++) {                               /* wait over 12ms                                       */
    }
                                                                /* ---------------- SYSTEM CONFIGURATION -------------- */
                                                                /* ... ICK  = PLL * 1/2 = 96Mhz                         */
                                                                /* ... PCKB = PLL * 1/2 = 48Mhz                         */
                                                                /* ... BCK  = PLL * 1/4 = 24Mhz                         */
                                                                /* ... FCK  = PLL * 1/4 = 24Mhz                         */
    SYSTEM.SCKCR.LONG    = 0x21031211;
    SYSTEM.SCKCR2.WORD   =     0x0031;                          /* ... UCK  = PLL * 1/4 = 48Mhz                         */
    SYSTEM.SCKCR3.WORD   =     0x0400;                          /* Switch to PLL clock                                  */
}


/*
*********************************************************************************************************
*                                     PERIPHERAL CLOCK FREQUENCY
*
* Description: This function is used to retrieve peripheral clock frequency.
*
* Arguments  : none
*
* Return     : Peripheral clock frequency in cycles.
*********************************************************************************************************
*/

CPU_INT32U  BSP_SysPerClkFreqGet (void)
{
    CPU_INT32U  pll_stc;
    CPU_INT32U  pll_div;
    CPU_INT32U  pll_freq;
    CPU_INT32U  per_div;
    CPU_INT32U  per_clk;


    pll_stc = SYSTEM.PLLCR.BIT.STC;
    pll_stc++;

    pll_div = SYSTEM.PLLCR.BIT.PLIDIV;
    switch (pll_div) {
        case 0:
             pll_div = 1;
             break;

        case 1:
             pll_div = 2;
             break;

        case 2:
             pll_div = 4;
             break;

        case 3:
        default:
             return (0);

    }

    pll_freq = BSP_CFG_SYS_EXT_CLK_FREQ * pll_stc / pll_div;

    per_div  = SYSTEM.SCKCR.BIT.PCKB;
    switch (per_div) {
        case 0:
            per_div = 1;
            break;

        case 1:
            per_div = 2;
            break;

        case 2:
            per_div = 4;
            break;

        case 3:
            per_div = 8;
            break;

        case 4:
            per_div = 16;
            break;

        case 5:
            per_div = 32;
            break;

        case 6:
            per_div = 64;
            break;

        default:
            return (0);
    }

    per_clk  = pll_freq / per_div;

    return (per_clk);
}
