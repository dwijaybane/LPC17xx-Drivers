/******************************************************************//**
* @file		lpc_ssp_glcd.c
* @brief	Contains all functions support for SSP based GLCD
*           library on LPC17xx
* @version	1.0
* @date		18. Dec. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup GLCD
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_ssp_glcd.h"
#include "math.h"
#include "Font_24x16.h"
#include "Font_5x7.h"
#include "key1.h"
#include "key2.h"
#include "key3.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

/******************************************************************************/
static volatile uint16_t TextColor = Black, BackColor = White;

// Swap two bytes
#define SWAP(x,y) do { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); } while(0)
#define bit_test(D,i) (D & (0x01 << i))

/** @addtogroup GLCD_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */

/*********************************************************************//**
 * @brief	    This function controls Backlight
 * @param[in]	NewState	ENABLE/DISABLE the Backlight
 * @return 		None
 **********************************************************************/
void GLCD_Backlight (FunctionalState NewState)
{
	if(NewState)
	{
		GPIO_SetValue(2, LCD_BK);
	}
	else
	{
		GPIO_ClearValue(2, LCD_BK);
	}
}


/*********************************************************************//**
 * @brief	    This function resets GLCD
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void GLCD_Reset (void)
{
	GPIO_SetValue(0, LCD_RST);
	delay_ms(2);
	GPIO_ClearValue(0, LCD_RST);  //reset low
	delay_ms(4);
	GPIO_SetValue(0, LCD_RST);  //reset low
}


/*********************************************************************//**
 * @brief	    This function controls GLCD Output
 * @param[in]	drv	    Output Format
 * 						- TOP_LEFT
 * 						- TOP_RIGHT
 * 						- BOTTOM_LEFT
 * 						- BOTTOM_RIGHT
 * @return 		None
 **********************************************************************/
void GLCD_Driver_OutCtrl (DRIVER_OUT_Type drv)
{
	Write_Command_Glcd(0x01);    // Driver Output Control

	switch (drv)
	{
	case TOP_LEFT:
		Write_Data_Glcd(0x72EF);       // Page 36-39 of SSD2119 datasheet
		break;

	case TOP_RIGHT:
		Write_Data_Glcd(0x70EF);       // Page 36-39 of SSD2119 datasheet
		break;

	case BOTTOM_LEFT:
		Write_Data_Glcd(0x32EF);       // Page 36-39 of SSD2119 datasheet
		break;

	case BOTTOM_RIGHT:
		Write_Data_Glcd(0x30EF);       // Page 36-39 of SSD2119 datasheet
		break;

	default:
		break;
	}
}


/*********************************************************************//**
 * @brief	    Set draw window region to required width and height
 *              as well as location
 * @param[in]	x        horizontal position
 *              y        vertical position
 *              w        width of bitmap
 *              h        height of bitmap
 * @return 		None
 **********************************************************************/
void GLCD_Window (uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	Write_Command_Glcd(0x45);      /* Horizontal GRAM Start Address      */
	Write_Data_Glcd(x);

	Write_Command_Glcd(0x46);      /* Horizontal GRAM End   Address (-1) */
	Write_Data_Glcd(x+w-1);

	Write_Command_Glcd(0x44);      /* Vertical   GRAM Start Address      */
	Write_Data_Glcd(y);

	Write_Command_Glcd(0x44);      /* Vertical   GRAM End   Address (-1) */
	Write_Data_Glcd((y+h-1)<<8);
}


/*********************************************************************//**
 * @brief	    This function Sets Cursor to to desired location
 * @param[in]	x        horizontal position
 *              y        vertical position
 *              w        width of bitmap
 *              h        height of bitmap
 * @return 		None
 **********************************************************************/
void GLCD_Set_Loc (uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	GLCD_Window (x,y,w,h);

	Write_Command_Glcd(0x4E);    // GDDRAM Horizontal
	Write_Data_Glcd(x);       // Page 58 of SSD2119 datasheet

	Write_Command_Glcd(0x4F);    // GDDRAM Vertical
	Write_Data_Glcd(y);       // Page 58 of SSD2119 datasheet

	Write_Command_Glcd(0x22);    // RAM data write/read
}


/*********************************************************************//**
 * @brief	    This function Sets Cursor to Home
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void GLCD_Display_Home (void)
{
	GLCD_Set_Loc (0,0,320,240);
}


/*********************************************************************//**
 * @brief	    This function Initializes GLCD
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void GLCD_Init (void)
{
	GPIO_SetDir(2, LCD_RS, 1);   // RS as output
	GPIO_SetDir(0, LCD_RST, 1);  // Reset as Output
	GPIO_SetDir(2, LCD_BK, 1);   // Backlight as output

	GLCD_Reset();                // Reset GLCD

	Write_Command_Glcd(0x28);    // VCOM OTP
	Write_Data_Glcd(0x0006);     // Page 55-56 of SSD2119 datasheet

	Write_Command_Glcd(0x00);    // start Oscillator
	Write_Data_Glcd(0x0001);     // Page 36 of SSD2119 datasheet

	Write_Command_Glcd(0x10);    // Sleep mode
	Write_Data_Glcd(0x0000);     // Page 49 of SSD2119 datasheet

	GLCD_Driver_OutCtrl (TOP_LEFT);

	Write_Command_Glcd(0x02);    // LCD Driving Waveform Control
	Write_Data_Glcd(0x0600);     // Page 40-42 of SSD2119 datasheet

	Write_Command_Glcd(0x03);    // Power Control 1
	Write_Data_Glcd(0x6A38);     // Page 43-44 of SSD2119 datasheet 6A38

	Write_Command_Glcd(0x11);    // Entry Mode
	Write_Data_Glcd(0x6870);     // Page 50-52 of SSD2119 datasheet

	Write_Command_Glcd(0x0F);    // Gate Scan Position
	Write_Data_Glcd(0x0000);     // Page 49 of SSD2119 datasheet

	Write_Command_Glcd(0x0B);    // Frame Cycle Control
	Write_Data_Glcd(0x5308);     // Page 45 of SSD2119 datasheet

	Write_Command_Glcd(0x0C);    // Power Control 2
	Write_Data_Glcd(0x0003);     // Page 47 of SSD2119 datasheet

	Write_Command_Glcd(0x0D);    // Power Control 3
	Write_Data_Glcd(0x000A);     // Page 48 of SSD2119 datasheet

	Write_Command_Glcd(0x0E);    // Power Control 4
	Write_Data_Glcd(0x2E00);     // Page 48 of SSD2119 datasheet

	Write_Command_Glcd(0x1E);    // Power Control 5
	Write_Data_Glcd(0x00BE);     // Page 53 of SSD2119 datasheet

	Write_Command_Glcd(0x25);    // Frame Frequency Control
	Write_Data_Glcd(0x8000);     // Page 53 of SSD2119 datasheet  8000

	Write_Command_Glcd(0x26);    // Analog setting
	Write_Data_Glcd(0x7800);     // Page 54 of SSD2119 datasheet

	Write_Command_Glcd(0x4E);    // Ram Address Set
	Write_Data_Glcd(0x0000);     // Page 58 of SSD2119 datasheet

	Write_Command_Glcd(0x4F);    // Ram Address Set
	Write_Data_Glcd(0x0000);     // Page 58 of SSD2119 datasheet

	Write_Command_Glcd(0x12);    // Sleep mode
	Write_Data_Glcd(0x08D9);     // Page 49 of SSD2119 datasheet

	// Gamma Control (R30h to R3Bh) -- Page 56 of SSD2119 datasheet
	Write_Command_Glcd(0x30);
	Write_Data_Glcd(0x0000);

	Write_Command_Glcd(0x31);
	Write_Data_Glcd(0x0104);

	Write_Command_Glcd(0x32);
	Write_Data_Glcd(0x0100);

	Write_Command_Glcd(0x33);
	Write_Data_Glcd(0x0305);

	Write_Command_Glcd(0x34);
	Write_Data_Glcd(0x0505);

	Write_Command_Glcd(0x35);
	Write_Data_Glcd(0x0305);

	Write_Command_Glcd(0x36);
	Write_Data_Glcd(0x0707);

	Write_Command_Glcd(0x37);
	Write_Data_Glcd(0x0300);

	Write_Command_Glcd(0x3A);
	Write_Data_Glcd(0x1200);

	Write_Command_Glcd(0x3B);
	Write_Data_Glcd(0x0800);

	Write_Command_Glcd(0x07);      // Display Control
	Write_Data_Glcd(0x0033);       // Page 45 of SSD2119 datasheet

	delay_ms(5);

	Write_Command_Glcd(0x22);    // RAM data write/read
}


/*********************************************************************//**
 * @brief	    Draw a pixel in foreground color
 * @param[in]	x        horizontal position
 *              y        vertical position
 * @return 		None
 **********************************************************************/
