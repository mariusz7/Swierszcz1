/********************************************************************
* Description: Morse support for avr
* Created at: Mon Jul 18 00:14:18 CEST 2005
* System: Linux 2.6.10-helios-5 on i686
********************************************************************/

#include "morse.h"
#include "radio.h"

#ifndef FINAL_RELEASE
# include "lcd.h"
#endif //FINAL_RELEASE

#include "debug4avr.h"

#include <avr/pgmspace.h>
#include <string.h>
#include <stdbool.h>
//-----------------------------------------------------------------------------
const char P_1 [] __attribute__((progmem)) = ".----";
const char P_2 [] __attribute__((progmem)) = "..---";
const char P_3 [] __attribute__((progmem)) = "...--";
const char P_4 [] __attribute__((progmem)) = "....-";
const char P_5 [] __attribute__((progmem)) = ".....";
const char P_6 [] __attribute__((progmem)) = "-....";
const char P_7 [] __attribute__((progmem)) = "--...";
const char P_8 [] __attribute__((progmem)) = "---..";
const char P_9 [] __attribute__((progmem)) = "----.";
const char P_0 [] __attribute__((progmem)) = "-----";
//-----------------------------------------------------------------------------
const char P_a [] __attribute__((progmem)) = ".-";
const char P_b [] __attribute__((progmem)) = "-...";
const char P_c [] __attribute__((progmem)) = "-.-.";
const char P_d [] __attribute__((progmem)) = "-..";
const char P_e [] __attribute__((progmem)) = ".";

const char P_f [] __attribute__((progmem)) = "..-.";
const char P_g [] __attribute__((progmem)) = "--.";
const char P_h [] __attribute__((progmem)) = "....";
const char P_i [] __attribute__((progmem)) = "..";
const char P_j [] __attribute__((progmem)) = ".---";

const char P_k [] __attribute__((progmem)) = "-.-";
const char P_l [] __attribute__((progmem)) = ".-..";
const char P_m [] __attribute__((progmem)) = "--";


const char P_n [] __attribute__((progmem)) = "-.";
const char P_o [] __attribute__((progmem)) = "---";
const char P_p [] __attribute__((progmem)) = ".--.";
const char P_q [] __attribute__((progmem)) = "--.-";
const char P_r [] __attribute__((progmem)) = ".-.";

const char P_s [] __attribute__((progmem)) = "...";
const char P_t [] __attribute__((progmem)) = "-";
const char P_u [] __attribute__((progmem)) = "..-";
const char P_v [] __attribute__((progmem)) = "...-";
const char P_w [] __attribute__((progmem)) = ".--";

