/*
*********************************************************************************************************
*                                         BOARD SUPPORT PACKAGE
*
*                                 (c) Copyright 2014, Micrium, Weston, FL
*                                          All Rights Reserved
*
* Filename      : bsp_int_vect_tbl.c
* Version       : V1.01
* Programmer(s) : JPB
*                 AA
*                 JJL
*                 JM
*********************************************************************************************************
*/

#include  <bsp_cfg.h>

#include  <cpu.h>
#include  <os.h>

#include  <cpu_core.h>
#include  <lib_def.h>
#include  <bsp_int_vect_tbl.h>

#if BSP_CFG_NET_EN > 0u
#include  <net_bsp.h>
#endif

#if BSP_CFG_SER_EN > 0u
#include  <bsp_ser.h>
#endif

#if BSP_CFG_USBD_EN > 0u
#include  <Source\usbd_core.h>
#include  <usbd_bsp_yrdkrx63n.h>
#endif

#if BSP_CFG_USBH_EN > 0u
#include  <usbh_bsp_rx600.h>
#endif


/*
*********************************************************************************************************
*                                       INTERRUPT VECTOR TABLE
*
* Note(s): 1) Could be in RAM if BSP_CFG_INT_VECT_RAM_EN is set to 1 in 'bsp_cfg.h'
*          2) In either case BSP_IntVectSet() must becalled to boint to BSP_IntVectTbl[]
*
*********************************************************************************************************
*/