void GLCD_PutPixel (uint16_t x, uint16_t y, uint16_t color)
{
	Write_Command_Glcd(0x4E);     /* GDDRAM Horizontal */
	Write_Data_Glcd(x);

	Write_Command_Glcd(0x4F);     /* GDDRAM Vertical */
	Write_Data_Glcd(y);

	Write_Command_Glcd(0x22);      /* RAM data write     */
	Write_Data_Glcd(color);
}


/*********************************************************************//**
 * @brief	    Set foreground color
 * @param[in]	color    foreground color
 * @return 		None
 **********************************************************************/
void GLCD_SetTextColor (uint16_t color)
{
	TextColor = color;
}


/*********************************************************************//**
 * @brief	    Set background color
 * @param[in]	color    background color
 * @return 		None
 **********************************************************************/
void GLCD_SetBackColor (uint16_t color)
{
	BackColor = color;
}


/*********************************************************************//**
 * @brief	    Start of data writing to LCD controller
 * @param[in]	None
 * @return 		None
 **********************************************************************/
static __INLINE void wr_dat_start (void)
{
	CS_Force1 (LPC_SSP1, DISABLE);
	GPIO_SetValue(2, LCD_RS);  // select data mode
}


/*********************************************************************//**
 * @brief	    Stop of data writing to LCD controller
 * @param[in]	None
 * @return 		None
 **********************************************************************/
static __INLINE void wr_dat_stop (void)
{
	CS_Force1 (LPC_SSP1, ENABLE);
}


/*********************************************************************//**
 * @brief	    Data writing to LCD controller
 * @param[in]	c     data to be written
 * @return 		None
 **********************************************************************/
static __INLINE void wr_dat_only (uint16_t c)
{
	SSP_DATA_SETUP_Type xferConfig;

	Tx_Buf1[0] = (uchar)(c>>8);    // 1st byte extract
	Tx_Buf1[1] = (uchar) c;        // 2nd byte extract

	xferConfig.tx_data = Tx_Buf1;               /* Send Instruction Byte    */
	xferConfig.rx_data = NULL;
	xferConfig.length = 2;
	SSP_ReadWrite(LPC_SSP1, &xferConfig, SSP_TRANSFER_POLLING);
}


/*********************************************************************//**
 * @brief	    Clear display
 * @param[in]	color    display clearing color
 * @return 		None
 **********************************************************************/
void GLCD_Clear (uint16_t color)
{
	unsigned int   i;

	GLCD_Window (0,0,320,240);    // Window Max

	Write_Command_Glcd(0x4E);     /* GDDRAM Horizontal */
	Write_Data_Glcd(0);

	Write_Command_Glcd(0x4F);     /* GDDRAM Vertical */
	Write_Data_Glcd(0);

	Write_Command_Glcd(0x22);
	wr_dat_start();
	for(i = 0; i < (WIDTH*HEIGHT); i++)
		wr_dat_only(color);
	wr_dat_stop();
}


/*********************************************************************//**
 * @brief	    Draw character on given position
 * @param[in]	x       horizontal position
 *              y       vertical position
 *              c       pointer to character bitmap
 * @return 		None
 **********************************************************************/
void GLCD_Draw_Char (uint16_t x, uint16_t y, uint16_t *c)
{
	int idx = 0, i, j;

	x = x-CHAR_W;

	Write_Command_Glcd(0x45);      /* Horizontal GRAM Start Address      */
	Write_Data_Glcd(x);

	Write_Command_Glcd(0x46);      /* Horizontal GRAM End   Address (-1) */
	Write_Data_Glcd(x+CHAR_W-1);

	Write_Command_Glcd(0x44);      /* Vertical   GRAM Start Address      */
	Write_Data_Glcd(y);

	Write_Command_Glcd(0x44);      /* Vertical   GRAM End   Address (-1) */
	Write_Data_Glcd((y+CHAR_H-1)<<8);

	Write_Command_Glcd(0x4E);     /* GDDRAM Horizontal */
	Write_Data_Glcd(x);

	Write_Command_Glcd(0x4F);     /* GDDRAM Vertical */
	Write_Data_Glcd(y);

	Write_Command_Glcd(0x22);

	wr_dat_start();
	for (j = 0; j < CHAR_H; j++)
	{
		for (i = 0; i<CHAR_W; i++)
		{
			if((c[idx] & (1 << i)) == 0x00)
			{
				wr_dat_only(BackColor);
			}
			else
			{
				wr_dat_only(TextColor);
			}
		}
		c++;
	}
	wr_dat_stop();
}


/*********************************************************************//**
 * @brief	    Disply character on given line
 * @param[in]	ln       line number
 *              col      column number
 *              c        ascii character
 * @return 		None
 **********************************************************************/
void GLCD_Display_Char (uint16_t ln, uint16_t col, uchar c)
{
	c -= 32;
	GLCD_Draw_Char(col * CHAR_W, ln * CHAR_H, (uint16_t *)&Font_24x16[c * CHAR_H]);
}


/*********************************************************************//**
 * @brief	    Disply string on given line
 * @param[in]	ln       line number
 *              col      column number
 *              s        pointer to string
 * @return 		None
 **********************************************************************/
void GLCD_Display_String (uint16_t ln, uint16_t col, uchar *s)
{
	GLCD_Window(0,0,320,240);  // Window Max
	while (*s)
	{
		GLCD_Display_Char(ln, col++, *s++);
	}
}


/*********************************************************************//**
 * @brief	    Clear given line
 * @param[in]	ln       line number
 * @return 		None
 **********************************************************************/
void GLCD_ClearLn (uint16_t ln)
{
	GLCD_Window(0,0,320,240);  // Window Max
	GLCD_Display_String(ln, 0, "                    ");
}


/*********************************************************************//**
 * @brief	    Display graphical bitmap image at position x horizontally
 *              and y vertically (This function is optimized for
 *              16 bits per pixel format, it has to be adapted for
 *              any other bits per pixel format)
 * @param[in]	x        horizontal position
 *              y        vertical position
 *              w        width of bitmap
 *              h        height of bitmap
 *              bitmap   address at which the bitmap data resides
 * @return 		None
 **********************************************************************/
void GLCD_Bitmap (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
	uint32_t i,j,k;

	GLCD_Set_Loc (x,y,w,h);

	wr_dat_start();
	k = 16;
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			wr_dat_only(bitmap[k++]);
		}
	}
	wr_dat_stop();
}


