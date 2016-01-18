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

#ifndef  BSP_OS_MODULE_PRESENT
#define  BSP_OS_MODULE_PRESENT

#include  <os.h>


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef   OS_SEM       BSP_OS_SEM;
typedef   OS_SEM_CTR   BSP_OS_SEM_VAL;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_RIIC0_MutexCreate(void);
void         BSP_OS_RIIC0_MutexPend  (void);
void         BSP_OS_RIIC0_MutexPost  (void);

CPU_BOOLEAN  BSP_OS_RSPI0_MutexCreate(void);
void         BSP_OS_RSPI0_MutexPend  (void);
void         BSP_OS_RSPI0_MutexPost  (void);

CPU_BOOLEAN  BSP_OS_GLCD_MutexCreate (void);
void         BSP_OS_GLCD_MutexPend   (void);
void         BSP_OS_GLCD_MutexPost   (void);

CPU_BOOLEAN  BSP_OS_SemCreate        (BSP_OS_SEM     *p_sem,
                                      BSP_OS_SEM_VAL  sem_val,
                                      CPU_CHAR       *p_sem_name);

CPU_BOOLEAN  BSP_OS_SemWait          (BSP_OS_SEM     *p_sem,
                                      CPU_INT32U      dly_ms);

CPU_BOOLEAN  BSP_OS_SemPost          (BSP_OS_SEM     *p_sem);

#endif
