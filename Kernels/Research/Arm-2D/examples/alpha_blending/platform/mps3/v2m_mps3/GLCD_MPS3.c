/*-----------------------------------------------------------------------------
 * Name:    GLCD_SPI_MPS3.c
 * Purpose: MPS3 low level Graphic LCD (320x240 pixels) with SPI interface
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


#include "SMM_MPS3.h"
#include "GLCD_MPS3.h"

#include "Font_6x8_h.h"
#include "Font_16x24_h.h"


#define CHARLCD_BASE    0x4130A000

// CLCD Controller Internal Register addresses
#define CHAR_COM        ((volatile unsigned int *)(CHARLCD_BASE + 0x000))
#define CHAR_DAT        ((volatile unsigned int *)(CHARLCD_BASE + 0x004))
#define CHAR_RD         ((volatile unsigned int *)(CHARLCD_BASE + 0x008))
#define CHAR_RAW        ((volatile unsigned int *)(CHARLCD_BASE + 0x00C))
#define CHAR_MASK       ((volatile unsigned int *)(CHARLCD_BASE + 0x010))
#define CHAR_STAT       ((volatile unsigned int *)(CHARLCD_BASE + 0x014))
#define CHAR_MISC       ((volatile unsigned int *)(CHARLCD_BASE + 0x04C))

/************************** Orientation  configuration ************************/

#ifndef LANDSCAPE
#define LANDSCAPE   1                   /* 1 for landscape, 0 for portrait    */
#endif
#ifndef ROTATE180
#define ROTATE180   1                   /* 1 to rotate the screen for 180 deg */
#endif

/*------------------------- Speed dependant settings -------------------------*/

/* If processor works on high frequency delay has to be increased, it can be
   increased by factor 2^N by this constant                                   */
#define DELAY_2N    8

/*---------------------- Graphic LCD size definitions ------------------------*/

#if (LANDSCAPE == 1)
#define WIDTH       320                 /* Screen Width (in pixels)           */
#define HEIGHT      240                 /* Screen Hight (in pixels)           */
#else
#define WIDTH       240                 /* Screen Width (in pixels)           */
#define HEIGHT      320                 /* Screen Hight (in pixels)           */
#endif
#define BPP         16                  /* Bits per pixel                     */
#define BYPP        ((BPP+7)/8)         /* Bytes per pixel                    */

/*--------------- Graphic LCD interface hardware definitions -----------------*/

/* Pin CS setting to 0 or 1                                                   */
#define LCD_CS(x)   ((x) ? (*CHAR_MISC |= CLCD_CS_Msk)    : (*CHAR_MISC &= ~CLCD_CS_Msk))
#define LCD_RST(x)  ((x) ? (*CHAR_MISC |= CLCD_RESET_Msk) : (*CHAR_MISC &= ~CLCD_RESET_Msk))
#define LCD_BL(x)   ((x) ? (*CHAR_MISC |= CLCD_BL_Msk)    : (*CHAR_MISC &= ~CLCD_BL_Msk))

#define BG_COLOR  0                     /* Background color                   */
#define TXT_COLOR 1                     /* Text color                         */

/************************ Global variables ************************************/

/******************************************************************************
* Text and background color                                                   *
*******************************************************************************/
static volatile unsigned short Color[2] = {White, Black};

/************************ Local auxiliary functions ***************************/

/*******************************************************************************
* Delay in while loop cycles                                                   *
*   Parameter:    cnt:    number of while cycles to delay                      *
*   Return:                                                                    *
*******************************************************************************/

static void delay (int cnt) {
/*
  cnt <<= DELAY_2N;
  while (cnt--) {
	 __asm volatile ("NOP");
  };
*/
extern void delay_ms(uint_fast32_t wMS);
    delay_ms(cnt);
}

/*******************************************************************************
* Write a command the LCD controller                                           *
*   Parameter:    cmd:    command to be written                                *
*   Return:                                                                    *
*******************************************************************************/

static __inline void wr_cmd (unsigned char cmd) {
  LCD_CS(0);
  *CHAR_COM = cmd;
  LCD_CS(1);
}


/*******************************************************************************
* Write data to the LCD controller                                             *
*   Parameter:    dat:    data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/

static __inline void wr_dat (unsigned short dat) {
  LCD_CS(0);
  *CHAR_DAT = (dat >>   8);                   /* Write D8..D15                */
  *CHAR_DAT = (dat & 0xFF);                   /* Write D0..D7                 */
  LCD_CS(1);
}