/*********************************************************************//**
 * @brief	    F at position x horizontally
 *              and y vertically (This function is optimized for
 *              16 bits per pixel format, it has to be adapted for
 *              any other bits per pixel format)
 * @param[in]	x        horizontal position
 *              y        vertical position
 *              w        width of bitmap
 *              h        height of bitmap
 *              color    window color
 * @return 		None
 **********************************************************************/
void GLCD_Window_Fill (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	uint32_t i,j;

	GLCD_Set_Loc (x,y,w,h);

	wr_dat_start();
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			wr_dat_only(color);
		}
	}
	wr_dat_stop();
}


/*********************************************************************//**
 * @brief	    Draw a line on a graphic LCD using Bresenham's
 *              line drawing algorithm
 * @param[in]	(x1, y1)   the start coordinate
 *              (x2, y2)   the end coordinate
 *              color      line color
 * @return 		None
 **********************************************************************/
void GLCD_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	int16_t  x, y, addx, addy, dx, dy;
	int32_t P,i;

	dx = abs((int16_t)(x2 - x1));
	dy = abs((int16_t)(y2 - y1));
	x = x1;
	y = y1;

	if(x1 > x2)
		addx = -1;
	else
		addx = 1;
	if(y1 > y2)
		addy = -1;
	else
		addy = 1;


	if(dx >= dy)
	{
		P = 2*dy - dx;

		for(i=0; i<=dx; ++i)
		{
			GLCD_PutPixel(x, y, color);

			if(P < 0)
			{
				P += 2*dy;
				x += addx;
			}
			else
			{
				P += 2*dy - 2*dx;
				x += addx;
				y += addy;
			}
		}
	}
	else
	{
		P = 2*dx - dy;

		for(i=0; i<=dy; ++i)
		{
			GLCD_PutPixel(x, y, color);

			if(P < 0)
			{
				P += 2*dx;
				y += addy;
			}
			else
			{
				P += 2*dx - 2*dy;
				x += addx;
				y += addy;
			}
		}
	}
}


/*********************************************************************//**
 * @brief	    Draw a Rectangle on a graphic LCD
 * @param[in]	(x1, y1)     the start coordinate
 *              (x2, y2)     the end coordinate
 *              fill         Fill Rectangle TRUE/FALSE or ON/OFF
 *              color        Boundary color
 *              fill_color   fill color
 * @return 		None
 **********************************************************************/
void GLCD_Rect(COORDINATE_Type *p1, COORDINATE_Type *p2, Bool fill, uint16_t color, uint16_t fill_color)
{
	int16_t  width,height;                          // Find the y min and max

	if(fill)
	{
		if(p2->x > p1->x)
		{
			if(p2->y > p1->y)
			{
				width = p2->x - p1->x;
				height = p2->y - p1->y;
				GLCD_Window_Fill(p1->x+1,p1->y+1,width-1,height-1,fill_color);
			}
			else
			{
				width = p2->x - p1->x;
				height = p1->y - p2->y;
				GLCD_Window_Fill(p1->x+1,p2->y+1,width-1,height-1,fill_color);
			}
		}
		else
		{
			if(p2->y > p1->y)
			{
				width = p1->x - p2->x;
				height = p2->y - p1->y;
				GLCD_Window_Fill(p2->x+1,p1->y+1,width-1,height-1,fill_color);
			}
			else
			{
				width = p1->x - p2->x;
				height = p1->y - p2->y;
				GLCD_Window_Fill(p2->x+1,p2->y+1,width-1,height-1,fill_color);
			}
		}
		fill = NO;
	}
	if(!fill)
	{
		GLCD_Line(p1->x, p1->y, p2->x, p1->y, color);      // Draw the outer border 4 sides
		GLCD_Line(p1->x, p2->y, p2->x, p2->y, color);
		GLCD_Line(p1->x, p1->y, p1->x, p2->y, color);
		GLCD_Line(p2->x, p1->y, p2->x, p2->y, color);
	}
}


/*********************************************************************//**
 * @brief	    Draw a frame on a graphic LCD
 * @param[in]	(x1, y1)     the start coordinate
 *              (x2, y2)     the end coordinate
 *              frame_width  Total Width of Frame
 *              color        Boundary color
 *              fill_color   Frame fill color
 * @return 		None
 **********************************************************************/
void GLCD_Frame(COORDINATE_Type *p1, COORDINATE_Type *p2, int16_t frame_width, uint16_t color, uint16_t fill_color)
{
	int16_t fw;                          // Find the y min and max

	fw = frame_width;

	while(fw)
	{
		if(fw < frame_width)
		{
			GLCD_Line(p1->x, p1->y+fw, p2->x, p1->y+fw, fill_color);      // Draw the interior 4 sides
			GLCD_Line(p1->x, p2->y-fw, p2->x, p2->y-fw, fill_color);
			GLCD_Line(p1->x+fw, p1->y, p1->x+fw, p2->y, fill_color);
			GLCD_Line(p2->x-fw, p1->y, p2->x-fw, p2->y, fill_color);
			fw--;
		}
		else
		{
			GLCD_Line(p1->x, p1->y+fw, p2->x, p1->y+fw, color);      // inner border 4 sides
			GLCD_Line(p1->x, p2->y-fw, p2->x, p2->y-fw, color);
			GLCD_Line(p1->x+fw, p1->y, p1->x+fw, p2->y, color);
			GLCD_Line(p2->x-fw, p1->y, p2->x-fw, p2->y, color);
			fw--;
		}
	}

	if(!fw)
	{
		GLCD_Line(p1->x, p1->y, p2->x, p1->y, color);      // Draw the outer border 4 sides
		GLCD_Line(p1->x, p2->y, p2->x, p2->y, color);
		GLCD_Line(p1->x, p1->y, p1->x, p2->y, color);
		GLCD_Line(p2->x, p1->y, p2->x, p2->y, color);
	}
}


/*********************************************************************//**
 * @brief	    Draw a rectangle/frame on a graphic LCD
 * @param[in]	(x1, y1)     the start coordinate
 *              (x2, y2)     the end coordinate
 *              (x3, y3)     the end coordinate
 *              fill         Fill Triangle TRUE/FALSE or ON/OFF
 *              color        Boundary color
 *              fill_color   Triangle Fill Color
 * @return 		None
 **********************************************************************/
