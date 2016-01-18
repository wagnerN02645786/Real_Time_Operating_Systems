/*
*********************************************************************************************************
*                                               uC/HTTP
*                                      Hypertext Transfer Protocol
*
*                         (c) Copyright 2004-2015; Micrium, Inc.; Weston, FL
*
*                  All rights reserved.  Protected by international copyright laws.
*
*                  uC/HTTP is provided in source form to registered licensees ONLY.  It is
*                  illegal to distribute this source code to any third party unless you receive
*                  written permission by an authorized Micrium representative.  Knowledge of
*                  the source code may NOT be used to develop a similar product.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available.  Your honesty is greatly appreciated.
*
*                  You can find our product's user manual, API reference, release notes and
*                  more information at: https://doc.micrium.com
*
*                  You can contact us at: http://www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    HTTP SERVER SOCKET MODULE
*
* Filename      : http-s_sock.h
* Version       : V3.00.01
* Programmer(s) : AA
*                 MM
*********************************************************************************************************
* Note(s)       : (1) See http-s.h file header.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This main network protocol suite header file is protected from multiple pre-processor
*               inclusion through use of the HTTPs module present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTPs_SOCK_MODULE_PRESENT                          /* See Note #1.                                         */
#define  HTTPs_SOCK_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <cpu_core.h>

#include  "http-s.h"

/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

NET_SOCK_ID   HTTPsSock_ListenInit   (const  HTTPs_CFG                 *p_cfg,
                                             NET_SOCK_PROTOCOL_FAMILY   family,
                                             HTTPs_ERR                 *p_err);

void          HTTPsSock_ListenClose  (       HTTPs_INSTANCE            *p_instance,
                                             NET_SOCK_ID                sock_listen_id);

NET_SOCK_QTY  HTTPsSock_ConnSel      (       HTTPs_INSTANCE            *p_instance,
                                             CPU_BOOLEAN                accept);

CPU_BOOLEAN   HTTPsSock_ConnDataRx   (       HTTPs_INSTANCE            *p_instance,
                                             HTTPs_CONN                *p_conn);

CPU_BOOLEAN   HTTPsSock_ConnDataTx   (       HTTPs_INSTANCE            *p_instance,
                                             HTTPs_CONN                *p_conn);

void          HTTPsSock_ConnClose    (       HTTPs_INSTANCE            *p_instance,
                                             HTTPs_CONN                *p_conn);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif  /* HTTPs_SOCK_MODULE_PRESENT  */
