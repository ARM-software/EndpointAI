/*-----------------------------------------------------------------------------
 * Name:    GLCD_SPI_MPS3.h
 * Purpose: Graphic LCD function prototypes and defines
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef _GLCD_SPI_H
#define _GLCD_SPI_H

/*------------------------------------------------------------------------------
  Color coding
  GLCD is coded:   15..11 red, 10..5 green, 4..0 blue  (unsigned short)  GLCD_R5, GLCD_G6, GLCD_B5
  original coding: 17..12 red, 11..6 green, 5..0 blue                    ORG_R6,  ORG_G6,  ORG_B6

  ORG_R1..5 = GLCD_R0..4,  ORG_R0 = GLCD_R4
  ORG_G0..5 = GLCD_G0..5,
  ORG_B1..5 = GLCD_B0..4,  ORG_B0 = GLCD_B4
 *----------------------------------------------------------------------------*/

/* GLCD RGB color definitions                                                 */
#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255, 0   */
#define White           0xFFFF      /* 255, 255, 255 */

extern void GLCD_Initialize     (void);
extern void GLCD_WindowMax      (void);
extern void GLCD_SetWindow      (unsigned int x, unsigned int y, unsigned int w, unsigned int h);
extern void GLCD_SetWindowSize  (unsigned int xs, unsigned int xe, unsigned int ys, unsigned int ye);
extern void GLCD_PutPixel       (unsigned int x, unsigned int y);
extern void GLCD_PutPixelColor  (unsigned int x, unsigned int y, unsigned short color);
extern void GLCD_SetTextColor   (unsigned short color);
extern void GLCD_SetBackColor   (unsigned short color);
extern void GLCD_Clear          (unsigned short color);
extern void GLCD_DrawChar       (unsigned int x,  unsigned int y, unsigned int cw, unsigned int ch, unsigned char *c);
extern void GLCD_DisplayChar    (unsigned int ln, unsigned int col, unsigned char fi, unsigned char  c);
extern void GLCD_DisplayString  (unsigned int ln, unsigned int col, unsigned char fi, char *s);
extern void GLCD_ClearLn        (unsigned int ln, unsigned char fi);
extern void GLCD_Bargraph       (unsigned int x,  unsigned int y, unsigned int w, unsigned int h, unsigned int val);
extern void GLCD_Bitmap         (unsigned int x,  unsigned int y, unsigned int w, unsigned int h, unsigned short *bitmap);
extern void GLCD_BitmapSize     (unsigned int xs, unsigned int xe, unsigned int ys, unsigned int ye, const unsigned short *bitmap);
extern void GLCD_ScrollVertical (unsigned int dy);

extern void GLCD_WrCmd          (unsigned char cmd);
extern void GLCD_WrReg          (unsigned char reg, unsigned short val);
extern void GLCD_Start          (void);
extern void GLCD_Write          (unsigned short data);
extern void GLCD_End            (void);

extern void GLCD_Boarder        (void);
extern void GLCD_Box            (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned short color);
extern void GLCD_BoxSize        (unsigned int xs, unsigned int xe, unsigned int ys, unsigned int ye, unsigned short color);

extern void GLCD_ChangeBitMap (unsigned int xs, unsigned int xe, unsigned int ys, unsigned int ye, 
                        const unsigned short * bitmap, unsigned int n, unsigned short * color);
#endif /* _GLCD_H */