void GLCD_Triangle(COORDINATE_Type *p1, COORDINATE_Type *p2, COORDINATE_Type *p3,COLORCFG_Type *cfg)
{
    if(cfg->fill)
    {
    	uint16_t t1x,t2x,y,minx,maxx,t1xp,t2xp;
    	Bool changed1 = FALSE;
    	Bool changed2 = FALSE;
    	int16_t signx1,signx2,dx1,dy1,dx2,dy2;
    	uint16_t e1,e2;
        // Sort vertices
    	if (p1->y > p2->y) { SWAP(p1->y,p2->y); SWAP(p1->x,p2->x); }
    	if (p1->y > p3->y) { SWAP(p1->y,p3->y); SWAP(p1->x,p3->x); }
    	if (p2->y > p3->y) { SWAP(p2->y,p3->y); SWAP(p2->x,p3->x); }

    	t1x=t2x=p1->x; y=p1->y;   // Starting points

    	dx1 = (int16_t)(p2->x - p1->x);
    	if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
    	dy1 = (int16_t)(p2->y - p1->y);

    	dx2 = (int8_t)(p3->x - p1->x);
    	if(dx2<0) { dx2=-dx2; signx2=-1; } else signx2=1;
    	dy2 = (int8_t)(p3->y - p1->y);

    	if (dy1 > dx1)
    	{   // swap values
            SWAP(dx1,dy1);
    		changed1 = TRUE;
    	}
    	if (dy2 > dx2)
    	{   // swap values
            SWAP(dy2,dx2);
    		changed2 = TRUE;
    	}

    	e2 = (uint16_t)(dx2>>1);
        // Flat top, just process the second half
        if(p1->y==p2->y) goto next;
        e1 = (uint16_t)(dx1>>1);

    	for (uint8_t i = 0; i < dx1;)
    	{
    		t1xp=0; t2xp=0;
    		if(t1x<t2x) { minx=t1x; maxx=t2x; }
    		else		{ minx=t2x; maxx=t1x; }
            // process first line until y value is about to change
    		while(i<dx1)
    		{
    			i++;
    			e1 += dy1;
    	   	   	while (e1 >= dx1)
    	   	   	{
    				e1 -= dx1;
       	   	   	   if (changed1) t1xp=signx1;//t1x += signx1;
    				else          goto next1;
    			}
    			if (changed1) break;
    			else t1x += signx1;
    		}
    	// Move line
    	next1:
            // process second line until y value is about to change
    		while (1)
    		{
    			e2 += dy2;
    			while (e2 >= dx2)
    			{
    				e2 -= dx2;
    				if (changed2) t2xp=signx2;//t2x += signx2;
    				else          goto next2;
    			}
    			if (changed2)     break;
    			else              t2x += signx2;
    		}
    	next2:
    		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
    		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
    		GLCD_Line(minx,y,maxx,y,cfg->fill_color);    // Draw line from min to max points found on the y

    		// Now increase y
    		if(!changed1) t1x += signx1;
    		t1x+=t1xp;
    		if(!changed2) t2x += signx2;
    		t2x+=t2xp;
        	y += 1;
    		if(y==p2->y) break;
       }
    	next:
    	// Second half
    	dx1 = (int8_t)(p3->x - p2->x); if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
    	dy1 = (int8_t)(p3->y - p2->y);
    	t1x=p2->x;

    	if (dy1 > dx1)
    	{   // swap values
            SWAP(dy1,dx1);
    		changed1 = TRUE;
    	} else changed1=FALSE;

    	e1 = (uint8_t)(dx1>>1);

    	for (uint8_t i = 0; i<=dx1; i++)
    	{
    		t1xp=0; t2xp=0;
    		if(t1x<t2x) { minx=t1x; maxx=t2x; }
    		else		{ minx=t2x; maxx=t1x; }
    	    // process first line until y value is about to change
    		while(i<dx1)
    		{
        		e1 += dy1;
    	   	   	while (e1 >= dx1)
    	   	   	{
    				e1 -= dx1;
       	   	   	   	if (changed1) { t1xp=signx1; break; }//t1x += signx1;
    				else          goto next3;
    			}
    			if (changed1) break;
    			else   	   	  t1x += signx1;
    			if(i<dx1) i++;
    		}
    	next3:
            // process second line until y value is about to change
    		while (t2x!=p3->x)
    		{
    			e2 += dy2;
    	   	   	while (e2 >= dx2)
    	   	   	{
    				e2 -= dx2;
    				if(changed2) t2xp=signx2;
    				else          goto next4;
    			}
    			if (changed2)     break;
    			else              t2x += signx2;
    		}
    	next4:

    		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
    		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
    		GLCD_Line(minx,y,maxx,y,cfg->fill_color);    // Draw line from min to max points found on the y
    		// Now increase y
    		if(!changed1) t1x += signx1;
    		t1x+=t1xp;
    		if(!changed2) t2x += signx2;
    		t2x+=t2xp;
        	y += 1;
    		if(y>p3->y) break;
    	}
    	cfg->fill = NO;
    }


	if(!cfg->fill && cfg->bndry)
	{
		GLCD_Line(p1->x, p1->y, p2->x, p2->y, cfg->bcolor);
		GLCD_Line(p1->x, p1->y, p3->x, p3->y, cfg->bcolor);
		GLCD_Line(p2->x, p2->y, p3->x, p3->y, cfg->bcolor);
	}
}


/*********************************************************************//**
 * @brief	    Draw a Circle with given Center and Radius
 * @param[in]	(x, y)     the center of the circle
 *              radius     the radius of the circle
 *              fill       YES or NO
 *              color      Boundary color
 * @return 		None
 **********************************************************************/
void GLCD_Circle(int16_t x, int16_t y, int16_t radius,COLORCFG_Type *cfg)
{
	int16_t a, b, P;
	a = 0;
	b = radius;
	P = 1 - radius;

	do
	{
		if(cfg->fill)
		{
			GLCD_Line(x-a, y+b, x+a, y+b, cfg->fill_color);
			GLCD_Line(x-a, y-b, x+a, y-b, cfg->fill_color);
			GLCD_Line(x-b, y+a, x+b, y+a, cfg->fill_color);
			GLCD_Line(x-b, y-a, x+b, y-a, cfg->fill_color);
		}

		if(P < 0)
			P+= 3 + 2*a++;
		else
			P+= 5 + 2*(a++ - b--);
	} while(a <= b);

	cfg->fill = NO;
	a = 0;
	b = radius;
	P = 1 - radius;
	do
	{
		if(!cfg->fill && cfg->bndry)
		{
			GLCD_PutPixel(a+x, b+y, cfg->bcolor);
			GLCD_PutPixel(b+x, a+y, cfg->bcolor);
			GLCD_PutPixel(x-a, b+y, cfg->bcolor);
			GLCD_PutPixel(x-b, a+y, cfg->bcolor);
			GLCD_PutPixel(b+x, y-a, cfg->bcolor);
			GLCD_PutPixel(a+x, y-b, cfg->bcolor);
			GLCD_PutPixel(x-a, y-b, cfg->bcolor);
			GLCD_PutPixel(x-b, y-a, cfg->bcolor);
		}

		if(P < 0)
			P+= 3 + 2*a++;
		else
			P+= 5 + 2*(a++ - b--);
	} while(a <= b);
}


/*********************************************************************//**
 * @brief	    Write text on a graphic LCD
 * @param[in]	(x,y)      The upper left coordinate of the first letter
 *              textptr    A pointer to an array of text to display
 *              row        Number of rows of pixels
 *              col        Number of Colums of pixels
 *              font       font 2 dimentional array
 *              size       The size of the text: 1 = 5x7, 2 = 10x14, ...
 *              color      font color
 * @return 		None
 **********************************************************************/
void GLCD_Text(int16_t x, int16_t y, uint8_t* textptr, uint16_t length, uint8_t row, uint8_t col, int8_t (*font)[row], int8_t size, uint16_t color)
{
   int16_t i, j, k, l, m;                     // Loop counters
   uint8_t pixelData[row];                     // Stores character data

   for(i=0; i<length; ++i, ++x) // Loop through the passed string
   {
      memcpy(pixelData, font[textptr[i]-' '], row);

      if(x+row*size >= 320)          // Performs character wrapping
      {
         x = 0;                           // Set x at far left position
         y += row*size + 1;                 // Set y at next position down
      }
      for(j=0; j<row; ++j, x+=size)         // Loop through character byte data
      {
         for(k=0; k<col*size; ++k)          // Loop through the vertical pixels
         {
            if(bit_test(pixelData[j], k)) // Check if the pixel should be set
            {
               for(l=0; l<size; ++l)      // The next two loops change the
               {                          // character's size
                  for(m=0; m<size; ++m)
                  {
                     GLCD_PutPixel(x+m, y+k*size+l, color); // Draws the pixel
                  }
               }
            }
         }
      }
   }
}