/*******************************************************************************
* Start of data writing to the LCD controller                                  *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

static __inline void wr_dat_start (void) {
  LCD_CS(0);
}


/*******************************************************************************
* Stop of data writing to the LCD controller                                   *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

static __inline void wr_dat_stop (void) {
  LCD_CS(1);
}


/*******************************************************************************
* Data writing to the LCD controller                                           *
*   Parameter:    dat:    data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/

static __inline void wr_dat_only (unsigned short dat) {
  *CHAR_DAT = (dat >>   8);                   /* Write D8..D15                */
  *CHAR_DAT = (dat & 0xFF);                   /* Write D0..D7                 */
}


/*******************************************************************************
* Write a value to the to LCD register                                         *
*   Parameter:    reg:    register to be written                               *
*                 val:    value to write to the register                       *
*******************************************************************************/

static __inline void wr_reg (unsigned char reg, unsigned short val) {

  LCD_CS(0);
  *CHAR_COM = reg;
  wr_dat_only(val);
  LCD_CS(1);
}

static __inline unsigned short rd_reg (unsigned char reg) {

  unsigned short val;
  LCD_CS(0);
  *CHAR_COM = reg;
  val = (*CHAR_DAT >> 8);
  val = val + (*CHAR_DAT & 0xFF);
  LCD_CS(1);
  return val;
}

/************************ Exported functions **********************************/

/*******************************************************************************
* Initialize the Himax LCD with HX8347-D LCD Controller                        *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_Initialize (void) {

    /* CLCD screen setup (Default CLCD screen interface state)                */
    LCD_CS(1);                              /* deassert nCS0 */
    LCD_RST(1);                             /* deassert Reset */
    LCD_BL(0);                              /* switch off backlight */

    // Reset CLCD screen
    LCD_RST(0);                             /* assert Reset */
    delay(1);
    LCD_RST(1);                             /* deassert Reset */
    delay(10);

    /* Driving ability settings ----------------------------------------------*/
    wr_reg(0xEA, 0x00);                 /* Power control internal used (1)    */
    wr_reg(0xEB, 0x20);                 /* Power control internal used (2)    */
    wr_reg(0xEC, 0x0C);                 /* Source control internal used (1)   */
    wr_reg(0xED, 0xC7);                 /* Source control internal used (2)   */
    wr_reg(0xE8, 0x38);                 /* Source output period Normal mode   */
    wr_reg(0xE9, 0x10);                 /* Source output period Idle mode     */
    wr_reg(0xF1, 0x01);                 /* RGB 18-bit interface ;0x0110       */
    wr_reg(0xF2, 0x10);

    /* Adjust the Gamma Curve ------------------------------------------------*/
    wr_reg(0x40, 0x01);
    wr_reg(0x41, 0x00);
    wr_reg(0x42, 0x00);
    wr_reg(0x43, 0x10);
    wr_reg(0x44, 0x0E);
    wr_reg(0x45, 0x24);
    wr_reg(0x46, 0x04);
    wr_reg(0x47, 0x50);
    wr_reg(0x48, 0x02);
    wr_reg(0x49, 0x13);
    wr_reg(0x4A, 0x19);
    wr_reg(0x4B, 0x19);
    wr_reg(0x4C, 0x16);

    wr_reg(0x50, 0x1B);
    wr_reg(0x51, 0x31);
    wr_reg(0x52, 0x2F);
    wr_reg(0x53, 0x3F);
    wr_reg(0x54, 0x3F);
    wr_reg(0x55, 0x3E);
    wr_reg(0x56, 0x2F);
    wr_reg(0x57, 0x7B);
    wr_reg(0x58, 0x09);
    wr_reg(0x59, 0x06);
    wr_reg(0x5A, 0x06);
    wr_reg(0x5B, 0x0C);
    wr_reg(0x5C, 0x1D);
    wr_reg(0x5D, 0xCC);

    /* Power voltage setting -------------------------------------------------*/
    wr_reg(0x1B, 0x1B);
    wr_reg(0x1A, 0x01);
    wr_reg(0x24, 0x2F);
    wr_reg(0x25, 0x57);
    wr_reg(0x23, 0x88);

    /* Power on setting ------------------------------------------------------*/
    wr_reg(0x18, 0x36);                 /* Internal oscillator frequency adj  */
    wr_reg(0x19, 0x01);                 /* Enable internal oscillator         */
    wr_reg(0x01, 0x00);                 /* Normal mode, no scrool             */
    wr_reg(0x1F, 0x88);                 /* Power control 6 - DDVDH Off        */
    delay(20);
    wr_reg(0x1F, 0x82);                 /* Power control 6 - Step-up: 3 x VCI */
    delay(5);
    wr_reg(0x1F, 0x92);                 /* Power control 6 - Step-up: On      */
    delay(5);
    wr_reg(0x1F, 0xD2);                 /* Power control 6 - VCOML active     */
    delay(5);

    /* Color selection -------------------------------------------------------*/
    wr_reg(0x17, 0x55);                 /* RGB, System interface: 16 Bit/Pixel*/
    wr_reg(0x00, 0x00);                 /* Scrolling off, no standby          */

    /* Interface config ------------------------------------------------------*/
    wr_reg(0x2F, 0x11);                 /* LCD Drive: 1-line inversion        */
    wr_reg(0x31, 0x00);
    wr_reg(0x32, 0x00);                 /* DPL=0, HSPL=0, VSPL=0, EPL=0       */

    /* Display on setting ----------------------------------------------------*/
    wr_reg(0x28, 0x38);                 /* PT(0,0) active, VGL/VGL            */
    delay(20);
    wr_reg(0x28, 0x3C);                 /* Display active, VGL/VGL            */

   #if (LANDSCAPE == 1)
    #if (ROTATE180 == 0)
     wr_reg (0x16, 0xA8);
    #else
     wr_reg (0x16, 0x68);
    #endif
   #else
    #if (ROTATE180 == 0)
     wr_reg (0x16, 0x08);
    #else
     wr_reg (0x16, 0xC8);
    #endif
   #endif

    /* Display scrolling settings --------------------------------------------*/
    wr_reg(0x0E, 0x00);                 /* TFA MSB                            */
    wr_reg(0x0F, 0x00);                 /* TFA LSB                            */
    wr_reg(0x10, 320 >> 8);             /* VSA MSB                            */
    wr_reg(0x11, 320 &  0xFF);          /* VSA LSB                            */
    wr_reg(0x12, 0x00);                 /* BFA MSB                            */
    wr_reg(0x13, 0x00);                 /* BFA LSB                            */

    LCD_BL(1);                          /* turn on backlight                  */
}


