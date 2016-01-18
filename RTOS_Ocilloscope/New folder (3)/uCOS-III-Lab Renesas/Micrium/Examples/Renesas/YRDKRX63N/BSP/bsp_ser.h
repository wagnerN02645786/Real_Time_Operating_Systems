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

#ifndef  BSP_SER_H_
#define  BSP_SER_H_


/*
*********************************************************************************************************
*                                             RS-232 SERVICES
*********************************************************************************************************
*/

void         BSP_Ser_Init            (CPU_INT32U  baud_rate);

CPU_INT08U   BSP_Ser_RdByte          (void);
void         BSP_Ser_RdStr           (CPU_CHAR    *p_str,
                                      CPU_INT16U   len);

void         BSP_Ser_WrByte          (CPU_INT08U   c);
void         BSP_Ser_WrStr           (CPU_CHAR     *p_str);

void         BSP_Ser_Printf          (CPU_CHAR     *format,
                                      ...);

CPU_ISR      BSP_Ser_ISR_Rx_Handler  (void);
CPU_ISR      BSP_Ser_ISR_Tx_Handler  (void);

#endif
