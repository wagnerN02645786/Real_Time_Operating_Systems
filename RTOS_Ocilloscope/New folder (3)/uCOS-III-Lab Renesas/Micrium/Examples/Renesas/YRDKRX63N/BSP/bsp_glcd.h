/*
*********************************************************************************************************
*
*                                         BOARD SUPPORT PACKAGE
*
*                                             Renesas RX63N
*                                                on the
*                                       YRDKRX63N Evaluation Board
*
*                                 (c) Copyright 2014, Micrium, Weston, FL
*                                          All Rights Reserved
*
* Filename      : bsp_glcd.h
* Version       : V1.00
* Programmer(s) : FGK
*                 JPB
*                 JM
*********************************************************************************************************
*/

#ifndef  BSP_GLCD_H_
#define  BSP_GLCD_H_


/*
*********************************************************************************************************
*                                              INCLUDES
*********************************************************************************************************
*/

#include  <Glyph.h>
#include  <cpu.h>


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  BSP_GLCD_FONT_SYMBOL   2u
#define  BSP_GLCD_FONT_LARGE    1u
#define  BSP_GLCD_FONT_SMALL    0u


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_GraphLCD_Init        (void);
void  BSP_GraphLCD_Clear       (void);
void  BSP_GraphLCD_SetFont     (       CPU_INT08U   font);
void  BSP_GraphLCD_ClrLine     (       CPU_INT08U   line);
void  BSP_GraphLCD_String      (       CPU_INT08U   line,
                                const  CPU_CHAR    *p_str);
void  BSP_GraphLCD_StringPos   (       CPU_INT08U   line,
                                       CPU_INT08U   pos,
                                const  CPU_CHAR    *p_str);

void  BSP_GraphLCD_StringPosLen(       CPU_INT08U   line,
                                       CPU_INT08U   pos,
                                const  CPU_CHAR    *p_str,
                                       CPU_SIZE_T   len_max);

void  BSP_GraphLCD_ProgBarPos  (       CPU_INT08U   line,
                                       CPU_INT08S   pos);

void  BSP_GraphLCD_CharPixel   (       CPU_INT08U   line,
                                       CPU_INT08U   px,
                                       CPU_CHAR     chr);

#endif