/*******************************************************************************
* Set draw window region                                                       *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   w:        window width in pixel                            *
*                   h:        window height in pixels                          *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_SetWindow (unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    unsigned int xe, ye;

    xe = x+w-1;
    ye = y+h-1;

    wr_reg(0x02, x  >>    8);           /* Column address start MSB           */
    wr_reg(0x03, x  &  0xFF);           /* Column address start LSB           */
    wr_reg(0x04, xe >>    8);           /* Column address end MSB             */
    wr_reg(0x05, xe &  0xFF);           /* Column address end LSB             */

    wr_reg(0x06, y  >>    8);           /* Row address start MSB              */
    wr_reg(0x07, y  &  0xFF);           /* Row address start LSB              */
    wr_reg(0x08, ye >>    8);           /* Row address end MSB                */
    wr_reg(0x09, ye &  0xFF);           /* Row address end LSB                */
}


/*******************************************************************************
* Set draw window region                                                       *
*   Parameter:      xs:       start horizontal position                        *
*                   xe:       end horizontal position                          *
*                   ys:       start vertical position                          *
*                   ye:       end vertical position                            *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_SetWindowSize (unsigned int xs, unsigned int xe, unsigned int ys, unsigned int ye) {

    wr_reg(0x02, xs  >>    8);           /* Column address start MSB           */
    wr_reg(0x03, xs  &  0xFF);           /* Column address start LSB           */
    wr_reg(0x04, xe  >>    8);           /* Column address end MSB             */
    wr_reg(0x05, xe  &  0xFF);           /* Column address end LSB             */

    wr_reg(0x06, ys  >>    8);           /* Row address start MSB              */
    wr_reg(0x07, ys  &  0xFF);           /* Row address start LSB              */
    wr_reg(0x08, ye  >>    8);           /* Row address end MSB                */
    wr_reg(0x09, ye  &  0xFF);           /* Row address end LSB                */
}


