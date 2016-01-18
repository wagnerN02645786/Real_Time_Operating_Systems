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
*               written permission by an authorized Micrium representative. Knowledge of
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
*
*                                         BOARD SUPPORT PACKAGE
*
* Filename      : bsp_int.c
* Version       : V1.00.00
* Programmer(s) : FF
*                 OD
*                 MD
*                 JM
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <bsp_cfg.h>
#include  <cpu.h>
#include  <bsp_int.h>


/*
*********************************************************************************************************
*                                             BSP_IntClr()
*
* Description : This function clears an interrupt
*
* Argument(s) : int_id        The interrupt ID
*                             BSP_INT_ID_WDTI
*                             BSP_INT_ID_LVI
*                             BSP_INT_ID_P0
*                                   .
*                                   .
*                             BSP_INT_ID_FL
* Returns     : none.
*
* Caller(s)   : Application.
*
* Note(s)     : This function clears interrupts by setting a 0 to the Interrupt
*               Request Flag Register.
*********************************************************************************************************
*/

void  BSP_IntClr (CPU_INT16U  int_id)
{
    if (int_id > BSP_CFG_INT_VECT_TBL_SIZE) {
        return;
    }
}


/*
*********************************************************************************************************
*                                               BSP_IntDis()
*
* Description : This function disables an interrupt.
*
* Argument(s) : int_id        The interrupt ID
*                             BSP_INT_ID_WDTI
*                             BSP_INT_ID_LVI
*                             BSP_INT_ID_P0
*                                   .
*                                   .
*                             BSP_INT_ID_FL
* Returns     : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Disable the specified interrupt by writing a 1 to the Interrupt
*               Mask Flag Register.
*********************************************************************************************************
*/

void  BSP_IntDis (CPU_INT16U  int_id)
{
                                                                /* -------------- ARGUMENTS CHECKING ---------------- */
    if (int_id > BSP_CFG_INT_VECT_TBL_SIZE) {
        return;
    }
}


/*
*********************************************************************************************************
*                                           BSP_IntDisAll()
*
* Description : Disable ALL interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{

}


/*
*********************************************************************************************************
*                                          BSP_IntEn()
*
* Description : This function enables an interrupt.
*
* Argument(s) : int_id        The interrupt ID
*                             BSP_INT_ID_WDTI
*                             BSP_INT_ID_LVI
*                             BSP_INT_ID_P0
*                                   .
*                                   .
*                             BSP_INT_ID_FL
* Returns     : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Enable interrupt by setting a 1 to the Interrupt Mask
*               Flag Register.
*********************************************************************************************************
*/

void  BSP_IntEn (CPU_INT16U  int_id)
{
    if (int_id > BSP_CFG_INT_VECT_TBL_SIZE) {
        return;
    }
}