#if     BSP_CFG_INT_VECT_TBL_RAM_EN > 0u
static  CPU_FNCT_VOID  BSP_IntVectTbl[] =
#else
const   CPU_FNCT_VOID  BSP_IntVectTbl[] =
#endif
{
    (CPU_FNCT_VOID)BSP_IntHandler_000,              /*  00 */
    (CPU_FNCT_VOID)BSP_IntHandler_001,              /*  01 */
    (CPU_FNCT_VOID)BSP_IntHandler_002,              /*  02 */
    (CPU_FNCT_VOID)BSP_IntHandler_003,              /*  03 */
    (CPU_FNCT_VOID)BSP_IntHandler_004,              /*  04 */
    (CPU_FNCT_VOID)BSP_IntHandler_005,              /*  05 */
    (CPU_FNCT_VOID)BSP_IntHandler_006,              /*  06 */
    (CPU_FNCT_VOID)BSP_IntHandler_007,              /*  07 */
    (CPU_FNCT_VOID)BSP_IntHandler_008,              /*  08 */
    (CPU_FNCT_VOID)BSP_IntHandler_009,              /*  09 */

    (CPU_FNCT_VOID)BSP_IntHandler_010,              /*  10 */
    (CPU_FNCT_VOID)BSP_IntHandler_011,              /*  11 */
    (CPU_FNCT_VOID)BSP_IntHandler_012,              /*  12 */
    (CPU_FNCT_VOID)BSP_IntHandler_013,              /*  13 */
    (CPU_FNCT_VOID)BSP_IntHandler_014,              /*  14 */
    (CPU_FNCT_VOID)BSP_IntHandler_015,              /*  15 */
    (CPU_FNCT_VOID)BSP_IntHandler_016,              /*  16 */
    (CPU_FNCT_VOID)BSP_IntHandler_017,              /*  17 */
    (CPU_FNCT_VOID)BSP_IntHandler_018,              /*  18 */
    (CPU_FNCT_VOID)BSP_IntHandler_019,              /*  19 */

    (CPU_FNCT_VOID)BSP_IntHandler_020,              /*  20 */
    (CPU_FNCT_VOID)BSP_IntHandler_021,              /*  21 */
    (CPU_FNCT_VOID)BSP_IntHandler_022,              /*  22 */
    (CPU_FNCT_VOID)BSP_IntHandler_023,              /*  23 */
    (CPU_FNCT_VOID)BSP_IntHandler_024,              /*  24 */
    (CPU_FNCT_VOID)BSP_IntHandler_025,              /*  25 */
    (CPU_FNCT_VOID)BSP_IntHandler_026,              /*  26 */
    (CPU_FNCT_VOID)OSCtxSwISR,                      /*  27, uC/OS-xx Context Switch                     */
    (CPU_FNCT_VOID)OS_BSP_TickISR,                  /*  28, uC/OS-xx Tick interrupt handler             */
    (CPU_FNCT_VOID)BSP_IntHandler_029,              /*  29 */

    (CPU_FNCT_VOID)BSP_IntHandler_030,              /*  30 */
    (CPU_FNCT_VOID)BSP_IntHandler_031,              /*  31 */
#if BSP_CFG_NET_EN > 0u
    (CPU_FNCT_VOID)NetDev_ISR,                      /*  32, Ethernet Controller                         */
#else
    (CPU_FNCT_VOID)BSP_IntHandler_032,              /*      32 ---------------------------------------- */
#endif

#if ((BSP_CFG_USBD_EN > 0u) || (BSP_CFG_USBH_EN > 0u))
#if BSP_CFG_USBD_EN > 0u
    (CPU_FNCT_VOID)USBD_BSP_RX63N_IntHandler,       /*  33, USB0  D0FIFO0                               */
    (CPU_FNCT_VOID)USBD_BSP_RX63N_IntHandler,       /*  34, USB0  D1FIFO0                               */
    (CPU_FNCT_VOID)USBD_BSP_RX63N_IntHandler,       /*  35, USB0  USBI0                                 */
#endif

#if BSP_CFG_USBH_EN > 0u
    (CPU_FNCT_VOID)USBH_RX600_BSP_IntHandler,       /*  33, USB0  D0FIFO0                               */
    (CPU_FNCT_VOID)USBH_RX600_BSP_IntHandler,       /*  34, USB0  D1FIFO0                               */
    (CPU_FNCT_VOID)USBH_RX600_BSP_IntHandler,       /*  35, USB0  USBI0                                 */
#endif

#else
    (CPU_FNCT_VOID)BSP_IntHandler_033,              /*      33 ---------------------------------------- */
    (CPU_FNCT_VOID)BSP_IntHandler_034,              /*      34 ---------------------------------------- */
    (CPU_FNCT_VOID)BSP_IntHandler_035,              /*      35 ---------------------------------------- */
#endif
    (CPU_FNCT_VOID)BSP_IntHandler_036,              /*  36 */
    (CPU_FNCT_VOID)BSP_IntHandler_037,              /*  37 */
    (CPU_FNCT_VOID)BSP_IntHandler_038,              /*  38 */
    (CPU_FNCT_VOID)BSP_IntHandler_039,              /*  39 */

    (CPU_FNCT_VOID)BSP_IntHandler_040,              /*  40 */
    (CPU_FNCT_VOID)BSP_IntHandler_041,              /*  41 */
    (CPU_FNCT_VOID)BSP_IntHandler_042,              /*  42 */
    (CPU_FNCT_VOID)BSP_IntHandler_043,              /*  43 */
    (CPU_FNCT_VOID)BSP_IntHandler_044,              /*  44 */
    (CPU_FNCT_VOID)BSP_IntHandler_045,              /*  45 */
    (CPU_FNCT_VOID)BSP_IntHandler_046,              /*  46 */
    (CPU_FNCT_VOID)BSP_IntHandler_047,              /*  47 */
    (CPU_FNCT_VOID)BSP_IntHandler_048,              /*  48 */
    (CPU_FNCT_VOID)BSP_IntHandler_049,              /*  49 */

    (CPU_FNCT_VOID)BSP_IntHandler_050,              /*  50 */
    (CPU_FNCT_VOID)BSP_IntHandler_051,              /*  51 */
    (CPU_FNCT_VOID)BSP_IntHandler_052,              /*  52 */
    (CPU_FNCT_VOID)BSP_IntHandler_053,              /*  53 */
    (CPU_FNCT_VOID)BSP_IntHandler_054,              /*  54 */
    (CPU_FNCT_VOID)BSP_IntHandler_055,              /*  55 */
    (CPU_FNCT_VOID)BSP_IntHandler_056,              /*  56 */
    (CPU_FNCT_VOID)BSP_IntHandler_057,              /*  57 */
    (CPU_FNCT_VOID)BSP_IntHandler_058,              /*  58 */
    (CPU_FNCT_VOID)BSP_IntHandler_059,              /*  59 */

    (CPU_FNCT_VOID)BSP_IntHandler_060,              /*  60 */
    (CPU_FNCT_VOID)BSP_IntHandler_061,              /*  61 */
    (CPU_FNCT_VOID)BSP_IntHandler_062,              /*  62 */
    (CPU_FNCT_VOID)BSP_IntHandler_063,              /*  63 */
    (CPU_FNCT_VOID)BSP_IntHandler_064,              /*  64 */
    (CPU_FNCT_VOID)BSP_IntHandler_065,              /*  65 */
    (CPU_FNCT_VOID)BSP_IntHandler_066,              /*  66 */
    (CPU_FNCT_VOID)BSP_IntHandler_067,              /*  67 */
    (CPU_FNCT_VOID)BSP_IntHandler_068,              /*  68 */
    (CPU_FNCT_VOID)BSP_IntHandler_069,              /*  69 */

    (CPU_FNCT_VOID)BSP_IntHandler_070,              /*  70 */
    (CPU_FNCT_VOID)BSP_IntHandler_071,              /*  71 */
    (CPU_FNCT_VOID)BSP_IntHandler_072,              /*  72 */
    (CPU_FNCT_VOID)BSP_IntHandler_073,              /*  73 */
    (CPU_FNCT_VOID)BSP_IntHandler_074,              /*  74 */
    (CPU_FNCT_VOID)BSP_IntHandler_075,              /*  75 */
    (CPU_FNCT_VOID)BSP_IntHandler_076,              /*  76 */
    (CPU_FNCT_VOID)BSP_IntHandler_077,              /*  77 */
    (CPU_FNCT_VOID)BSP_IntHandler_078,              /*  78 */
    (CPU_FNCT_VOID)BSP_IntHandler_079,              /*  79 */

    (CPU_FNCT_VOID)BSP_IntHandler_080,              /*  80 */
    (CPU_FNCT_VOID)BSP_IntHandler_081,              /*  81 */
    (CPU_FNCT_VOID)BSP_IntHandler_082,              /*  82 */
    (CPU_FNCT_VOID)BSP_IntHandler_083,              /*  83 */
    (CPU_FNCT_VOID)BSP_IntHandler_084,              /*  84 */
    (CPU_FNCT_VOID)BSP_IntHandler_085,              /*  85 */
    (CPU_FNCT_VOID)BSP_IntHandler_086,              /*  86 */
    (CPU_FNCT_VOID)BSP_IntHandler_087,              /*  87 */
    (CPU_FNCT_VOID)BSP_IntHandler_088,              /*  88 */
    (CPU_FNCT_VOID)BSP_IntHandler_089,              /*  89 */

#if ((BSP_CFG_USBD_EN > 0u) || (BSP_CFG_USBH_EN > 0u))
#if BSP_CFG_USBD_EN > 0u
    (CPU_FNCT_VOID)USBD_BSP_RX63N_IntHandler,       /*  90, USB  USBR0                                  */
#endif

#if BSP_CFG_USBH_EN > 0u
    (CPU_FNCT_VOID)USBH_RX600_BSP_IntHandler,       /*  90, USB  USBR0                                  */
#endif

#else
    (CPU_FNCT_VOID)BSP_IntHandler_090,              /*      90 ---------------------------------------- */
#endif
    (CPU_FNCT_VOID)BSP_IntHandler_091,              /*  91 */
    (CPU_FNCT_VOID)BSP_IntHandler_092,              /*  92 */
    (CPU_FNCT_VOID)BSP_IntHandler_093,              /*  93 */
    (CPU_FNCT_VOID)BSP_IntHandler_094,              /*  94 */
    (CPU_FNCT_VOID)BSP_IntHandler_095,              /*  95 */
    (CPU_FNCT_VOID)BSP_IntHandler_096,              /*  96 */
    (CPU_FNCT_VOID)BSP_IntHandler_097,              /*  97 */
    (CPU_FNCT_VOID)BSP_IntHandler_098,              /*  98 */
    (CPU_FNCT_VOID)BSP_IntHandler_099,              /*  99 */

    (CPU_FNCT_VOID)BSP_IntHandler_100,              /* 100 */
    (CPU_FNCT_VOID)BSP_IntHandler_101,              /* 101 */
    (CPU_FNCT_VOID)BSP_IntHandler_102,              /* 102 */
    (CPU_FNCT_VOID)BSP_IntHandler_103,              /* 103 */
    (CPU_FNCT_VOID)BSP_IntHandler_104,              /* 104 */
    (CPU_FNCT_VOID)BSP_IntHandler_105,              /* 105 */
    (CPU_FNCT_VOID)BSP_IntHandler_106,              /* 106 */
    (CPU_FNCT_VOID)BSP_IntHandler_107,              /* 107 */
    (CPU_FNCT_VOID)BSP_IntHandler_108,              /* 108 */
    (CPU_FNCT_VOID)BSP_IntHandler_109,              /* 109 */

    (CPU_FNCT_VOID)BSP_IntHandler_110,              /* 110 */
    (CPU_FNCT_VOID)BSP_IntHandler_111,              /* 111 */
    (CPU_FNCT_VOID)BSP_IntHandler_112,              /* 112 */
    (CPU_FNCT_VOID)BSP_IntHandler_113,              /* 113 */
    (CPU_FNCT_VOID)BSP_IntHandler_114,              /* 114 */
    (CPU_FNCT_VOID)BSP_IntHandler_115,              /* 115 */
    (CPU_FNCT_VOID)BSP_IntHandler_116,              /* 116 */
    (CPU_FNCT_VOID)BSP_IntHandler_117,              /* 117 */
    (CPU_FNCT_VOID)BSP_IntHandler_118,              /* 118 */
    (CPU_FNCT_VOID)BSP_IntHandler_119,              /* 119 */

    (CPU_FNCT_VOID)BSP_IntHandler_120,              /* 120 */
    (CPU_FNCT_VOID)BSP_IntHandler_121,              /* 121 */
    (CPU_FNCT_VOID)BSP_IntHandler_122,              /* 122 */
    (CPU_FNCT_VOID)BSP_IntHandler_123,              /* 123 */
    (CPU_FNCT_VOID)BSP_IntHandler_124,              /* 124 */
    (CPU_FNCT_VOID)BSP_IntHandler_125,              /* 125 */
    (CPU_FNCT_VOID)BSP_IntHandler_126,              /* 126 */
    (CPU_FNCT_VOID)BSP_IntHandler_127,              /* 127 */
    (CPU_FNCT_VOID)BSP_IntHandler_128,              /* 128 */
    (CPU_FNCT_VOID)BSP_IntHandler_129,              /* 129 */

    (CPU_FNCT_VOID)BSP_IntHandler_130,              /* 130 */
    (CPU_FNCT_VOID)BSP_IntHandler_131,              /* 131 */
    (CPU_FNCT_VOID)BSP_IntHandler_132,              /* 132 */
    (CPU_FNCT_VOID)BSP_IntHandler_133,              /* 133 */
    (CPU_FNCT_VOID)BSP_IntHandler_134,              /* 134 */
    (CPU_FNCT_VOID)BSP_IntHandler_135,              /* 135 */
    (CPU_FNCT_VOID)BSP_IntHandler_136,              /* 136 */
    (CPU_FNCT_VOID)BSP_IntHandler_137,              /* 137 */
    (CPU_FNCT_VOID)BSP_IntHandler_138,              /* 138 */
    (CPU_FNCT_VOID)BSP_IntHandler_139,              /* 139 */

    (CPU_FNCT_VOID)BSP_IntHandler_140,              /* 140 */
    (CPU_FNCT_VOID)BSP_IntHandler_141,              /* 141 */
    (CPU_FNCT_VOID)BSP_IntHandler_142,              /* 142 */
    (CPU_FNCT_VOID)BSP_IntHandler_143,              /* 143 */
    (CPU_FNCT_VOID)BSP_IntHandler_144,              /* 144 */
    (CPU_FNCT_VOID)BSP_IntHandler_145,              /* 145 */
    (CPU_FNCT_VOID)BSP_IntHandler_146,              /* 146 */
    (CPU_FNCT_VOID)BSP_IntHandler_147,              /* 147 */
    (CPU_FNCT_VOID)BSP_IntHandler_148,              /* 148 */
    (CPU_FNCT_VOID)BSP_IntHandler_149,              /* 149 */

    (CPU_FNCT_VOID)BSP_IntHandler_150,              /* 150 */
    (CPU_FNCT_VOID)BSP_IntHandler_151,              /* 151 */
    (CPU_FNCT_VOID)BSP_IntHandler_152,              /* 152 */
    (CPU_FNCT_VOID)BSP_IntHandler_153,              /* 153 */
    (CPU_FNCT_VOID)BSP_IntHandler_154,              /* 154 */
    (CPU_FNCT_VOID)BSP_IntHandler_155,              /* 155 */
    (CPU_FNCT_VOID)BSP_IntHandler_156,              /* 156 */
    (CPU_FNCT_VOID)BSP_IntHandler_157,              /* 157 */
    (CPU_FNCT_VOID)BSP_IntHandler_158,              /* 158 */
    (CPU_FNCT_VOID)BSP_IntHandler_159,              /* 159 */

    (CPU_FNCT_VOID)BSP_IntHandler_160,              /* 160 */
    (CPU_FNCT_VOID)BSP_IntHandler_161,              /* 161 */
    (CPU_FNCT_VOID)BSP_IntHandler_162,              /* 162 */
    (CPU_FNCT_VOID)BSP_IntHandler_163,              /* 163 */
    (CPU_FNCT_VOID)BSP_IntHandler_164,              /* 164 */
    (CPU_FNCT_VOID)BSP_IntHandler_165,              /* 165 */
    (CPU_FNCT_VOID)BSP_IntHandler_166,              /* 166 */
    (CPU_FNCT_VOID)BSP_IntHandler_167,              /* 167 */
    (CPU_FNCT_VOID)BSP_IntHandler_168,              /* 168 */
    (CPU_FNCT_VOID)BSP_IntHandler_169,              /* 169 */

    (CPU_FNCT_VOID)BSP_IntHandler_170,              /* 170 */
    (CPU_FNCT_VOID)BSP_IntHandler_171,              /* 171 */
    (CPU_FNCT_VOID)BSP_IntHandler_172,              /* 172 */
    (CPU_FNCT_VOID)BSP_IntHandler_173,              /* 173 */
    (CPU_FNCT_VOID)BSP_IntHandler_174,              /* 174 */
    (CPU_FNCT_VOID)BSP_IntHandler_175,              /* 175 */
    (CPU_FNCT_VOID)BSP_IntHandler_176,              /* 176 */
    (CPU_FNCT_VOID)BSP_IntHandler_177,              /* 177 */
    (CPU_FNCT_VOID)BSP_IntHandler_178,              /* 178 */
    (CPU_FNCT_VOID)BSP_IntHandler_179,              /* 179 */

    (CPU_FNCT_VOID)BSP_IntHandler_180,              /* 180 */
    (CPU_FNCT_VOID)BSP_IntHandler_181,              /* 181 */
    (CPU_FNCT_VOID)BSP_IntHandler_182,              /* 182 */
    (CPU_FNCT_VOID)BSP_IntHandler_183,              /* 183 */
    (CPU_FNCT_VOID)BSP_IntHandler_184,              /* 184 */
    (CPU_FNCT_VOID)BSP_IntHandler_185,              /* 185 */
    (CPU_FNCT_VOID)BSP_IntHandler_186,              /* 186 */
    (CPU_FNCT_VOID)BSP_IntHandler_187,              /* 187 */
    (CPU_FNCT_VOID)BSP_IntHandler_188,              /* 188 */
    (CPU_FNCT_VOID)BSP_IntHandler_189,              /* 189 */

    (CPU_FNCT_VOID)BSP_IntHandler_190,              /* 190 */
    (CPU_FNCT_VOID)BSP_IntHandler_191,              /* 191 */
    (CPU_FNCT_VOID)BSP_IntHandler_192,              /* 192 */
    (CPU_FNCT_VOID)BSP_IntHandler_193,              /* 193 */
    (CPU_FNCT_VOID)BSP_IntHandler_194,              /* 194 */
    (CPU_FNCT_VOID)BSP_IntHandler_195,              /* 195 */
    (CPU_FNCT_VOID)BSP_IntHandler_196,              /* 196 */
    (CPU_FNCT_VOID)BSP_IntHandler_197,              /* 197 */
#if BSP_CFG_NET_EN > 0u
    (CPU_FNCT_VOID)NetDev_ISR,                      /* 198, DMA Controller for Ethernet                 */
#else
    (CPU_FNCT_VOID)BSP_IntHandler_198,              /*      198 --------------------------------------- */
#endif
    (CPU_FNCT_VOID)BSP_IntHandler_199,              /* 199 */

    (CPU_FNCT_VOID)BSP_IntHandler_200,              /* 200 */
    (CPU_FNCT_VOID)BSP_IntHandler_201,              /* 201 */
    (CPU_FNCT_VOID)BSP_IntHandler_202,              /* 202 */
    (CPU_FNCT_VOID)BSP_IntHandler_203,              /* 203 */
    (CPU_FNCT_VOID)BSP_IntHandler_204,              /* 204 */
    (CPU_FNCT_VOID)BSP_IntHandler_205,              /* 205 */
    (CPU_FNCT_VOID)BSP_IntHandler_206,              /* 206 */
    (CPU_FNCT_VOID)BSP_IntHandler_207,              /* 207 */
    (CPU_FNCT_VOID)BSP_IntHandler_208,              /* 208 */
    (CPU_FNCT_VOID)BSP_IntHandler_209,              /* 209 */

    (CPU_FNCT_VOID)BSP_IntHandler_210,              /* 210 */
    (CPU_FNCT_VOID)BSP_IntHandler_211,              /* 211 */
    (CPU_FNCT_VOID)BSP_IntHandler_212,              /* 212 */
    (CPU_FNCT_VOID)BSP_IntHandler_213,              /* 213 */
    (CPU_FNCT_VOID)BSP_IntHandler_214,              /* 214 */
    (CPU_FNCT_VOID)BSP_IntHandler_215,              /* 215 */
    (CPU_FNCT_VOID)BSP_IntHandler_216,              /* 216 */
    (CPU_FNCT_VOID)BSP_IntHandler_217,              /* 217 */
    (CPU_FNCT_VOID)BSP_IntHandler_218,              /* 218 */
    (CPU_FNCT_VOID)BSP_IntHandler_219,              /* 219 */

#if BSP_CFG_SER_EN > 0u
    (CPU_FNCT_VOID)BSP_Ser_ISR_Rx_Handler,          /* 220, UART Rx Interrupts                          */
    (CPU_FNCT_VOID)BSP_Ser_ISR_Tx_Handler,          /* 221, UART Tx Interrupts                          */
#else
    (CPU_FNCT_VOID)BSP_IntHandler_220,              /*      220 --------------------------------------- */
    (CPU_FNCT_VOID)BSP_IntHandler_221,              /*      221 --------------------------------------- */
#endif
    (CPU_FNCT_VOID)BSP_IntHandler_222,              /* 222 */
    (CPU_FNCT_VOID)BSP_IntHandler_223,              /* 223 */
    (CPU_FNCT_VOID)BSP_IntHandler_224,              /* 224 */
    (CPU_FNCT_VOID)BSP_IntHandler_225,              /* 225 */
    (CPU_FNCT_VOID)BSP_IntHandler_226,              /* 226 */
    (CPU_FNCT_VOID)BSP_IntHandler_227,              /* 227 */
    (CPU_FNCT_VOID)BSP_IntHandler_228,              /* 228 */
    (CPU_FNCT_VOID)BSP_IntHandler_229,              /* 229 */

    (CPU_FNCT_VOID)BSP_IntHandler_230,              /* 230 */
    (CPU_FNCT_VOID)BSP_IntHandler_231,              /* 231 */
    (CPU_FNCT_VOID)BSP_IntHandler_232,              /* 232 */
    (CPU_FNCT_VOID)BSP_IntHandler_233,              /* 233 */
    (CPU_FNCT_VOID)BSP_IntHandler_234,              /* 234 */
    (CPU_FNCT_VOID)BSP_IntHandler_235,              /* 235 */
    (CPU_FNCT_VOID)BSP_IntHandler_236,              /* 236 */
    (CPU_FNCT_VOID)BSP_IntHandler_237,              /* 237 */
    (CPU_FNCT_VOID)BSP_IntHandler_238,              /* 238 */
    (CPU_FNCT_VOID)BSP_IntHandler_239,              /* 239 */

    (CPU_FNCT_VOID)BSP_IntHandler_240,              /* 240 */
    (CPU_FNCT_VOID)BSP_IntHandler_241,              /* 241 */
    (CPU_FNCT_VOID)BSP_IntHandler_242,              /* 242 */
    (CPU_FNCT_VOID)BSP_IntHandler_243,              /* 243 */
    (CPU_FNCT_VOID)BSP_IntHandler_244,              /* 244 */
    (CPU_FNCT_VOID)BSP_IntHandler_245,              /* 245 */
    (CPU_FNCT_VOID)BSP_IntHandler_246,              /* 246 */
    (CPU_FNCT_VOID)BSP_IntHandler_247,              /* 247 */
    (CPU_FNCT_VOID)BSP_IntHandler_248,              /* 248 */
    (CPU_FNCT_VOID)BSP_IntHandler_249,              /* 249 */

    (CPU_FNCT_VOID)BSP_IntHandler_250,              /* 250 */
    (CPU_FNCT_VOID)BSP_IntHandler_251,              /* 251 */
    (CPU_FNCT_VOID)BSP_IntHandler_252,              /* 252 */
    (CPU_FNCT_VOID)BSP_IntHandler_253,              /* 253 */
    (CPU_FNCT_VOID)BSP_IntHandler_254,              /* 254 */
    (CPU_FNCT_VOID)BSP_IntHandler_255,              /* 255 */
};