const char P_x [] __attribute__((progmem)) = "-..-";
const char P_y [] __attribute__((progmem)) = "-.--";
const char P_z [] __attribute__((progmem)) = "--..";
//-----------------------------------------------------------------------------
const char P_dot [] __attribute__((progmem)) = ".-.-.-";
const char P_comma [] __attribute__((progmem)) = "--..--";
//FIXME apostrofu ''' nie ma
const char P_underscore [] __attribute__((progmem)) = "..--.-";
const char P_colon [] __attribute__((progmem)) = "---...";
const char P_question_mark [] __attribute__((progmem)) = "..--..";
const char P_minus [] __attribute__((progmem)) = "-....-";
//'/'
//'('
//')'
const char P_sign_of_equality [] __attribute__((progmem)) = "-...-";
//'@'
//-----------------------------------------------------------------------------
static volatile bool morse_is_initialized; //=false
//-----------------------------------------------------------------------------
void morse_init (void)
{
  REQUIRE (! morse_is_initialized);

  //obecnie, ta funkcja wydaje sie niepotrzebna, ale jest obecna ze wzgledu
  //na spojnosc wykorzystania bibliotek (a moze sie przydac w przyslosci
  //i wtedy nie bedzie trzeba dodawac jej wywolania do roznych programow,
  //bo juz tam beda musialy byc)

  morse_is_initialized = true;
}
/*---------------------------------------------------------------------------*/
void morse_dot (void)
{
  REQUIRE (morse_is_initialized);

  tx_radio_on ();
  waitms (MORSE_DOT_MS);
  tx_radio_off ();
}
/*---------------------------------------------------------------------------*/
void morse_dash (void)
{
  REQUIRE (morse_is_initialized);

  tx_radio_on ();
  waitms (MORSE_DASH_MS);
  tx_radio_off ();
}
/*---------------------------------------------------------------------------*/
PGM_P morse_char2dotdash (char c)
{
  PGM_P rval;

  switch (c)
  {
    //cyfry
    case '1': rval = P_1; break;
    case '2': rval = P_2; break;
    case '3': rval = P_3; break;
    case '4': rval = P_4; break;
    case '5': rval = P_5; break;
    case '6': rval = P_6; break;
    case '7': rval = P_7; break;
    case '8': rval = P_8; break;
    case '9': rval = P_9; break;
    case '0': rval = P_0; break;

    //litery
    case 'A': case 'a': rval = P_a; break;
    case 'B': case 'b': rval = P_b; break;
    case 'C': case 'c': rval = P_c; break;
    case 'D': case 'd': rval = P_d; break;
    case 'E': case 'e': rval = P_e; break;

    case 'F': case 'f': rval = P_f; break;
    case 'G': case 'g': rval = P_g; break;
    case 'H': case 'h': rval = P_h; break;
    case 'I': case 'i': rval = P_i; break;
    case 'J': case 'j': rval = P_j; break;

    case 'K': case 'k': rval = P_k; break;
    case 'L': case 'l': rval = P_l; break;
    case 'M': case 'm': rval = P_m; break;


    case 'N': case 'n': rval = P_n; break;
    case 'O': case 'o': rval = P_o; break;
    case 'P': case 'p': rval = P_p; break;
    case 'Q': case 'q': rval = P_q; break;
    case 'R': case 'r': rval = P_r; break;

    case 'S': case 's': rval = P_s; break;
    case 'T': case 't': rval = P_t; break;
    case 'U': case 'u': rval = P_u; break;
    case 'V': case 'v': rval = P_v; break;
    case 'W': case 'w': rval = P_w; break;

    case 'X': case 'x': rval = P_x; break;
    case 'Y': case 'y': rval = P_y; break;
    case 'Z': case 'z': rval = P_z; break;

    //_wybrane_ znaki specjalne (w razie potrzeby mozna dodac pozostale)
    case '.': rval = P_dot; break;
    case ',': rval = P_comma; break;
    //FIXME apostrofu ''' nie ma
    case '_': case ' ': rval = P_underscore; break;
    case ':': rval = P_colon; break;
    case '?': rval = P_question_mark; break;
    case '-': rval = P_minus; break;
    //'/'
    //'('
    //')'
    case '=': rval = P_sign_of_equality; break;
    //'@'

    //w razie przekazania jakiegos nieobslugiwanego, domyslny znak to '?'
    default:  rval = P_question_mark; /* break; */
  }

  return rval;
}
//-----------------------------------------------------------------------------
void morse_emit_symbol (char symbol)
{
  if ('.' == symbol)
  {
    morse_dot ();
  }
  else if ('-' == symbol)
  {
    morse_dash ();
  }
  else
  {
    ; //FIXME nie powinno sie nigdy zdarzyc, ale moze dodac jednak jakas obsluge
  }
}
//-----------------------------------------------------------------------------
void morse_char (char c)
{
  REQUIRE (morse_is_initialized);

  PGM_P morse_code_P = morse_char2dotdash (c); //uzyskanie wskaznika w kodzie

  if ('\0' == pgm_read_byte (morse_code_P)) //nie powinno sie nigdy zdarzyc
  {
    return;
  }
  
  uint8_t i;
  uint8_t dotdash;

#ifndef FINAL_RELEASE
  lcd_yxstr (1, 0, "                ");
  lcd_yx (1, 0);
  lcd_char (c);
  lcd_char (':');
#endif //FINAL_RELEASE

  for (i = 0; i < (MORSE_LONGEST_CODE + 1); i++)
  {
    dotdash = pgm_read_byte (morse_code_P + i);

    ASSERT (('.' == dotdash) || ('-' == dotdash));

#ifndef FINAL_RELEASE
    lcd_char (dotdash);
#endif //FINAL_RELEASE

    morse_emit_symbol (dotdash);

    if ('\0' == pgm_read_byte (morse_code_P + i + 1))
    {
      break;
    }

    waitms (MORSE_DOTDASH_GAP_MS);
  }
}
/*---------------------------------------------------------------------------*/
void morse_str (char *str)
{
  REQUIRE (morse_is_initialized);

  if (NULL == str) //FIXME nie powinno sie nigdy zdarzyc,
  {                //ale moze jednak reagowac jakos na taki wskaznik ?
    return;
  }

  int16_t len = strlen (str);
  int16_t i;

  for (i = 0; i < (len - 1); i++)
  {
    morse_char (str [i]);
    waitms (MORSE_CHAR_GAP_MS);
  }
  morse_char (str [i]);
}
//-----------------------------------------------------------------------------