/*******************************************************************************
* Set draw window region to whole screen                                       *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_WindowMax (void) {
  GLCD_SetWindow (0, 0, WIDTH, HEIGHT);
}


/*******************************************************************************
* Draw a pixel in foreground color                                             *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_PutPixel (unsigned int x, unsigned int y) {

    wr_reg(0x02, x >>    8);            /* Column address start MSB           */
    wr_reg(0x03, x &  0xFF);            /* Column address start LSB           */
    wr_reg(0x04, x >>    8);            /* Column address end MSB             */
    wr_reg(0x05, x &  0xFF);            /* Column address end LSB             */

    wr_reg(0x06, y >>    8);            /* Row address start MSB              */
    wr_reg(0x07, y &  0xFF);            /* Row address start LSB              */
    wr_reg(0x08, y >>    8);            /* Row address end MSB                */
    wr_reg(0x09, y &  0xFF);            /* Row address end LSB                */

  wr_cmd(0x22);
  wr_dat(Color[TXT_COLOR]);
}


/*******************************************************************************
* Draw a pixel in supplied color                                             *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   color:    color of pixel                                   *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_PutPixelColor (unsigned int x, unsigned int y, unsigned short color) {

    wr_reg(0x02, x >>    8);            /* Column address start MSB           */
    wr_reg(0x03, x &  0xFF);            /* Column address start LSB           */
    wr_reg(0x04, x >>    8);            /* Column address end MSB             */
    wr_reg(0x05, x &  0xFF);            /* Column address end LSB             */

    wr_reg(0x06, y >>    8);            /* Row address start MSB              */
    wr_reg(0x07, y &  0xFF);            /* Row address start LSB              */
    wr_reg(0x08, y >>    8);            /* Row address end MSB                */
    wr_reg(0x09, y &  0xFF);            /* Row address end LSB                */

  wr_cmd(0x22);
  wr_dat(color);
}


/*******************************************************************************
* Set foreground color                                                         *
*   Parameter:      color:    foreground color                                 *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_SetTextColor (unsigned short color) {
  Color[TXT_COLOR] = color;
}


/*******************************************************************************
* Set background color                                                         *
*   Parameter:      color:    background color                                 *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_SetBackColor (unsigned short color) {
  Color[BG_COLOR] = color;
}


/*******************************************************************************
* Clear display                                                                *
*   Parameter:      color:    display clearing color                           *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_Clear (unsigned short color) {
  unsigned int i;

  GLCD_WindowMax();
  wr_cmd(0x22);
  wr_dat_start();

  for(i = 0; i < (WIDTH*HEIGHT); i++)
    wr_dat_only(color);
  wr_dat_stop();
}


/*******************************************************************************
* Draw character on given position                                             *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   cw:       character width in pixel                         *
*                   ch:       character height in pixels                       *
*                   c:        pointer to character bitmap                      *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_DrawChar (unsigned int x, unsigned int y, unsigned int cw, unsigned int ch, unsigned char *c) {
  unsigned int i, j, k, pixs;

  GLCD_SetWindow(x, y, cw, ch);

  wr_cmd(0x22);
  wr_dat_start();

  k  = (cw + 7)/8;

  if (k == 1) {
    for (j = 0; j < ch; j++) {
      pixs = *(unsigned char  *)c;
      c += 1;

      for (i = 0; i < cw; i++) {
        wr_dat_only (Color[(pixs >> i) & 1]);
      }
    }
  }
  else if (k == 2) {
    for (j = 0; j < ch; j++) {
      pixs = *(unsigned short *)c;
      c += 2;

      for (i = 0; i < cw; i++) {
        wr_dat_only (Color[(pixs >> i) & 1]);
      }
    }
  }
  wr_dat_stop();
}


/*******************************************************************************
* Disply character on given line                                               *
*   Parameter:      ln:       line number                                      *
*                   col:      column number                                    *
*                   fi:       font index (0 = 6x8, 1 = 16x24)                  *
*                   c:        ascii character                                  *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_DisplayChar (unsigned int ln, unsigned int col, unsigned char fi, unsigned char c) {
  c -= 32;
  switch (fi) {
    case 0:  /* Font 6 x 8 */
      GLCD_DrawChar(col *  6, ln *  8,  6,  8, (unsigned char *)&Font_6x8_h  [c * 8]);
      break;
    case 1:  /* Font 16 x 24 */
      GLCD_DrawChar(col * 16, ln * 24, 16, 24, (unsigned char *)&Font_16x24_h[c * 24]);
      break;
  }
}


