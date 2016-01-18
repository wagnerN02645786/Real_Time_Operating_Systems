/*
*********************************************************************************************************
*                                             uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                         (c) Copyright 2004-2015; Micrium, Inc.; Weston, FL
*
*                  All rights reserved.  Protected by international copyright laws.
*
*                  uC/TCP-IP is provided in source form to registered licensees ONLY.  It is
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
*                                       NETWORK IP GENERIC LAYER
*                                         (INTERNET PROTOCOL)
*
* Filename      : net_ip.c
* Version       : V3.03.01
* Programmer(s) : ITJ
*                 SL
*********************************************************************************************************
* Note(s)       : (1) This module is responsible to initialize different IP version enabled.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#define    MICRIUM_SOURCE
#define    NET_IP_MODULE
#include  "net_ip.h"
#include  "net_cfg_net.h"

#ifdef  NET_IPv4_MODULE_EN
#include  "../IP/IPv4/net_ipv4.h"
#endif
#ifdef  NET_IPv6_MODULE_EN
#include  "../IP/IPv6/net_ipv6.h"
#endif


/*
*********************************************************************************************************
*                                             NetIP_Init()
*
* Description : Initialize IP modules.
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Net_Init().
*
*               This function is an INTERNAL network protocol suite function & MUST NOT be called by
*               application function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  NetIP_Init (NET_ERR  *p_err)
{
#ifdef  NET_IPv4_MODULE_EN
    NetIPv4_Init();
   *p_err = NET_ERR_NONE;
#endif

#ifdef  NET_IPv6_MODULE_EN
    NetIPv6_Init(p_err);
    if (*p_err == NET_IPv6_ERR_NONE) {
        *p_err = NET_ERR_NONE;
    }
#endif
}



/*
*********************************************************************************************************
*                                      NetIP_IF_Init()
*
* Description : Initialize IP objects for given Interface.
*
* Argument(s) : if_nbr      Network Interface number to initialize.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               NET_ERR_NONE
*
*                               ----------- RETURNED BY NetIF_LinkStateSubscribeHandler() : ------------
*                               See NetIF_LinkStateSubscribeHandler() for additional return error codes.
*
* Return(s)   : None.
*
* Caller(s)   : NetIF_Add().
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  NetIP_IF_Init (NET_IF_NBR   if_nbr,
                     NET_ERR     *p_err)
{

#ifdef  NET_IPv4_MODULE_EN
   *p_err = NET_ERR_NONE;
#endif

#ifdef  NET_IPv6_MODULE_EN
                                                                /* --------- INIT IPv6 LINK CHANGE SUBSCRIBE ---------- */
    NetIF_LinkStateSubscribeHandler(if_nbr,
                                   &NetIPv6_LinkStateSubscriber,
                                    p_err);
    switch (*p_err) {
        case NET_IF_ERR_NONE:
            *p_err = NET_ERR_NONE;
             break;


        default:
            return;
    }
#endif

    (void)&if_nbr;
}
