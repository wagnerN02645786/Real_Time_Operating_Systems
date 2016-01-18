/*
*********************************************************************************************************
*                                         BOARD SUPPORT PACKAGE
*
*                                 (c) Copyright 2014, Micrium, Weston, FL
*                                          All Rights Reserved
*
* Filename      : bsp_int.h
* Version       : V1.00
* Programmer(s) : JPB
*                 AA
*                 JJL
*                 JM
*********************************************************************************************************
*/

#ifndef  BSP_INT_H_
#define  BSP_INT_H_


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_IntClr     (CPU_INT16U  int_id);
void  BSP_IntDis     (CPU_INT16U  int_id);
void  BSP_IntDisAll  (void);
void  BSP_IntEn      (CPU_INT16U  int_id);

#endif