/*******************************************************************************
* Disply string on given line                                                  *
*   Parameter:      ln:       line number                                      *
*                   col:      column number                                    *
*                   fi:       font index (0 = 6x8, 1 = 16x24)                  *
*                   s:        pointer to string                                *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_DisplayString (unsigned int ln, unsigned int col, unsigned char fi, char *s) {
  while (*s) {
    GLCD_DisplayChar(ln, col++, fi, *s++);
  }
}


/*******************************************************************************
* Clear given line                                                             *
*   Parameter:      ln:       line number                                      *
*                   fi:       font index (0 = 6x8, 1 = 16x24)                  *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_ClearLn (unsigned int ln, unsigned char fi) {
  unsigned char i;
  char buf[60];

  GLCD_WindowMax();
  switch (fi) {
    case 0:  /* Font 6 x 8 */
      for (i = 0; i < (WIDTH+5)/6; i++)
        buf[i] = ' ';
      buf[i+1] = 0;
      break;
    case 1:  /* Font 16 x 24 */
      for (i = 0; i < (WIDTH+15)/16; i++)
        buf[i] = ' ';
      buf[i+1] = 0;
      break;
  }
  GLCD_DisplayString (ln, 0, fi, buf);
}

/*******************************************************************************
* Draw bargraph                                                                *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   w:        maximum width of bargraph (in pixels)            *
*                   h:        bargraph height                                  *
*                   val:      value of active bargraph (in 1/1024)             *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_Bargraph (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int val) {
  int i,j;

  val = (val * w) >> 10;                /* Scale value                        */
  GLCD_SetWindow(x, y, w, h);
  wr_cmd(0x22);
  wr_dat_start();
  for (i = 0; i < h; i++) {
    for (j = 0; j <= w-1; j++) {
      if(j >= val) {
        wr_dat_only(Color[BG_COLOR]);
      } else {
        wr_dat_only(Color[TXT_COLOR]);
      }
    }
  }
  wr_dat_stop();
}


/*******************************************************************************
* Display graphical bitmap image at position x horizontally and y vertically   *
* (This function is optimized for 16 bits per pixel format, it has to be       *
*  adapted for any other bits per pixel format)                                *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   w:        width of bitmap                                  *
*                   h:        height of bitmap                                 *
*                   bitmap:   address at which the bitmap data resides         *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_Bitmap (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned short *bitmap) {
  int i;
  unsigned short *bitmap_ptr = bitmap;

  GLCD_SetWindow (x, y, w, h);

  wr_cmd(0x22);
  wr_dat_start();

  for (i = 0; i < (w*h); i++) {
    wr_dat_only (bitmap_ptr[i]);
  }
  wr_dat_stop();
}


/*******************************************************************************
* Display graphical bitmap image at position x horizontally and y vertically   *
* (This function is optimized for 16 bits per pixel format, it has to be       *
*  adapted for any other bits per pixel format)                                *
*   Parameter:      xs:        horizontal start position                       *
*                   xe:        horizontal end position                         *
*                   ys:        vertical start position                         *
*                   ye:        vertical end position                           *
*                   bitmap:   address at which the bitmap data resides         *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_BitmapSize (unsigned int xs, unsigned int xe, unsigned int ys, unsigned int ye, const unsigned short *bitmap) {
  int i;
  unsigned short *bitmap_ptr = (unsigned short *)bitmap;

  GLCD_SetWindowSize (xs, xe, ys, ye);

  wr_cmd(0x22);
  wr_dat_start();

    for (i = 0; i < ((xe-xs+1)*(ye-ys)); i++) {
      wr_dat_only (bitmap_ptr[i]);
  }
  wr_dat_stop();
}


/*******************************************************************************
* Scroll content of the whole display for dy pixels vertically                 *
*   Parameter:      dy:       number of pixels for vertical scroll             *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_ScrollVertical (unsigned int dy) {
#if (LANDSCAPE == 0)
  static unsigned int y = 0;

  y = y + dy;
  while (y >= HEIGHT)
    y -= HEIGHT;

//  if (Himax) {
    wr_reg(0x01, 0x08);
    wr_reg(0x14, y>>8);                 /* VSP MSB                            */
    wr_reg(0x15, y&0xFF);               /* VSP LSB                            */