/*********************************************************************//**
 * @brief		Modified version of Standard Printf statement
 *
 * @par			Supports standard formats "%c %s %d %x"
 * 				"%d" and "%x" requires non-standard qualifiers,"%dfn, %xfn":-
 *		        f supplies a fill character
 *		        n supplies a field width
 *
 *		        ENABLE RTC_SUPPORT in lpc17xx_uart.h for RTC Features
 *
 *				Supports custom formats  "%b  %u %t %y %a"
 *				"%b"	prints a 2 digit BCD value with leading zero
 *				"%u"	prints the 16 bit unsigned integer in hex format
 *				"%t"    prints current time
 *				"%y"    prints current date
 *				"%a"    prints alarm time and date
 * @param[in]	UARTx	Selected UART peripheral used to send data,
 * 				should be:
 *  			- LPC_UART0: UART0 peripheral
 * 				- LPC_UART1: UART1 peripheral
 * 				- LPC_UART2: UART2 peripheral
 * 				- LPC_UART3: UART3 peripheral
 * @param[in] 	*format Character format
 * @param[in]   ...  <multiple argument>
 *
 * @return 		return with valid character or nothing
 **********************************************************************/
int16 gprintf(int16_t x, int16_t y, int8_t size, uint16_t color, const char *format, ...)
{
	uchar hex[]= "0123456789ABCDEF";
	unsigned int width_dec[10] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};
	unsigned int width_hex[10] = { 0x1, 0x10, 0x100, 0x1000, 0x10000, 0x100000, 0x1000000, 0x10000000};
	unsigned int temp;

	uchar format_flag, fill_char;
	ulong32 u_val, div_val;
	uint16 base;

	uchar *ptr;
#ifdef RTC_MODE
	RTC_TIME_Type FullTime;
#endif
	va_list ap;
	va_start(ap, format);

	for(;;)
	{
		while((format_flag = *format++) != '%')      // until full format string read
		{
			if(!format_flag)
			{                        // until '%' or '\0'
				return (0);
			}

		    if(x+5*size >= 320)          // Performs character wrapping
		    {
		       x = 0;                           // Set x at far left position
		       y += 7*size + 1;                 // Set y at next position down
		    }
			GLCD_Text(x,y,&format_flag,1,5,7,default5x7,size,color);
			x=x+5*size+1;
		}

		switch(format_flag = *format++)
		{
			case 'c':
				format_flag = va_arg(ap, int);
			    if(x+5*size >= 320)          // Performs character wrapping
			    {
			       x = 0;                           // Set x at far left position
			       y += 7*size + 1;                 // Set y at next position down
			    }
				GLCD_Text(x,y,&format_flag,1,5,7,default5x7,size,color);
				x=x+5*size+1;

				continue;

			default:
			    if(x+5*size >= 320)          // Performs character wrapping
			    {
			       x = 0;                           // Set x at far left position
			       y += 7*size + 1;                 // Set y at next position down
			    }
				GLCD_Text(x,y,&format_flag,1,5,7,default5x7,size,color);
				x=x+5*size+1;

        		continue;

			case 'b':
				format_flag = va_arg(ap,int);
			    if(x+5*size >= 320)          // Performs character wrapping
			    {
			       x = 0;                           // Set x at far left position
			       y += 7*size + 1;                 // Set y at next position down
			    }
				GLCD_Text(x,y,&(hex[(uint16)format_flag >> 4]),1,5,7,default5x7,size,color);
				x=x+5*size+1;
			    if(x+5*size >= 320)          // Performs character wrapping
			    {
			       x = 0;                           // Set x at far left position
			       y += 7*size + 1;                 // Set y at next position down
			    }
				GLCD_Text(x,y,&(hex[(uint16)format_flag & 0x0F]),1,5,7,default5x7,size,color);
				x=x+5*size+1;

				continue;

			case 's':
				ptr = va_arg(ap, schar *);
				while(*ptr)
				{
				    if(x+5*size >= 320)          // Performs character wrapping
				    {
				       x = 0;                           // Set x at far left position
				       y += 7*size + 1;                 // Set y at next position down
				    }
					GLCD_Text(x,y,&(*ptr++),1,5,7,default5x7,size,color);
					x=x+5*size+1;
				}

				continue;
#ifdef RTC_MODE
			case 't':
				RTC_GetFullTime (LPC_RTC, &FullTime);
			    gprintf(x,y,size,color, "%d02:%d02:%d02",FullTime.HOUR,FullTime.MIN,FullTime.SEC);

				continue;

			case 'y':
				RTC_GetFullTime (LPC_RTC, &FullTime);
			    gprintf(x,y,size,color, "%d02/%d02/%d04",FullTime.DOM,FullTime.MONTH,FullTime.YEAR);

				continue;

			case 'a':
				RTC_GetFullAlarmTime (LPC_RTC, &FullTime);
				gprintf(x,y,size,color, "Time: %d02:%d02:%d02",FullTime.HOUR,FullTime.MIN,FullTime.SEC);
				gprintf(x,y,size,color, "  Date: %d02/%d02/%d04",FullTime.DOM,FullTime.MONTH,FullTime.YEAR);

				continue;
#endif
			case 'u':
				base = 16;
				div_val = 0x100000;
				u_val = va_arg(ap, uint32_t);
				do
				{
				    if(x+5*size >= 320)          // Performs character wrapping
				    {
				       x = 0;                           // Set x at far left position
				       y += 7*size + 1;                 // Set y at next position down
				    }
					GLCD_Text(x,y,&(hex[u_val/div_val]),1,5,7,default5x7,size,color);
					x=x+5*size+1;
					u_val %= div_val;
					div_val /= base;
				}while(div_val);

				continue;

			case 'd':
				base = 10;
				fill_char = *format++;
				format_flag = ( *format++ ) - '1';
				div_val = width_dec[format_flag];
				u_val = va_arg(ap,int);

				if(((int)u_val) < 0)
				{
					u_val = - u_val;    // applied to unsigned type, result still unsigned
					temp = '-';
				    if(x+5*size >= 320)          // Performs character wrapping
				    {
				       x = 0;                           // Set x at far left position
				       y += 7*size + 1;                 // Set y at next position down
				    }
				    GLCD_Text(x,y,&temp,1,5,7,default5x7,size,color);
				    x=x+5*size+1;
				}

				goto  CONVERSION_LOOP;

			case 'x':
				base = 16;
				fill_char = *format++;
				format_flag = (*format++) - '1';
				div_val = width_hex[format_flag];
				u_val = va_arg(ap, int);

				CONVERSION_LOOP:
				while(div_val > 1 && div_val > u_val)
				{
					div_val /= base;
				    if(x+5*size >= 320)          // Performs character wrapping
				    {
				       x = 0;                           // Set x at far left position
				       y += 7*size + 1;                 // Set y at next position down
				    }
					GLCD_Text(x,y,&fill_char,1,5,7,default5x7,size,color);
					x=x+5*size+1;
				}

				do
				{
				    if(x+5*size >= 320)          // Performs character wrapping
				    {
				       x = 0;                           // Set x at far left position
				       y += 7*size + 1;                 // Set y at next position down
				    }
					GLCD_Text(x,y,&(hex[u_val/div_val]),1,5,7,default5x7,size,color);
					x=x+5*size+1;
					u_val %= div_val;
					div_val /= base;
				}while(div_val);
		}// end of switch statement
	}
	return(0);
}


