/********************************************************************
* Description: character lcd support for avr
* Created at: Mon Jul 18 00:14:18 CEST 2005
* System: Linux 2.6.10-helios-5 on i686
********************************************************************/

#include "lcd.h"
#include "common.h"
#include "debug4avr.h"

#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
/*---------------------------------------------------------------------------*/
#define MAKE_BIT_EQUAL_TO(x, xn, bit) \
  if (bit) \
  { \
    sbi (x, xn); \
  } \
  else \
    cbi (x, xn)

#define SELECT_INSTRUCTION_REGISTER() cbi (LCD_RS_PORTx, LCD_RS_PORTxn)
#define SELECT_DATA_REGISTER()        sbi (LCD_RS_PORTx, LCD_RS_PORTxn)
#define SET_LCD_WRITING_STATE()       cbi (LCD_RW_PORTx, LCD_RW_PORTxn)
#define SET_LCD_READING_STATE()       sbi (LCD_RW_PORTx, LCD_RW_PORTxn)
#define SET_E_LOW()                   cbi (LCD_ENABLE_PORTx, LCD_ENABLE_PORTxn)
#define SET_E_HIGH()                  sbi (LCD_ENABLE_PORTx, LCD_ENABLE_PORTxn)

#define LCD_STROBE() \
  waitus (10), \
  SET_E_HIGH (), \
  waitus (10), \
  SET_E_LOW (), \
  waitus (10)
/*---------------------------------------------------------------------------*/
static void lcd_write_4MSB (uint8_t byte);
static void lcd_write_4LSB (uint8_t byte);

static void lcd_DBX_as_output (void);
static void lcd_DBX_as_input (void);

static void lcd_write_instruction_register (uint8_t byte);
static void lcd_write_data_register (uint8_t byte);

