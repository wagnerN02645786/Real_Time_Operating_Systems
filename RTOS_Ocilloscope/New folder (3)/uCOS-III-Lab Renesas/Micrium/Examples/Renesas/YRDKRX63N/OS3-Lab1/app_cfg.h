/*
*********************************************************************************************************
*                                            
*                                       APPLICATION CONFIGURATION
*
*                             (c) Copyright 2014, Micrium, Inc., Weston, FL
*                                          All Rights Reserved
*
*                                            Renesas RX63N
*                                               on the
*                                              YRDKRX63N
*                                          Evaluation Board
*
*
* File : app_cfg.h
* By   : JPB
*        JM
*********************************************************************************************************
*/

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT

#include  <cpu.h>


/*
*********************************************************************************************************
*                                            START TASK
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_PRIO                        3u
#define  APP_CFG_TASK_START_STK_SIZE                  512u


/*
*********************************************************************************************************
*                                             TRACING
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                                0u
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                               1u
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                                2u
#endif

void         BSP_Ser_Printf          (CPU_CHAR     *format,
                                      ...);
#define  APP_CFG_TRACE_LEVEL                          TRACE_LEVEL_OFF
#define  APP_CFG_TRACE                                BSP_Ser_Printf

#define  APP_TRACE_INFO(x)    ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_INFO) ? (void)(APP_CFG_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)     ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_DBG)  ? (void)(APP_CFG_TRACE x) : (void)0)

#endif