schar GLCD_Getche(void)
{
	schar key=0;
	ts_event ts;
	uint16_t x,y;
	Bool flag = 0,up=1;

	uint16_t good_state = 0;
	uint16_t adc_state = 0;
    uint16_t temp;

	while(1)
	{
		good_state = 0;
		adc_state = 0;

		while (good_state<3)          // while less that 3 good samples
		{
			TSC2004_Read_Values (&ts);

			x = (uint16_t)((ts.x)/11)-24;
		   	y = (uint16_t)((ts.y)/13)-36;

		   	temp = x;
		   	if (adc_state == temp)
		   	{
		   		good_state++;
		   	}
		   	else
		   	{
		   		good_state = 0;
		   	}

		   	adc_state = temp;
		}
		x = adc_state;               //Sampled X value

		switch(keybd)
		{
		case KEY1:
			if(!flag)
			{
				GLCD_Bitmap(0,133,320,107,key1);
				flag = 1;
			}
			key = Keyboard1(x,y);
			break;

		case KEY2:
			if(!flag)
			{
				GLCD_Bitmap(0,133,320,107,key2);
				flag = 1;
			}
			key = Keyboard2(x,y);
			break;

		case KEY3:
			if(!flag)
			{
				GLCD_Bitmap(0,133,320,107,key3);
				flag = 1;
			}
			key = Keyboard3(x,y);
			break;
		}

		if(key == 0)
		{
			up = 0;    // pen up done now key can be pressed
		}

		if(key == KEY1 && !up)
		{
			flag = 0;
			keybd = KEY1;
			up = 1;
		}
		else if(key == KEY2 && !up)
		{
			flag = 0;
			keybd = KEY2;
			up = 1;
		}
		else if(key == KEY3 && !up)
		{
			flag = 0;
			keybd = KEY3;
			up = 1;
		}

		if((key!=KEY1) && (key!=KEY2) && (key!=KEY3) && key && !up)
		{
			return(key);
		}
	}
}


schar Keyboard1(uint16_t x, uint16_t y)
{
	schar key;

	if(y>138 && y<159)
	{
		if(y>138 && y<154)
		{
			if(x>5 && x<23)
			{
				return 'q';
			}
			else if(x>33 && x<52)
			{
				return 'w';
			}
		}
		if(y>141 && y<159)
		{
			if(x>64 && x<84)
			{
				return 'e';
			}
			else if(x>98 && x<120)
			{
				return 'r';
			}
			else if(x>133 && x<155)
			{
				return 't';
			}
			else if(x>168 && x<189)
			{
				return 'y';
			}
			else if(x>200 && x<220)
			{
				return 'u';
			}
			else if(x>231 && x<256)
			{
				return 'i';
			}
			else if(x>266 && x<289)
			{
				return 'o';
			}
			else if(x>300 && x<320)
			{
				return 'p';
			}
		}
	}
	else if(y>170 && y<192)
	{
		if(y>170 && y<188)
		{
			if(x>20 && x<40)
			{
				return 'a';
			}
			else if(x>50 && x<70)
			{
				return 's';
			}
		}

		if(y>173 && y<192)
		{
			if(x>81 && x<103)
			{
				return 'd';
			}
			else if(x>115 && x<138)
			{
				return 'f';
			}
			else if(x>150 && x<172)
			{
				return 'g';
			}
			else if(x>184 && x<207)
			{
				return 'h';
			}
			else if(x>216 && x<238)
			{
				return 'j';
			}
			else if(x>249 && x<270)
			{
				return 'k';
			}
			else if(x>284 && x<304)
			{
				return 'l';
			}
		}
	}
	else if(y>197 && y<221)
	{
		if(x>3 && x<30 && y>197 && y<214)
		{
			key = CAPS;
			return key; // CAPS FLAG
		}

		if(y>202 && y<221)
		{
			if(x>49 && x<70)
			{
				return 'z';
			}
			else if(x>82 && x<103)
			{
				return 'x';
			}
			else if(x>114 && x<139)
			{
				return 'c';
			}
			else if(x>148 && x<172)
			{
				return 'v';
			}
			else if(x>183 && x<205)
			{
				return 'b';
			}
			else if(x>216 && x<238)
			{
				return 'n';
			}
			else if(x>249 && x<270)
			{
				return 'm';
			}
			else if(x>290 && x<320)
			{
				key = BK_SPACE;
				return key;  // Backspace flag
			}
		}
	}
	else if(x>5 && x<54 && y>230 && y<242)
	{
		key = KEY2;
		return key; // Keyboard2 flag
	}
	else if(x>67 && x<252 && y>234 && y<250)
	{
		return ' ';
	}
	else if(x>265 && x<320 && y>231 && y<250)
	{
		key = CR;
		return key;  // Carriage Return
	}
	return 0;
}


schar Keyboard2(uint16_t x, uint16_t y)
{
	schar key;

	if(y>138 && y<161)
	{
		if(y>138 && y<158)
		{
			if(x>5 && x<25)
			{
				return '1';
			}
			else if(x>35 && x<55)
			{
				return '2';
			}
		}
		if(y>141 && y<161)
		{
			if(x>68 && x<86)
			{
				return '3';
			}
			else if(x>99 && x<122)
			{
				return '4';
			}
			else if(x>132 && x<154)
			{
				return '5';
			}
			else if(x>165 && x<186)
			{
				return '6';
			}
			else if(x>198 && x<218)
			{
				return '7';
			}
			else if(x>229 && x<252)
			{
				return '8';
			}
			else if(x>263 && x<283)
			{
				return '9';
			}
			else if(x>295 && x<317)
			{
				return '0';
			}
		}
	}
	else if(y>168 && y<192)
	{
		if(y>168 && y<189)
		{
			if(x>5 && x<25)
			{
				return '-';
			}
			else if(x>35 && x<56)
			{
				return '/';
			}
		}

		if(y>173 && y<192)
		{
			if(x>66 && x<88)
			{
				return ':';
			}
			else if(x>99 && x<122)
			{
				return ';';
			}
			else if(x>132 && x<155)
			{
				return '(';
			}
			else if(x>166 && x<187)
			{
				return ')';
			}
			else if(x>198 && x<218)
			{
				return '$';
			}
			else if(x>229 && x<251)
			{
				return '&';
			}
			else if(x>262 && x<283)
			{
				return '@';
			}
			else if(x>294 && x<317)
			{
				return '"';
			}
		}
	}
	else if(y>197 && y<221)
	{
		if(x>5 && x<35 && y>197 && y<215)
		{
			key = KEY3;
			return key; // CAPS FLAG
		}

		if(y>202 && y<221)
		{
			if(x>52 && x<88)
			{
				return '.';
			}
			else if(x>96 && x<132)
			{
				return ',';
			}
			else if(x>142 && x<180)
			{
				return '?';
			}
			else if(x>189 && x<225)
			{
				return '!';
			}
			else if(x>235 && x<269)
			{
				return 0x27;   // ' single quote
			}
			else if(x>283 && x<317)
			{
				key = BK_SPACE;
				return key;  // Backspace flag
			}
		}
	}
	else if(x>5 && x<57 && y>229 && y<242)
	{
		key = KEY1;
		return key; // Keyboard2 flag
	}
	else if(x>69 && x<251 && y>234 && y<250)
	{
		return ' ';
	}
	else if(x>263 && x<318 && y>231 && y<250)
	{
		key = CR;
		return key;  // Carriage Return
	}
	return 0;
}