/*
*********************************************************************************************************
*                                            BSP_IntVectSet()
*
* Description : Assign ISR handler.
*
* Argument(s) : int_id      Interrupt for which vector will be set.
*
*               isr         Handler to assign.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void   BSP_IntVectSet          (CPU_INT16U     int_id,
                                CPU_FNCT_VOID  isr)
{
#if  BSP_CFG_INT_VECT_TBL_RAM_EN > 0u
    CPU_SR_ALLOC();


    CPU_INT_VECT_TBL_BASE_SET((CPU_INT32U)&BSP_IntVectTbl[0]);
    if (int_id < BSP_CFG_INT_VECT_TBL_SIZE) {
        CPU_CRITICAL_ENTER();
        BSP_IntVectTbl[int_id] = isr;
        CPU_CRITICAL_EXIT();
    }
#else
    (void)&int_id;
    (void)&isr;
    CPU_INT_VECT_TBL_BASE_SET((CPU_INT32U)&BSP_IntVectTbl[0]);
#endif
}


/*
*********************************************************************************************************
*                                          BSP_IntVectTblInit()
*
* Description : Sets up the interrupt vector table base address of the CPU.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntVectTblInit (void)
{
    CPU_INT_VECT_TBL_BASE_SET((CPU_INT32U)&BSP_IntVectTbl[0]);
}


/*
*********************************************************************************************************
*                                        BSP_IntHandler_xxx()
*
* Description : Dummy interrupt handlers.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntHandler_000 (void)
{
    while (1);
}


void  BSP_IntHandler_001 (void)
{
    while (1);
}


void  BSP_IntHandler_002 (void)
{
    while (1);
}


void  BSP_IntHandler_003 (void)
{
    while (1);
}


void  BSP_IntHandler_004 (void)
{
    while (1);
}


void  BSP_IntHandler_005 (void)
{
    while (1);
}


void  BSP_IntHandler_006 (void)
{
    while (1);
}


void  BSP_IntHandler_007 (void)
{
    while (1);
}


void  BSP_IntHandler_008 (void)
{
    while (1);
}


void  BSP_IntHandler_009 (void)
{
    while (1);
}


void  BSP_IntHandler_010 (void)
{
    while (1);
}


void  BSP_IntHandler_011 (void)
{
    while (1);
}


void  BSP_IntHandler_012 (void)
{
    while (1);
}


void  BSP_IntHandler_013 (void)
{
    while (1);
}


void  BSP_IntHandler_014 (void)
{
    while (1);
}


void  BSP_IntHandler_015 (void)
{
    while (1);
}


void  BSP_IntHandler_016 (void)
{
    while (1);
}


void  BSP_IntHandler_017 (void)
{
    while (1);
}


void  BSP_IntHandler_018 (void)
{
    while (1);
}


void  BSP_IntHandler_019 (void)
{
    while (1);
}


void  BSP_IntHandler_020 (void)
{
    while (1);
}


void  BSP_IntHandler_021 (void)
{
    while (1);
}


void  BSP_IntHandler_022 (void)
{
    while (1);
}


void  BSP_IntHandler_023 (void)
{
    while (1);
}


void  BSP_IntHandler_024 (void)
{
    while (1);
}


void  BSP_IntHandler_025 (void)
{
    while (1);
}


void  BSP_IntHandler_026 (void)
{
    while (1);
}


void  BSP_IntHandler_027 (void)
{
    while (1);
}


void  BSP_IntHandler_028 (void)
{
    while (1);
}


void  BSP_IntHandler_029 (void)
{
    while (1);
}


void  BSP_IntHandler_030 (void)
{
    while (1);
}


void  BSP_IntHandler_031 (void)
{
    while (1);
}


void  BSP_IntHandler_032 (void)
{
    while (1);
}


void  BSP_IntHandler_033 (void)
{
    while (1);
}


void  BSP_IntHandler_034 (void)
{
    while (1);
}


void  BSP_IntHandler_035 (void)
{
    while (1);
}


void  BSP_IntHandler_036 (void)
{
    while (1);
}


void  BSP_IntHandler_037 (void)
{
    while (1);
}


void  BSP_IntHandler_038 (void)
{
    while (1);
}


void  BSP_IntHandler_039 (void)
{
    while (1);
}


void  BSP_IntHandler_040 (void)
{
    while (1);
}


void  BSP_IntHandler_041 (void)
{
    while (1);
}


void  BSP_IntHandler_042 (void)
{
    while (1);
}


void  BSP_IntHandler_043 (void)
{
    while (1);
}


void  BSP_IntHandler_044 (void)
{
    while (1);
}


void  BSP_IntHandler_045 (void)
{
    while (1);
}


void  BSP_IntHandler_046 (void)
{
    while (1);
}


void  BSP_IntHandler_047 (void)
{
    while (1);
}


void  BSP_IntHandler_048 (void)
{
    while (1);
}


void  BSP_IntHandler_049 (void)
{
    while (1);
}


void  BSP_IntHandler_050 (void)
{
    while (1);
}


void  BSP_IntHandler_051 (void)
{
    while (1);
}


void  BSP_IntHandler_052 (void)
{
    while (1);
}


void  BSP_IntHandler_053 (void)
{
    while (1);
}


void  BSP_IntHandler_054 (void)
{
    while (1);
}


void  BSP_IntHandler_055 (void)
{
    while (1);
}


void  BSP_IntHandler_056 (void)
{
    while (1);
}


void  BSP_IntHandler_057 (void)
{
    while (1);
}


void  BSP_IntHandler_058 (void)
{
    while (1);
}


void  BSP_IntHandler_059 (void)
{
    while (1);
}


void  BSP_IntHandler_060 (void)
{
    while (1);
}


void  BSP_IntHandler_061 (void)
{
    while (1);
}


void  BSP_IntHandler_062 (void)
{
    while (1);
}


void  BSP_IntHandler_063 (void)
{
    while (1);
}


void  BSP_IntHandler_064 (void)
{
    while (1);
}


void  BSP_IntHandler_065 (void)
{
    while (1);
}


void  BSP_IntHandler_066 (void)
{
    while (1);
}


void  BSP_IntHandler_067 (void)
{
    while (1);
}


void  BSP_IntHandler_068 (void)
{
    while (1);
}


void  BSP_IntHandler_069 (void)
{
    while (1);
}


void  BSP_IntHandler_070 (void)
{
    while (1);
}


void  BSP_IntHandler_071 (void)
{
    while (1);
}


void  BSP_IntHandler_072 (void)
{
    while (1);
}


void  BSP_IntHandler_073 (void)
{
    while (1);
}


void  BSP_IntHandler_074 (void)
{
    while (1);
}


void  BSP_IntHandler_075 (void)
{
    while (1);
}


void  BSP_IntHandler_076 (void)
{
    while (1);
}


void  BSP_IntHandler_077 (void)
{
    while (1);
}


void  BSP_IntHandler_078 (void)
{
    while (1);
}


void  BSP_IntHandler_079 (void)
{
    while (1);
}


void  BSP_IntHandler_080 (void)
{
    while (1);
}


void  BSP_IntHandler_081 (void)
{
    while (1);
}


void  BSP_IntHandler_082 (void)
{
    while (1);
}


void  BSP_IntHandler_083 (void)
{
    while (1);
}


void  BSP_IntHandler_084 (void)
{
    while (1);
}


void  BSP_IntHandler_085 (void)
{
    while (1);
}


void  BSP_IntHandler_086 (void)
{
    while (1);
}


void  BSP_IntHandler_087 (void)
{
    while (1);
}


void  BSP_IntHandler_088 (void)
{
    while (1);
}


void  BSP_IntHandler_089 (void)
{
    while (1);
}


void  BSP_IntHandler_090 (void)
{
    while (1);
}


void  BSP_IntHandler_091 (void)
{
    while (1);
}


void  BSP_IntHandler_092 (void)
{
    while (1);
}


void  BSP_IntHandler_093 (void)
{
    while (1);
}


void  BSP_IntHandler_094 (void)
{
    while (1);
}


void  BSP_IntHandler_095 (void)
{
    while (1);
}


void  BSP_IntHandler_096 (void)
{
    while (1);
}


void  BSP_IntHandler_097 (void)
{
    while (1);
}


void  BSP_IntHandler_098 (void)
{
    while (1);
}


void  BSP_IntHandler_099 (void)
{
    while (1);
}


void  BSP_IntHandler_100 (void)
{
    while (1);
}


void  BSP_IntHandler_101 (void)
{
    while (1);
}


void  BSP_IntHandler_102 (void)
{
    while (1);
}


void  BSP_IntHandler_103 (void)
{
    while (1);
}


void  BSP_IntHandler_104 (void)
{
    while (1);
}


void  BSP_IntHandler_105 (void)
{
    while (1);
}


void  BSP_IntHandler_106 (void)
{
    while (1);
}


void  BSP_IntHandler_107 (void)
{
    while (1);
}


void  BSP_IntHandler_108 (void)
{
    while (1);
}


void  BSP_IntHandler_109 (void)
{
    while (1);
}


void  BSP_IntHandler_110 (void)
{
    while (1);
}


void  BSP_IntHandler_111 (void)
{
    while (1);
}


void  BSP_IntHandler_112 (void)
{
    while (1);
}


void  BSP_IntHandler_113 (void)
{
    while (1);
}


void  BSP_IntHandler_114 (void)
{
    while (1);
}


void  BSP_IntHandler_115 (void)
{
    while (1);
}


void  BSP_IntHandler_116 (void)
{
    while (1);
}


void  BSP_IntHandler_117 (void)
{
    while (1);
}


void  BSP_IntHandler_118 (void)
{
    while (1);
}


void  BSP_IntHandler_119 (void)
{
    while (1);
}


void  BSP_IntHandler_120 (void)
{
    while (1);
}


void  BSP_IntHandler_121 (void)
{
    while (1);
}


void  BSP_IntHandler_122 (void)
{
    while (1);
}


void  BSP_IntHandler_123 (void)
{
    while (1);
}


void  BSP_IntHandler_124 (void)
{
    while (1);
}


void  BSP_IntHandler_125 (void)
{
    while (1);
}


void  BSP_IntHandler_126 (void)
{
    while (1);
}


void  BSP_IntHandler_127 (void)
{
    while (1);
}


void  BSP_IntHandler_128 (void)
{
    while (1);
}


void  BSP_IntHandler_129 (void)
{
    while (1);
}


void  BSP_IntHandler_130 (void)
{
    while (1);
}


void  BSP_IntHandler_131 (void)
{
    while (1);
}


void  BSP_IntHandler_132 (void)
{
    while (1);
}


void  BSP_IntHandler_133 (void)
{
    while (1);
}


void  BSP_IntHandler_134 (void)
{
    while (1);
}


void  BSP_IntHandler_135 (void)
{
    while (1);
}


void  BSP_IntHandler_136 (void)
{
    while (1);
}


void  BSP_IntHandler_137 (void)
{
    while (1);
}


void  BSP_IntHandler_138 (void)
{
    while (1);
}


void  BSP_IntHandler_139 (void)
{
    while (1);
}


void  BSP_IntHandler_140 (void)
{
    while (1);
}


void  BSP_IntHandler_141 (void)
{
    while (1);
}


void  BSP_IntHandler_142 (void)
{
    while (1);
}


void  BSP_IntHandler_143 (void)
{
    while (1);
}


void  BSP_IntHandler_144 (void)
{
    while (1);
}


void  BSP_IntHandler_145 (void)
{
    while (1);
}


void  BSP_IntHandler_146 (void)
{
    while (1);
}


void  BSP_IntHandler_147 (void)
{
    while (1);
}


void  BSP_IntHandler_148 (void)
{
    while (1);
}


void  BSP_IntHandler_149 (void)
{
    while (1);
}


void  BSP_IntHandler_150 (void)
{
    while (1);
}


void  BSP_IntHandler_151 (void)
{
    while (1);
}


void  BSP_IntHandler_152 (void)
{
    while (1);
}


void  BSP_IntHandler_153 (void)
{
    while (1);
}


void  BSP_IntHandler_154 (void)
{
    while (1);
}


void  BSP_IntHandler_155 (void)
{
    while (1);
}


void  BSP_IntHandler_156 (void)
{
    while (1);
}


void  BSP_IntHandler_157 (void)
{
    while (1);
}


void  BSP_IntHandler_158 (void)
{
    while (1);
}


void  BSP_IntHandler_159 (void)
{
    while (1);
}


void  BSP_IntHandler_160 (void)
{
    while (1);
}


void  BSP_IntHandler_161 (void)
{
    while (1);
}


void  BSP_IntHandler_162 (void)
{
    while (1);
}


void  BSP_IntHandler_163 (void)
{
    while (1);
}


void  BSP_IntHandler_164 (void)
{
    while (1);
}


void  BSP_IntHandler_165 (void)
{
    while (1);
}


void  BSP_IntHandler_166 (void)
{
    while (1);
}


void  BSP_IntHandler_167 (void)
{
    while (1);
}


void  BSP_IntHandler_168 (void)
{
    while (1);
}


void  BSP_IntHandler_169 (void)
{
    while (1);
}


void  BSP_IntHandler_170 (void)
{
    while (1);
}


void  BSP_IntHandler_171 (void)
{
    while (1);
}


void  BSP_IntHandler_172 (void)
{
    while (1);
}


void  BSP_IntHandler_173 (void)
{
    while (1);
}


void  BSP_IntHandler_174 (void)
{
    while (1);
}


void  BSP_IntHandler_175 (void)
{
    while (1);
}


void  BSP_IntHandler_176 (void)
{
    while (1);
}


void  BSP_IntHandler_177 (void)
{
    while (1);
}


void  BSP_IntHandler_178 (void)
{
    while (1);
}


void  BSP_IntHandler_179 (void)
{
    while (1);
}


void  BSP_IntHandler_180 (void)
{
    while (1);
}


void  BSP_IntHandler_181 (void)
{
    while (1);
}


void  BSP_IntHandler_182 (void)
{
    while (1);
}


void  BSP_IntHandler_183 (void)
{
    while (1);
}


void  BSP_IntHandler_184 (void)
{
    while (1);
}


void  BSP_IntHandler_185 (void)
{
    while (1);
}


void  BSP_IntHandler_186 (void)
{
    while (1);
}


void  BSP_IntHandler_187 (void)
{
    while (1);
}


void  BSP_IntHandler_188 (void)
{
    while (1);
}


void  BSP_IntHandler_189 (void)
{
    while (1);
}


void  BSP_IntHandler_190 (void)
{
    while (1);
}


void  BSP_IntHandler_191 (void)
{
    while (1);
}


void  BSP_IntHandler_192 (void)
{
    while (1);
}


void  BSP_IntHandler_193 (void)
{
    while (1);
}


void  BSP_IntHandler_194 (void)
{
    while (1);
}


void  BSP_IntHandler_195 (void)
{
    while (1);
}


void  BSP_IntHandler_196 (void)
{
    while (1);
}


void  BSP_IntHandler_197 (void)
{
    while (1);
}


void  BSP_IntHandler_198 (void)
{
    while (1);
}


void  BSP_IntHandler_199 (void)
{
    while (1);
}


void  BSP_IntHandler_200 (void)
{
    while (1);
}


void  BSP_IntHandler_201 (void)
{
    while (1);
}


void  BSP_IntHandler_202 (void)
{
    while (1);
}


void  BSP_IntHandler_203 (void)
{
    while (1);
}


void  BSP_IntHandler_204 (void)
{
    while (1);
}


void  BSP_IntHandler_205 (void)
{
    while (1);
}


void  BSP_IntHandler_206 (void)
{
    while (1);
}


void  BSP_IntHandler_207 (void)
{
    while (1);
}


void  BSP_IntHandler_208 (void)
{
    while (1);
}


void  BSP_IntHandler_209 (void)
{
    while (1);
}


void  BSP_IntHandler_210 (void)
{
    while (1);
}


void  BSP_IntHandler_211 (void)
{
    while (1);
}


void  BSP_IntHandler_212 (void)
{
    while (1);
}


void  BSP_IntHandler_213 (void)
{
    while (1);
}


void  BSP_IntHandler_214 (void)
{
    while (1);
}


void  BSP_IntHandler_215 (void)
{
    while (1);
}


void  BSP_IntHandler_216 (void)
{
    while (1);
}


void  BSP_IntHandler_217 (void)
{
    while (1);
}


void  BSP_IntHandler_218 (void)
{
    while (1);
}


void  BSP_IntHandler_219 (void)
{
    while (1);
}


void  BSP_IntHandler_220 (void)
{
    while (1);
}


void  BSP_IntHandler_221 (void)
{
    while (1);
}


void  BSP_IntHandler_222 (void)
{
    while (1);
}


void  BSP_IntHandler_223 (void)
{
    while (1);
}


void  BSP_IntHandler_224 (void)
{
    while (1);
}


void  BSP_IntHandler_225 (void)
{
    while (1);
}


void  BSP_IntHandler_226 (void)
{
    while (1);
}


void  BSP_IntHandler_227 (void)
{
    while (1);
}


void  BSP_IntHandler_228 (void)
{
    while (1);
}


void  BSP_IntHandler_229 (void)
{
    while (1);
}


void  BSP_IntHandler_230 (void)
{
    while (1);
}


void  BSP_IntHandler_231 (void)
{
    while (1);
}


void  BSP_IntHandler_232 (void)
{
    while (1);
}


void  BSP_IntHandler_233 (void)
{
    while (1);
}


void  BSP_IntHandler_234 (void)
{
    while (1);
}


void  BSP_IntHandler_235 (void)
{
    while (1);
}


void  BSP_IntHandler_236 (void)
{
    while (1);
}


void  BSP_IntHandler_237 (void)
{
    while (1);
}


void  BSP_IntHandler_238 (void)
{
    while (1);
}


void  BSP_IntHandler_239 (void)
{
    while (1);
}


void  BSP_IntHandler_240 (void)
{
    while (1);
}


void  BSP_IntHandler_241 (void)
{
    while (1);
}


void  BSP_IntHandler_242 (void)
{
    while (1);
}


void  BSP_IntHandler_243 (void)
{
    while (1);
}


void  BSP_IntHandler_244 (void)
{
    while (1);
}


void  BSP_IntHandler_245 (void)
{
    while (1);
}


void  BSP_IntHandler_246 (void)
{
    while (1);
}


void  BSP_IntHandler_247 (void)
{
    while (1);
}


void  BSP_IntHandler_248 (void)
{
    while (1);
}


void  BSP_IntHandler_249 (void)
{
    while (1);
}


void  BSP_IntHandler_250 (void)
{
    while (1);
}


void  BSP_IntHandler_251 (void)
{
    while (1);
}


void  BSP_IntHandler_252 (void)
{
    while (1);
}


void  BSP_IntHandler_253 (void)
{
    while (1);
}


void  BSP_IntHandler_254 (void)
{
    while (1);
}


void  BSP_IntHandler_255 (void)
{
    while (1);
}