//  }
//  else {
//    wr_reg(0x6A, y);
//    wr_reg(0x61, 3);
//  }
#endif
}


/*******************************************************************************
* Write a command to the LCD controller                                        *
*   Parameter:      cmd:      command to write to the LCD                      *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_WrCmd (unsigned char cmd) {
  wr_cmd (cmd);
}


/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_WrReg (unsigned char reg, unsigned short val) {
  wr_reg (reg, val);
}


/*******************************************************************************
* Start GLCD sequence                                                          *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_Start (void) {
    wr_cmd(0x22);
    wr_dat_start();
}


/*******************************************************************************
* Write GLCD data                                                              *
*   Parameter:        16 bits video data                                       *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_Write (unsigned short data) {
    wr_dat_only (data);
}


/*******************************************************************************
* End GLCD sequence                                                            *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_End (void) {
    wr_dat_stop();
}


/*******************************************************************************
* Write a boarder to the LCD screen in foreground color                        *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_Boarder (void) {
  int i;
    for (i = 0; i < WIDTH; i++)
    {
		GLCD_PutPixel (i, 0);
		GLCD_PutPixel (i, HEIGHT - 1);

		if (i < HEIGHT)
		{
			GLCD_PutPixel (0, i);
			GLCD_PutPixel (WIDTH - 1, i);
		}
    }
}


/*******************************************************************************
* Draw box filled with color                                                   *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   w:        window width in pixels                           *
*                   h:        window height in pixels                          *
*                   color:    box color                                        * 
*   Return:                                                                    *
*******************************************************************************/
void GLCD_Box (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned short color) {
  unsigned int i;

  GLCD_SetWindow (x, y, w, h);

  wr_cmd(0x22);
  wr_dat_start();
  for(i = 0; i < (w*h); i++){
      wr_dat_only (color);
  }
  wr_dat_stop();
}


/*******************************************************************************
* Draw box filled with color                                                   *
*   Parameter:      xs:        horizontal start position                       *
*                   xe:        horizontal end position                         *
*                   ys:        vertical start position                         *
*                   ye:        vertical end position                           *
*                   color:     box color                                       *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_BoxSize (unsigned int xs, unsigned int xe, unsigned int ys, unsigned int ye, unsigned short color) {
  int i;

  GLCD_SetWindowSize (xs, xe, ys, ye);

  wr_cmd(0x22);
  wr_dat_start();

  for (i = 0; i < ((xe-xs+1)*(ye-ys)); i++) {
      wr_dat_only (color);
  }
  wr_dat_stop();
}

/*******************************************************************************
* Display graphical bitmap image at position x horizontally and y vertically   *
* but modify specified colors in original                                      *
* (This function is optimized for 16 bits per pixel format, it has to be       *
*  adapted for any other bits per pixel format)                                *
*   Parameter:      xs:        horizontal start position                       *
*                   xe:        horizontal end position                         *
*                   ys:        vertical start position                         *
*                   ye:        vertical end position                           *
*                   bitmap:    address at which the bitmap data resides        *
*                   n:         number of colors to change                      *
*                   color:     pointer to area of colors to change             *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_ChangeBitMap (unsigned int xs, unsigned int xe, unsigned int ys, unsigned int ye, 
                        const unsigned short * bitmap, unsigned int n, unsigned short * color)
{
    unsigned int i, j;
    unsigned short col;

    GLCD_SetWindowSize (xs, xe, ys, ye);
    
    wr_cmd(0x22);
    wr_dat_start();
	for(i = 0; i < ((xe-xs+1)*(ye-ys)); i++)
	{
		col = (bitmap[i] & 0xFFFF);
        // Check for each color to be changed
        for(j = 0; j < n; j++)
        {
            if(col == color[j * 2])
                col = color[(j * 2)+1];   // Change color
        }

 		wr_dat_only(col);
	}
    wr_dat_stop();
}

/******************************************************************************/
