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

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                            Renesas RX63N
*                                                on the
*                                      YRDKRX63N Evaluation Board
*
* Filename      : bsp_glcd.c
* Version       : V1.00
* Programmer(s) : FGK
*                 JPB
*                 JM
*********************************************************************************************************
*/

#include  <bsp.h>
#include  <bsp_glcd.h>
#include  <bsp_os.h>
#include  <iodefine.h>
#include  <lib_def.h>
#include  <lib_str.h>


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

static  T_glyphHandle  BSP_GraphLCD;


/*
*********************************************************************************************************
*                                      GRAPH LCD INITIALIZATION
*********************************************************************************************************
*/

void  BSP_GraphLCD_Init (void)
{
    CPU_BOOLEAN   init;
    T_glyphError  err;


    init = BSP_OS_GLCD_MutexCreate();
    if (init != DEF_OK) {
        return;
    }

    BSP_OS_GLCD_MutexPend();
    err = GlyphOpen(&BSP_GraphLCD, 0);

    if (err == GLYPH_ERROR_NONE) {
        GlyphNormalScreen(BSP_GraphLCD);
        GlyphClearScreen(BSP_GraphLCD);
    }
    BSP_OS_GLCD_MutexPost();
}


/*
*********************************************************************************************************
*                                       GRAPH LCD CLEAR SCREEN
*********************************************************************************************************
*/

void  BSP_GraphLCD_Clear (void)
{
    BSP_OS_GLCD_MutexPend();
    GlyphClearScreen(BSP_GraphLCD);
    BSP_OS_GLCD_MutexPost();
}


/*
*********************************************************************************************************
*                                         GRAPH LCD SET FONT
*********************************************************************************************************
*/

void  BSP_GraphLCD_SetFont (CPU_INT08U  font)
{
    BSP_OS_GLCD_MutexPend();
    switch (font) {
        case BSP_GLCD_FONT_LARGE:
             GlyphSetFont(BSP_GraphLCD, GLYPH_FONT_8_BY_8);
             break;

        case BSP_GLCD_FONT_SMALL:
             GlyphSetFont(BSP_GraphLCD, GLYPH_FONT_5_BY_7);
             break;
    }
    BSP_OS_GLCD_MutexPost();
}


/*
*********************************************************************************************************
*                                       GRAPH LCD PRINT STRING
*********************************************************************************************************
*/

void  BSP_GraphLCD_String (       CPU_INT08U   line,
                           const  CPU_CHAR    *p_str)
{
    CPU_INT08U  y;


    y = line * 9u;

    BSP_OS_GLCD_MutexPend();
    GlyphSetXY (BSP_GraphLCD, 0u, y);
    GlyphString(BSP_GraphLCD, (uint8_t *)p_str, Str_Len(p_str));
    BSP_OS_GLCD_MutexPost();
}


/*
*********************************************************************************************************
*                                        GRAPH LCD CLEAR LINE
*********************************************************************************************************
*/

void  BSP_GraphLCD_ClrLine (CPU_INT08U  line)
{
    CPU_INT08U  y;


    y = line * 9u;

    BSP_OS_GLCD_MutexPend();
    GlyphEraseBlock(BSP_GraphLCD, 0u, y, 95u, y + 7u);
    BSP_OS_GLCD_MutexPost();
}


/*
*********************************************************************************************************
*                                 GRAPH LCD PRINT STRING AT POSITION
*********************************************************************************************************
*/

void  BSP_GraphLCD_StringPos (       CPU_INT08U   line,
                                     CPU_INT08U   pos,
                              const  CPU_CHAR    *p_str)
{
    CPU_INT08U  x;
    CPU_INT08U  y;


    x = pos  * 5u;
    y = line * 9u;

    BSP_OS_GLCD_MutexPend();

    GlyphSetXY (BSP_GraphLCD, x, y);
    GlyphString(BSP_GraphLCD, (uint8_t *)p_str, Str_Len(p_str));

    BSP_OS_GLCD_MutexPost();
}


/*
*********************************************************************************************************
*                                 GRAPH LCD PRINT PROGRESS BAR AT POSITION
*********************************************************************************************************
*/

void  BSP_GraphLCD_ProgBarPos (CPU_INT08U  line,
                               CPU_INT08S  pos)
{
    T_glyphFont  font;
    CPU_INT08U   x;
    CPU_INT08U   y;
    CPU_CHAR     chr;


    y =  line *  9u;
    x = (pos  * 86) / 100 + 2;

    BSP_OS_GLCD_MutexPend();
    GlyphGetFont(BSP_GraphLCD, &font);

    chr = 6u;
    GlyphSetXY (BSP_GraphLCD, 0, y);
    GlyphString(BSP_GraphLCD, (uint8_t *)&chr, 1u);
    if (pos >= 0) {
        chr = 7u;
        GlyphSetXY (BSP_GraphLCD, x, y);
        GlyphString(BSP_GraphLCD, (uint8_t *)&chr, 1u);
    }

    GlyphSetFont(BSP_GraphLCD, font);
    BSP_OS_GLCD_MutexPost();
}


/*
*********************************************************************************************************
*                           GRAPH LCD PRINT STRING AT POSITION WITH MAX LEN
*********************************************************************************************************
*/

void  BSP_GraphLCD_StringPosLen (       CPU_INT08U   line,
                                        CPU_INT08U   pos,
                                 const  CPU_CHAR    *p_str,
                                        CPU_SIZE_T   len_max)
{
    CPU_INT08U  x;
    CPU_INT08U  y;
    CPU_SIZE_T  len;


    x = pos  * 5u;
    y = line * 9u;

    len = DEF_MIN(Str_Len_N(p_str, len_max + 1u), len_max);

    BSP_OS_GLCD_MutexPend();
    GlyphSetXY (BSP_GraphLCD, x, y);
    GlyphString(BSP_GraphLCD, (uint8_t *)p_str, len);
    BSP_OS_GLCD_MutexPost();
}


/*
*********************************************************************************************************
*                             GRAPH LCD PRINT CHARACTER AT PIXEL POSITION
*********************************************************************************************************
*/

void  BSP_GraphLCD_CharPixel (CPU_INT08U  line,
                              CPU_INT08U  px,
                              CPU_CHAR    chr)
{
    CPU_INT08U  y;


    y = line * 9u;

    BSP_OS_GLCD_MutexPend();
    GlyphSetXY (BSP_GraphLCD, px, y);
    GlyphString(BSP_GraphLCD, (uint8_t *)&chr, 1);
    BSP_OS_GLCD_MutexPost();
}

void  BSP_GraphLCD_String1(uint8_t char_1[], uint32_t aX ,uint32_t aY) 
 {
   change(char_1);
        GlyphSetXY(BSP_GraphLCD, aX, aY);
	GlyphChar(BSP_GraphLCD, '~');
 }
