/*
*********************************************************************************************************
*                                                Renesas
*                                         Board Support Package
*                                               RDKRX63N
*
*                             (c) Copyright 2014, Micrium, Weston, FL
*                                           All Rights Reserved
*
*
* File         : BSP_OS.C
* Originally by: Jean J. Labrosse
*
* RDKRX62N Expansion Board modification by Ian Hall
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include <bsp_os.h>


/*
*********************************************************************************************************
*                                           LOCAL VARIABLES
*********************************************************************************************************
*/

static  OS_MUTEX  RIIC0_Mutex;
static  OS_MUTEX  RSPI0_Mutex;
static  OS_MUTEX  GLCD_Mutex;


/*
*********************************************************************************************************
*                                     BSP_OS_RIIC0_MutexCreate()
*
* Description : Creates a Mutex used to protect access at a low level to the RIIC ch 0 interface.
*               Allows RIIC0 connected peripherals to be accessed by multiple tasks.
*
* Argument(s) : none.
*
* Return(s)   : DEF_OK,   if successful.
*               DEF_FAIL, otherwise.
*
* Caller(s)   : BSP RIIC0 initialization.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_RIIC0_MutexCreate (void)
{
    OS_ERR  err;


    OSMutexCreate(&RIIC0_Mutex,
                  "RIIC0 Mutex",
                  &err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                      BSP_OS_RIIC0_MutexPend()
*
* Description : Obtain mutex. Block if the mutex is not available.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP RIIC0 read/write code.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_OS_RIIC0_MutexPend (void)
{
    OS_ERR  err;
    CPU_TS  ts;


    OSMutexPend(&RIIC0_Mutex,
                 0u,                                            /* no timeout                         */
                 OS_OPT_PEND_BLOCKING,                          /* block if not available             */
                &ts,                                            /* timestamp                          */
                &err);
}


/*
*********************************************************************************************************
*                                      BSP_OS_RIIC0_MutexPost()
*
* Description : Return ownership of mutex back to the RTOS.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP RIIC0 read/write code.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_OS_RIIC0_MutexPost (void)
{
    OS_ERR  err;


    OSMutexPost(&RIIC0_Mutex,
                 OS_OPT_POST_NONE,
                &err);
}


/*
*********************************************************************************************************
*                                     BSP_OS_RSPI0_MutexCreate()
*
* Description : Creates a Mutex used to protect access at a low level to the RSPI ch 0 interface.
*               Allows RSPI0 connected peripherals to be accessed by multiple tasks.
*
* Argument(s) : none.
*
* Return(s)   : DEF_OK,   if successful.
*               DEF_FAIL, otherwise.
*
* Caller(s)   : BSP RSPI0 initialization.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_RSPI0_MutexCreate (void)
{
    OS_ERR  err;


    OSMutexCreate(&RSPI0_Mutex,
                  "RSPI0 Mutex",
                  &err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                      BSP_OS_RSPI0_MutexPend()
*
* Description : Obtain mutex. Block if the mutex is not available.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP RSPI0 read/write code.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_OS_RSPI0_MutexPend (void)
{
    OS_ERR  err;
    CPU_TS  ts;


    OSMutexPend(&RSPI0_Mutex,
                 0u,                                            /* no timeout                         */
                 OS_OPT_PEND_BLOCKING,                          /* block if not available             */
                &ts,                                            /* timestamp                          */
                &err);
}


/*
*********************************************************************************************************
*                                      BSP_OS_RSPI0_MutexPost()
*
* Description : Return ownership of mutex back to the RTOS.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP RSPI0 read/write code.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_OS_RSPI0_MutexPost (void)
{
    OS_ERR  err;


    OSMutexPost(&RSPI0_Mutex,
                 OS_OPT_POST_NONE,
                &err);
}


/*
*********************************************************************************************************
*                                      BSP_OS_GLCD_MutexCreate()
*
* Description : Creates a Mutex used to protect access at a low level to the graph LCD. Allows graph LCD
*               to be accessed by multiple tasks.
*
* Argument(s) : none.
*
* Return(s)   : DEF_OK,   if successful.
*               DEF_FAIL, otherwise.
*
* Caller(s)   : BSP graph LCD initialization.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_GLCD_MutexCreate (void)
{
    OS_ERR  err;


    OSMutexCreate(&GLCD_Mutex,
                  "GLCD Mutex",
                  &err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                       BSP_OS_GLCD_MutexPend()
*
* Description : Obtain mutex. Block if the mutex is not available.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP graph LCD read/write code.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_OS_GLCD_MutexPend (void)
{
    OS_ERR  err;
    CPU_TS  ts;


    OSMutexPend(&GLCD_Mutex,
                 0u,                                            /* no timeout                         */
                 OS_OPT_PEND_BLOCKING,                          /* block if not available             */
                &ts,                                            /* timestamp                          */
                &err);
}


/*
*********************************************************************************************************
*                                       BSP_OS_GLCD_MutexPost()
*
* Description : Return ownership of mutex back to the RTOS.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP graph LCD read/write code.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_OS_GLCD_MutexPost (void)
{
    OS_ERR  err;


    OSMutexPost(&GLCD_Mutex,
                 OS_OPT_POST_NONE,
                &err);
}


/*
*********************************************************************************************************
*                                      BSP_OS_SemCreate()
*
* Description : Creates a sempahore to lock/unlock
*
* Argument(s) : p_sem        Pointer to a BSP_OS_SEM structure
*
*               sem_val      Initial value of the semaphore.
*
*               p_sem_name   Pointer to the semaphore name.
*
* Return(s)   : DEF_OK       if the semaphore was created.
*               DEF_FAIL     if the sempahore could not be created.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemCreate (BSP_OS_SEM       *p_sem,
                               BSP_OS_SEM_VAL    sem_val,
                               CPU_CHAR         *p_sem_name)
{
    OS_ERR     err;


    OSSemCreate((OS_SEM    *)p_sem,
                (CPU_CHAR  *)p_sem_name,
                (OS_SEM_CTR )sem_val,
                (OS_ERR    *)&err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                     BSP_OS_SemWait()
*
* Description : Wait on a semaphore to become available
*
* Argument(s) : sem          sempahore handler
*
*               dly_ms       delay in miliseconds to wait on the semaphore
*
* Return(s)   : DEF_OK       if the semaphore was acquire
*               DEF_FAIL     if the sempahore could not be acquire
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemWait (BSP_OS_SEM  *p_sem,
                             CPU_INT32U   dly_ms)
{
    OS_ERR      err;
    CPU_INT32U  dly_ticks;


    dly_ticks  = ((dly_ms * DEF_TIME_NBR_mS_PER_SEC) / OSCfg_TickRate_Hz);

    OSSemPend((OS_SEM *)p_sem,
              (OS_TICK )dly_ticks,
              (OS_OPT  )OS_OPT_PEND_BLOCKING,
              (CPU_TS  )0,
              (OS_ERR *)&err);

    if (err != OS_ERR_NONE) {
       return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                      BSP_OS_SemPost()
*
* Description : Post a semaphore
*
* Argument(s) : sem          Semaphore handler
*
* Return(s)   : DEF_OK       if the semaphore was posted.
*               DEF_FAIL     if the sempahore could not be posted.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemPost (BSP_OS_SEM *p_sem)
{
    OS_ERR  err;


    OSSemPost((OS_SEM *)p_sem,
              (OS_OPT  )OS_OPT_POST_1,
              (OS_ERR *)&err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}
