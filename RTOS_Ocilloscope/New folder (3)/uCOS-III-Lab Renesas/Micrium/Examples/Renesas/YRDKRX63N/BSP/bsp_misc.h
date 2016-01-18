/*
*********************************************************************************************************
*
*                                         BOARD SUPPORT PACKAGE
*
*                                 (c) Copyright 2014, Micrium, Weston, FL
*                                          All Rights Reserved
*
* Filename      : bsp.h
* Version       : V1.00
* Programmer(s) : JPB
*                 AA
*                 JM
*********************************************************************************************************
*/

#ifndef  BSP_MISC_H_
#define  BSP_MISC_H_

/*
*********************************************************************************************************
*                                         MISCELLANEOUS SERVICES
*********************************************************************************************************
*/

void         BSP_MiscInit  (void);

void         BSP_ADC_Init  (void);

CPU_INT16U   BSP_ADC_Rd    (void);

CPU_INT16U   BSP_MicRd     (void);

CPU_BOOLEAN  BSP_SwsRd     (CPU_INT08U  sw_nbr);

#endif
