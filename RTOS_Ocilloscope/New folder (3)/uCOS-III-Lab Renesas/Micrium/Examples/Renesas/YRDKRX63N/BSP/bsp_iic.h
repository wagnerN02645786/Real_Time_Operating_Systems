/*
*********************************************************************************************************
*                                           BOARD SUPPORT PACKAGE
*
*                            (c) Copyright 2013; Micrium, Inc.; Weston, FL
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
* Filename      : bsp_iic.h
* Version       : V1.00
* Programmer(s) : JPB
*                 AA
*                 JJL
*********************************************************************************************************
*/

#ifndef  BSP_IIC_H_
#define  BSP_IIC_H_


/*
*********************************************************************************************************
*                                            PROTOTYPES
*********************************************************************************************************
*/

void  BSP_RIIC0_Init     (void);

void  BSP_RIIC0_MasterRd (CPU_INT08U    addr,
                          CPU_INT08U   *p_data,
                          CPU_SIZE_T    len,
                          CPU_BOOLEAN   start);

void  BSP_RIIC0_MasterWr (CPU_INT08U    addr,
                          CPU_INT08U   *p_data,
                          CPU_SIZE_T    len,
                          CPU_BOOLEAN   stop);

#endif

