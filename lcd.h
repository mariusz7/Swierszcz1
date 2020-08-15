/********************************************************************
* Description: character lcd support for lcd
* Created at: Mon Jul 18 00:14:18 CEST 2005
* System: Linux 2.6.10-helios-5 on i686
********************************************************************/

#ifndef LCD_H
#define LCD_H
/*---------------------------------------------------------------------------*/
#include <inttypes.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "common.h"
/*---------------------------------------------------------------------------*/
#define LCD_DB7_PORTx   PORTx (A)
#define LCD_DB7_DDRx     DDRx (A)
#define LCD_DB7_PINx     PINx (A)

#define LCD_DB7_PORTxn PORTxn (A, 7)
#define LCD_DB7_DDxn     DDxn (A, 7)
#define LCD_DB7_PINxn   PINxn (A, 7)


#define LCD_DB6_PORTx   PORTx (A)
#define LCD_DB6_DDRx     DDRx (A)
#define LCD_DB6_PINx     PINx (A)

#define LCD_DB6_PORTxn PORTxn (A, 6)
#define LCD_DB6_DDxn     DDxn (A, 6)
#define LCD_DB6_PINxn   PINxn (A, 6)


#define LCD_DB5_PORTx   PORTx (A)
#define LCD_DB5_DDRx     DDRx (A)
#define LCD_DB5_PINx     PINx (A)

#define LCD_DB5_PORTxn PORTxn (A, 5)
#define LCD_DB5_DDxn     DDxn (A, 5)
#define LCD_DB5_PINxn   PINxn (A, 5)


#define LCD_DB4_PORTx   PORTx (A)
#define LCD_DB4_DDRx     DDRx (A)
#define LCD_DB4_PINx     PINx (A)

#define LCD_DB4_PORTxn PORTxn (A, 4)
#define LCD_DB4_DDxn     DDxn (A, 4)
#define LCD_DB4_PINxn   PINxn (A, 4)


#define LCD_ENABLE_PORTx   PORTx (A)
#define LCD_ENABLE_DDRx     DDRx (A)
#define LCD_ENABLE_PINx     PINx (A)

#define LCD_ENABLE_PORTxn PORTxn (A, 3)
#define LCD_ENABLE_DDxn     DDxn (A, 3)
#define LCD_ENABLE_PINxn   PINxn (A, 3)


#define LCD_RS_PORTx   PORTx (A)
#define LCD_RS_DDRx     DDRx (A)
#define LCD_RS_PINx     PINx (A)

#define LCD_RS_PORTxn PORTxn (A, 2)
#define LCD_RS_DDxn     DDxn (A, 2)
#define LCD_RS_PINxn   PINxn (A, 2)


#define LCD_RW_PORTx   PORTx (D)
#define LCD_RW_DDRx     DDRx (D)
#define LCD_RW_PINx     PINx (D)

#define LCD_RW_PORTxn PORTxn (D, 7)
#define LCD_RW_DDxn     DDxn (D, 7)
#define LCD_RW_PINxn   PINxn (D, 7)


#define LCD_LINE_1_START 0x00 //DDRAM address of first char of line 1
#define LCD_LINE_2_START 0x40 //DDRAM address of first char of line 2
#define LCD_LINE_WIDTH 16
#define LCD_LINES 2
/*---------------------------------------------------------------------------*/
void lcd_init (void);
bool lcd_initialized (void);
void lcd_on (void);
//void lcd_off (void);
//void lcd_home (void);
void lcd_clear (void);

/* line 0: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 */
/* line 1: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 */
void lcd_yx (uint8_t y, uint8_t x);
void lcd_yxstr (uint8_t y, uint8_t x, uint8_t* str);
void lcd_str (uint8_t* str);
//void lcd_str_P (PGM_P str);
void lcd_yxstr_P (uint8_t y, uint8_t x, PGM_P str);

void lcd_char (uint8_t data);
void lcd_uint8_t (uint8_t value);
void lcd_uint16_t (uint16_t value);
void lcd_waitbusy (void);
void lcd_uint32_t (uint32_t value);
/*---------------------------------------------------------------------------*/
#endif /*LCD_H*/