schar Keyboard3(uint16_t x, uint16_t y)
{
	schar key;

	if(y>138 && y<161)
	{
		if(y>138 && y<158)
		{
			if(x>5 && x<25)
			{
				return '[';
			}
			else if(x>35 && x<55)
			{
				return ']';
			}
		}
		if(y>141 && y<161)
		{
			if(x>65 && x<87)
			{
				return '{';
			}
			else if(x>97 && x<122)
			{
				return '}';
			}
			else if(x>131 && x<155)
			{
				return '#';
			}
			else if(x>164 && x<187)
			{
				return '%';
			}
			else if(x>197 && x<219)
			{
				return '^';
			}
			else if(x>230 && x<254)
			{
				return '*';
			}
			else if(x>261 && x<282)
			{
				return '+';
			}
			else if(x>295 && x<317)
			{
				return '=';
			}
		}
	}
	else if(y>168 && y<192)
	{
		if(y>168 && y<189)
		{
			if(x>5 && x<25)
			{
				return '_';
			}
			else if(x>35 && x<56)
			{
				return 0x5C; // '\' backslash
			}
		}

		if(y>173 && y<192)
		{
			if(x>64 && x<86)
			{
				return '|';
			}
			else if(x>97 && x<122)
			{
				return '~';
			}
			else if(x>131 && x<155)
			{
				return '<';
			}
			else if(x>164 && x<187)
			{
				return '>';
			}
			else if(x>198 && x<219)
			{
				return 0x7F;  // pound symbol
			}
			else if(x>230 && x<254)
			{
				return 0x81;  // Euro dollar symbol
			}
			else if(x>261 && x<282)
			{
				return 0x80;  // Yen symbol
			}
			else if(x>293 && x<317)
			{
				return 0x82;   // Center dot
			}
		}
	}
	else if(y>197 && y<221)
	{
		if(x>5 && x<34 && y>197 && y<215)
		{
			key = KEY2;
			return key; // CAPS FLAG
		}

		if(y>202 && y<221)
		{
			if(x>50 && x<84)
			{
				return '.';
			}
			else if(x>95 && x<131)
			{
				return ',';
			}
			else if(x>141 && x<179)
			{
				return '?';
			}
			else if(x>188 && x<224)
			{
				return '!';
			}
			else if(x>232 && x<269)
			{
				return 0x27;   // ' single quote
			}
			else if(x>285 && x<317)
			{
				key = BK_SPACE;
				return key;  // Backspace flag
			}
		}
	}
	else if(x>5 && x<24 && y>225 && y<241)
	{
		key = KEY1;
		return key; // Keyboard2 flag
	}
	else if(x>34 && x<57 && y>225 && y<241)
	{
		// Globe Symbol
	}
	else if(x>68 && x<251 && y>234 && y<250)
	{
		return ' ';
	}
	else if(x>262 && x<318 && y>231 && y<250)
	{
		key = CR;
		return key;  // Carriage Return
	}
	return 0;
}


uchar GLCD_Get_Line(schar s[], uint8_t lim)
{
	COORDINATE_Type point1,point2;
	schar kb;             // input character

	uint8_t pointer=0;    // Pointer in buffer
	uint8_t count=0;      // character count

	uint16_t i=6,j=6;
	Bool CAPSLOCK=0;

    /*Frame Coordinate*/
    point1.x = 0;
    point1.y = 0;
    point2.x = 320;
    point2.y = 133;
    GLCD_Frame(&point1,&point2,3,DarkGrey,LightGrey);

	while(1)
	{
		kb = GLCD_Getche();

		if(kb == CR)                        // CARRIAGE return
		{
			s[pointer] = '\0';             // put null char on last position
		    i = 0;                         // Set x at far left position
		    j += 8;                         // Set y at next position down
			break;                         // yes, exit from this loop
		}

		else if(kb == BK_SPACE)
		{
			if(pointer==0)                 // any characters entered
			{
				continue;                    // no, so get another character
			}

			i=i-6;
			GLCD_Erase(i,j,1,1,White);

			pointer--;                     // decrement pointer
			count--;                       // decrement character count

			continue;
		}

		else if((pointer < lim) && (kb!=CR) && (kb!=BK_SPACE))
		{
		    if(kb == CAPS)
		    {
		    	CAPSLOCK = !CAPSLOCK;
		    }
		    if(CAPSLOCK && (kb!=CAPS) && (keybd==KEY1))
		    {
		    	s[pointer] = to_upper(kb);               // save character and increment pointer
		    	pointer++;
		    	count++;                       // increment count

		    	if(i+5 >= 314)          // Performs character wrapping
		    	{
		    		i = 6;                           // Set x at far left position
		    		j += 8;                 // Set y at next position down
				}

		    	gprintf(i,j,1,Black,"%c",to_upper(kb));
		    	i=i+6;
		    }
		    else if(!CAPSLOCK && (kb!=CAPS) && (keybd==KEY1))
		    {
		    	s[pointer] = kb;               // save character and increment pointer
		    	pointer++;
		    	count++;                       // increment count

		    	if(i+5 >= 314)          // Performs character wrapping
		    	{
		    		i = 6;                           // Set x at far left position
		    		j += 8;                 // Set y at next position down
				}

		    	gprintf(i,j,1,Black,"%c",kb);
		    	i=i+6;
		    }
		    else if(kb!=CAPS)
		    {
		    	s[pointer] = kb;               // save character and increment pointer
		    	pointer++;
		    	count++;                       // increment count

		    	if(i+5 >= 314)          // Performs character wrapping
		    	{
		    		i = 6;                           // Set x at far left position
		    		j += 8;                 // Set y at next position down
				}

		    	gprintf(i,j,1,Black,"%c",kb);
		    	i=i+6;
		    }

			continue;                      // and get some more
		}
	}
	return(count);
}


/*********************************************************************//**
 * @brief	    Clear Given length of Characters
 * @param[in]	x        horizontal position
 *              y        vertical position
 *              size     font size multiply by 5x7 fonts
 *              length   length of string
 *              color    Erase color
 * @return 		None
 **********************************************************************/
void GLCD_Erase(uint16_t x, uint16_t y, int8_t size, uint16_t length, uint16_t color)
{
    if(x+6*size >= 320)          // Performs character wrapping
    {
       x = 0;                           // Set x at far left position
       y += 7*size + 1;                 // Set y at next position down
    }
	GLCD_Window_Fill(x,y,(6*size*length)+1,(7*size)+1,color);
}


/*********************************************************************//**
 * @brief	    Draw Bar for Bar Graph (Dependent on Bar Graph Test)
 * @param[in]	index      Bar Number (from 1 to 10)
 *              width      Bar width
 *              per        Percentage height
 *              color      Bar Color
 * @return 		None
 **********************************************************************/
void GLCD_Bar(int16_t index,uint8_t width,int16_t per,uint16_t color)
{
	/**************************************************************************
	 * Par1 : const 30 indicates  ref distance line. 28 indicates spacing between
	 * two bar graphs at multiples of 28, now as the center of bar is located
	 * half of width should be left and other half on right, so after subracting
	 * half width from center we get x point of bar.
	 * Par2 : 220 is Y ref line so (220 - bar heigth) will give y point of bar.
	 * Par4 : per*2 is scaling for 200 pixels for 100 percentage heigth.
	 **************************************************************************/
	if((per<=100) && (index<=10))
	{
		GLCD_Window_Fill(30+(index*26)-(width-(width/2)),220-(per*2),width,per*2,color);
	}
}


/*********************************************************************//**
 * @brief	    Draw Line Bar for Bar Graph (Dependent on Bar Graph Test)
 * @param[in]	index      Bar Number (from 1 to 10)
 *              width      Bar width
 *              y          y_reference for bar
 *              per        Percentage height
 *              dec        decrement bar YES/NO or TRUE/FALSE
 *              color      Bar Color
 * @return 		None
 **********************************************************************/
void GLCD_LBar(int16_t index, uint8_t width, int16_t y, int16_t per, Bool dec,uint16_t color)
{
	int16_t x1,x2,h,y_diff;

	if(dec)
	{
		y_diff=y+(per);
	}
	else
	{
		y_diff=y-(per);
	}

	if((y_diff>=20) && (index<=10) && !dec)
	{
		for(h=0;h<=per;h++)
		{
			x1 = 30+(index*26)-(width-(width/2));
			x2 = x1+width;
			GLCD_Line(x1,y-h,x2,y-h,color);
		}
	}
	else if(dec && (y_diff<=219) && (index<=10))
	{
		for(h=0;h<=per;h++)
		{
			x1 = 30+(index*26)-(width-(width/2));
			x2 = x1+width;
			GLCD_Line(x1,y+h,x2,y+h,color);
		}
	}
}