static uint8_t lcd_read_instruction_register (void);
/*---------------------------------------------------------------------------*/
static volatile bool lcd_is_initialized; //=false
/*---------------------------------------------------------------------------*/
static void lcd_DBX_as_output (void)
{
  sbi (LCD_DB7_DDRx, LCD_DB7_DDxn); //configure pins for output
  sbi (LCD_DB6_DDRx, LCD_DB6_DDxn);
  sbi (LCD_DB5_DDRx, LCD_DB5_DDxn);
  sbi (LCD_DB4_DDRx, LCD_DB4_DDxn);
}
/*---------------------------------------------------------------------------*/
static void lcd_DBX_as_input (void)
{
  cbi (LCD_DB7_DDRx, LCD_DB7_DDxn); //configure pins for input
  cbi (LCD_DB6_DDRx, LCD_DB6_DDxn);
  cbi (LCD_DB5_DDRx, LCD_DB5_DDxn);
  cbi (LCD_DB4_DDRx, LCD_DB4_DDxn);
}
/*---------------------------------------------------------------------------*/
void lcd_init (void)
{
  REQUIRE (! lcd_is_initialized);

  waitms (500); //wait for power to be stable

  lcd_DBX_as_output ();
  sbi (LCD_RS_DDRx, LCD_RS_DDxn); //configure pins for output
  sbi (LCD_RW_DDRx, LCD_RW_DDxn);
  sbi (LCD_ENABLE_DDRx, LCD_ENABLE_DDxn);

  SELECT_INSTRUCTION_REGISTER ();

  //Initializing by instruction
  lcd_write_4MSB (_BV (5) | _BV (4)); //wartosci bitow z dokumentacji
  waitms (41); //correct

  lcd_write_4MSB (_BV (5) | _BV (4));
  waitms (10);

  lcd_write_4MSB (_BV (5) | _BV (4));
  waitms (10);

  //Set LCD to 4 bit mode
  lcd_write_4MSB (_BV (5));

  //Function set
  lcd_write_instruction_register (_BV (5) | _BV (3));

  //Display on/off control
  lcd_write_instruction_register (_BV (3));

  //Clear display
  lcd_write_instruction_register (_BV (0));

  //Entry mode set
  lcd_write_instruction_register (_BV (2) | _BV (1));

  lcd_is_initialized = true;
}
/*---------------------------------------------------------------------------*/
/*
bool lcd_initialized (void)
{
  return lcd_is_initialized;
}
*/
/*---------------------------------------------------------------------------*/
static uint8_t lcd_read_instruction_register (void)
{
  uint8_t byte_read = 0;

  SELECT_INSTRUCTION_REGISTER ();
  SET_LCD_READING_STATE ();

  lcd_DBX_as_input ();

  waitus (1); //t{AS} >= 40 ns

  //odczytaj gorna czesc bajtu

  SET_E_HIGH ();

  waitus (2); //t{DDR} <= 160 ns

  if (LCD_DB7_PINx & _BV (LCD_DB7_PINxn)) { byte_read |= _BV (7); }
  if (LCD_DB6_PINx & _BV (LCD_DB6_PINxn)) { byte_read |= _BV (6); }
  if (LCD_DB5_PINx & _BV (LCD_DB5_PINxn)) { byte_read |= _BV (5); }
  if (LCD_DB4_PINx & _BV (LCD_DB4_PINxn)) { byte_read |= _BV (4); }

  waitus (3); //PW{EH} >= 230 ns

  SET_E_LOW ();

  waitus (5); //t{cycE} >= 500 ns

  //odczytaj dolna czesc bajtu

  SET_E_HIGH (); // _/

  waitus (2); //t{DDR} <= 160 ns

  if (LCD_DB7_PINx & _BV (LCD_DB7_PINxn)) { byte_read |= _BV (3); }
  if (LCD_DB6_PINx & _BV (LCD_DB6_PINxn)) { byte_read |= _BV (2); }
  if (LCD_DB5_PINx & _BV (LCD_DB5_PINxn)) { byte_read |= _BV (1); }
  if (LCD_DB4_PINx & _BV (LCD_DB4_PINxn)) { byte_read |= _BV (0); }

  waitus (3); //PW{EH} >= 230 ns

  SET_E_LOW (); // \_

  waitus (5); //t{cycE} >= 500 ns

  return byte_read;
}
/*---------------------------------------------------------------------------*/
/*
static bool lcd_is_busy (void)
{
  return (lcd_read_instruction_register () & _BV (7));
}
*/
/*---------------------------------------------------------------------------*/
void lcd_waitbusy (void)
{
  while ((lcd_read_instruction_register () & _BV (7))) //uwaga na nieskonczona petle (watchdog)
  {
    ; //watchdog
  }
}
/*---------------------------------------------------------------------------*/
static void lcd_write_4MSB (uint8_t byte)
{
  SET_LCD_WRITING_STATE ();

  lcd_DBX_as_output ();

  MAKE_BIT_EQUAL_TO (LCD_DB7_PORTx, LCD_DB7_PORTxn, (byte & _BV (7)));
  MAKE_BIT_EQUAL_TO (LCD_DB6_PORTx, LCD_DB6_PORTxn, (byte & _BV (6)));
  MAKE_BIT_EQUAL_TO (LCD_DB5_PORTx, LCD_DB5_PORTxn, (byte & _BV (5)));
  MAKE_BIT_EQUAL_TO (LCD_DB4_PORTx, LCD_DB4_PORTxn, (byte & _BV (4)));

  LCD_STROBE ();
}
/*---------------------------------------------------------------------------*/
static void lcd_write_4LSB (uint8_t byte)
{
  SET_LCD_WRITING_STATE ();

  lcd_DBX_as_output ();

  MAKE_BIT_EQUAL_TO (LCD_DB7_PORTx, LCD_DB7_PORTxn, (byte & _BV (3)));
  MAKE_BIT_EQUAL_TO (LCD_DB6_PORTx, LCD_DB6_PORTxn, (byte & _BV (2)));
  MAKE_BIT_EQUAL_TO (LCD_DB5_PORTx, LCD_DB5_PORTxn, (byte & _BV (1)));
  MAKE_BIT_EQUAL_TO (LCD_DB4_PORTx, LCD_DB4_PORTxn, (byte & _BV (0)));

  LCD_STROBE ();
}
/*---------------------------------------------------------------------------*/
static void lcd_write_instruction_register (uint8_t byte)
{
  lcd_waitbusy ();

  SELECT_INSTRUCTION_REGISTER ();

  lcd_write_4MSB (byte);
  lcd_write_4LSB (byte);
}
/*---------------------------------------------------------------------------*/
static void lcd_write_data_register (uint8_t byte)
{
  lcd_waitbusy (); //delme ???????????????

  SELECT_DATA_REGISTER ();

  lcd_write_4MSB (byte);
  lcd_write_4LSB (byte);
}
/*---------------------------------------------------------------------------*/
void lcd_on (void)
{
  REQUIRE (lcd_is_initialized);

  lcd_write_instruction_register (_BV (3) | _BV (2));
//  waitms (10); //delme ?
}
/*---------------------------------------------------------------------------*/
/*
void lcd_off (void)
{
  REQUIRE (lcd_is_initialized);

  lcd_instruction_register_write (BV (3));
}
*/
/*---------------------------------------------------------------------------*/
void lcd_clear (void)
{
  REQUIRE (lcd_is_initialized);

  lcd_write_instruction_register (_BV (0));
}
/*---------------------------------------------------------------------------*/
/*
void lcd_home (void)
{
  REQUIRE (lcd_is_initialized);

  lcd_instruction_register_write (BV (1));
}
*/
/*---------------------------------------------------------------------------*/
/*
line 0: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
line 1: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
*/
void lcd_yx (uint8_t y, uint8_t x)
{
  REQUIRE (lcd_is_initialized);

  if (! (((0 == y) || (1 == y)) && (/*(0 <= x) || */(15 >= x))))
  {
    return; //correct ?
  }

  REQUIRE (((0 == y) || (1 == y)) && (/*(0 <= x) || */(15 >= x)));

  uint8_t DDRAM_address = _BV (7);

  if (0 == y)
  {
    DDRAM_address += LCD_LINE_1_START;
  }
  else if (1 == y)
  {
    DDRAM_address += LCD_LINE_2_START;
  }
  else
  {
    NOENTRY (); //correct ?
  }

  DDRAM_address += x;

  lcd_write_instruction_register (DDRAM_address);
}
/*---------------------------------------------------------------------------*/
void lcd_char (uint8_t data)
{
  REQUIRE (lcd_is_initialized);

  lcd_write_data_register (data);
}
/*---------------------------------------------------------------------------*/
void lcd_uint8_t (uint8_t value)
{
  REQUIRE (lcd_is_initialized);

  #define STRLEN_2POW8 3 //2^8 daje 3-y cyfrowa liczbe

  uint8_t result_str [STRLEN_2POW8];
  uint8_t i;

  for (i = 0; i < STRLEN_2POW8; i++)
  {
    result_str [i] = '0';
  }

  for (i = 0; (i < STRLEN_2POW8) && (0 < value); i++)
  {
    result_str [i] += value % 10;
    value /= 10;
  }

  i = STRLEN_2POW8;
  do
  {
    lcd_write_data_register (result_str [--i]);
  }
  while (i);
}
/*---------------------------------------------------------------------------*/
void lcd_uint16_t (uint16_t value)
{
  REQUIRE (lcd_is_initialized);

  #define STRLEN_2POW16 5 //2^16 daje 5-cio cyfrowa liczbe

  uint8_t result_str [STRLEN_2POW16];
  uint8_t i;

  for (i = 0; i < STRLEN_2POW16; i++)
  {
    result_str [i] = '0';
  }

  for (i = 0; (i < STRLEN_2POW16) && (0 < value); i++)
  {
    result_str [i] += (value % 10);
    value /= 10;
  }

  i = STRLEN_2POW16;
  do
  {
    lcd_write_data_register (result_str [--i]);
  }
  while (i);
}
/*---------------------------------------------------------------------------*/
void lcd_uint32_t (uint32_t value)
{
  REQUIRE (lcd_is_initialized);

  #define STRLEN_2POW32 10 //2^32 daje 10-cio cyfrowa liczbe

  uint8_t result_str [STRLEN_2POW32];
  uint8_t i;

  for (i = 0; i < STRLEN_2POW32; i++)
  {
    result_str [i] = '0';
  }

  for (i = 0; (i < STRLEN_2POW32) && (0 < value); i++)
  {
    result_str [i] += value % 10;
    value /= 10;
  }

  i = STRLEN_2POW32;
  do
  {
    lcd_write_data_register (result_str [--i]);
  }
  while (i);
}
//-----------------------------------------------------------------------------
void lcd_str (uint8_t *str)
{
  REQUIRE (lcd_is_initialized);
  REQUIRE (NULL != str);

  size_t str_len = strlen (str);

  uint8_t i;
  for (i = 0; i < str_len; i++)
  {
    lcd_write_data_register (str [i]);
  }
}
//-----------------------------------------------------------------------------
void lcd_yxstr (uint8_t y, uint8_t x, uint8_t *str)
{
  REQUIRE (lcd_is_initialized);
  REQUIRE (NULL != str);

  lcd_yx (y, x);

  size_t str_len = strlen (str);

  if (0 == str_len)
  {
    return;
  }

  bool str_to_long;

  if (LCD_LINE_WIDTH < x + str_len)
  {
    str_to_long = true;
    str_len = LCD_LINE_WIDTH - x - 1; //-1 na strzalke w prawo
  }
  else
  {
    str_to_long = false;
  }

  uint8_t i;
  for (i = 0; i < str_len; i++)
  {
    lcd_write_data_register (str [i]);
  }

  #define RIGHT_ARROW_CODE 126

  if (true == str_to_long)
  {
    lcd_write_data_register (RIGHT_ARROW_CODE);
  }
}
//-----------------------------------------------------------------------------
/*
void lcd_yxstr_P (uint8_t y, uint8_t x, PGM_P str)
{
  REQUIRE (lcd_is_initialized);

  uint8_t sram_string [LCD_LINE_WIDTH + 2]; //+1 na dodatkowy bajt (zeby uzyskac strzalke w prawo jezeli string jest za dlugi +1 na '\0'


#warning wczytujemy za duzo bajtow // jezeli string jest krotki !
  memcpy_P (sram_string, str, LCD_LINE_WIDTH + 2); //a co jezeli str zaczyna sie zaraz przed koncem FLASH'a

  sram_string [LCD_LINE_WIDTH + 1] = '\0';
  lcd_yxstr (y, x, sram_string);
}
*/
/*---------------------------------------------------------------------------*/