/*********************************************************************//**
 * @brief	    Display Bar Graph function
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Show_BarGraph(void)
{
	COORDINATE_Type point1,point2,point3;
	COLORCFG_Type tricfg;
    uint16_t y_scale,x_scale,i;

	// X and Y lines
	GLCD_Line(30,5,30,238,Black);
	GLCD_Line(1,220,315,220,Black);

	gprintf(22,5,1,Black,"Y");
	gprintf(310,225,1,Black,"X");
	gprintf(50,5,1,Black,"X=ADC-CH  Y=VAL  Y-MAX=4095  Y-UNIT=40.95");

	// Y Line Arrows Top and Bottom
	// Top arrow coordinates
	point1.x = 30;
	point1.y = 5;
	point2.x = 28;
	point2.y = 10;
	point3.x = 32;
	point3.y = 10;
	tricfg.fill = YES;
	tricfg.bndry= NO;
	tricfg.fill_color=Black;
	GLCD_Triangle(&point1,&point2,&point3,&tricfg);

	// Bottom arrow coordinates
	point1.x = 30;
	point1.y = 238;
	point2.x = 28;
	point2.y = 233;
	point3.x = 32;
	point3.y = 233;
	tricfg.fill = YES;
	tricfg.bndry= NO;
	tricfg.fill_color=Black;
	GLCD_Triangle(&point1,&point2,&point3,&tricfg);

	// X Line Arrows Left and Right
	// Left arrow coordinates
	point1.x = 1;
	point1.y = 220;
	point2.x = 6;
	point2.y = 218;
	point3.x = 6;
	point3.y = 222;
	tricfg.fill = YES;
	tricfg.bndry= NO;
	tricfg.fill_color=Black;
	GLCD_Triangle(&point1,&point2,&point3,&tricfg);

	// Right arrow coordinates
	point1.x = 315;
	point1.y = 220;
	point2.x = 310;
	point2.y = 218;
	point3.x = 310;
	point3.y = 222;
	tricfg.fill = YES;
	tricfg.bndry= NO;
	tricfg.fill_color=Black;
	GLCD_Triangle(&point1,&point2,&point3,&tricfg);

	// X and Y scaling
	gprintf(0,225,1,Black,"(0,0)");
	for(y_scale=200,i=1; y_scale>=20; y_scale=y_scale-20,i++)
	{
		if(y_scale!=20)
		{
			gprintf(14,y_scale,1,Black,"%d02",i*10);
		}
		else
		{
			gprintf(10,y_scale,1,Black,"%d03",i*10);
		}

	}
	for(x_scale=56,i=1; x_scale<=300; x_scale=x_scale+26,i++)
	{
		gprintf(x_scale,225,1,Black,"%d02",i);
	}
}


/*********************************************************************//**
 * @brief	    Display Bar Graph function
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Show_BarGraph_VI(void)
{
	COORDINATE_Type point1,point2,point3;
	COLORCFG_Type tricfg;
    uint16_t y_scale,x_scale,i;

	// X and Y lines
	GLCD_Line(30,5,30,238,Black);
	GLCD_Line(1,220,315,220,Black);

	gprintf(22,5,1,Black,"Y");
	gprintf(310,225,1,Black,"X");
	gprintf(50,5,1,Black,"X=T  Y=V");

	// Y Line Arrows Top and Bottom
	// Top arrow coordinates
	point1.x = 30;
	point1.y = 5;
	point2.x = 28;
	point2.y = 10;
	point3.x = 32;
	point3.y = 10;
	tricfg.fill = YES;
	tricfg.bndry= NO;
	tricfg.fill_color=Black;
	GLCD_Triangle(&point1,&point2,&point3,&tricfg);

	// Bottom arrow coordinates
	point1.x = 30;
	point1.y = 238;
	point2.x = 28;
	point2.y = 233;
	point3.x = 32;
	point3.y = 233;
	tricfg.fill = YES;
	tricfg.bndry= NO;
	tricfg.fill_color=Black;
	GLCD_Triangle(&point1,&point2,&point3,&tricfg);

	// X Line Arrows Left and Right
	// Left arrow coordinates
	point1.x = 1;
	point1.y = 220;
	point2.x = 6;
	point2.y = 218;
	point3.x = 6;
	point3.y = 222;
	tricfg.fill = YES;
	tricfg.bndry= NO;
	tricfg.fill_color=Black;
	GLCD_Triangle(&point1,&point2,&point3,&tricfg);

	// Right arrow coordinates
	point1.x = 315;
	point1.y = 220;
	point2.x = 310;
	point2.y = 218;
	point3.x = 310;
	point3.y = 222;
	tricfg.fill = YES;
	tricfg.bndry= NO;
	tricfg.fill_color=Black;
	GLCD_Triangle(&point1,&point2,&point3,&tricfg);

	// X and Y scaling
	gprintf(0,225,1,Black,"(0,0)");
	for(y_scale=200,i=1; y_scale>=20; y_scale=y_scale-20,i++)
	{
		if(y_scale!=20)
		{
			gprintf(14,y_scale,1,Black,"%d02",i*10);
		}
		else
		{
			gprintf(10,y_scale,1,Black,"%d03",i*10);
		}

	}
	for(x_scale=56,i=1; x_scale<=300; x_scale=x_scale+26,i++)
	{
		if(x_scale<290)
		{
			gprintf(x_scale,225,1,Black,"%d02",i*10);
		}
		else
		{
			gprintf(x_scale-5,225,1,Black,"%d03",i*10);
		}
	}
}


/*********************************************************************//**
 * @brief	    This function writes commands to the GLCD
 * @param[in]	Command		command to be written on GLCD
 * @return 		status
 **********************************************************************/
uchar Write_Command_Glcd (uint8_t Command)
{
	SSP_DATA_SETUP_Type xferConfig;
	uint8_t WriteStatus =0;
	__IO uint32_t i;

	GPIO_ClearValue(2, LCD_RS);  //select command mode

	CS_Force1 (LPC_SSP1, DISABLE);                        /* Select device           */
	xferConfig.tx_data = &Command;               /* Send Instruction Byte    */
	xferConfig.rx_data = NULL;
	xferConfig.length = 1;
	WriteStatus = SSP_ReadWrite(LPC_SSP1, &xferConfig, SSP_TRANSFER_POLLING);

	if(WriteStatus)
	{
		CS_Force1 (LPC_SSP1, ENABLE);                          /* CS high inactive        */
		for(i=925; i>0; i--);
		GPIO_SetValue(2, LCD_RS);  // select data mode
		return(1);
	}
	else
		return(0);
}


/*********************************************************************//**
 * @brief	    This function writes data to the GLCD
 * @param[in]	data	data to be written on GLCD
 * @return 		None
 **********************************************************************/
uchar Write_Data_Glcd (uint16_t data)
{
	SSP_DATA_SETUP_Type xferConfig;
	uint8_t WriteStatus =0;

	Tx_Buf1[0] = (uchar)(data>>8);    // 1st byte extract
	Tx_Buf1[1] = (uchar) data;        // 2nd byte extract

	GPIO_SetValue(2, LCD_RS);  // select data mode

	CS_Force1 (LPC_SSP1, DISABLE);                        /* Select device           */
	xferConfig.tx_data = Tx_Buf1;               /* Send Instruction Byte    */
	xferConfig.rx_data = NULL;
	xferConfig.length = 2;
	WriteStatus = SSP_ReadWrite(LPC_SSP1, &xferConfig, SSP_TRANSFER_POLLING);

	if(WriteStatus)
	{
		CS_Force1 (LPC_SSP1, ENABLE);                          /* CS high inactive        */
		return(1);
	}
	else
		return(0);
}


/**
 * @}
 */

/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

