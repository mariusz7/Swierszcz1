/********************************************************************
* Description: main program file
* Created at: Mon Jul 18 00:14:18 CEST 2005
* System: Linux 2.6.10-helios-5 on i686
********************************************************************/

#include "radio.h"
#include "morse.h"

#ifndef FINAL_RELEASE
# include "led.h"
# include "lcd.h"
#endif //FINAL_RELEASE

#include "dallas.h"		//include dallas 1-wire support
#include "ds18b20.h"	//include dallas temp sensor support

#include "debug4avr.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//-----------------------------------------------------------------------------
#ifdef SHOW_WARNINGS
# warning FIXME ustawic zadana odstep miedzy seria pomiarow temp. na wszystkich czujnikach (0 - 65535 s)
#endif //SHOW_WARNINGS

#define TEMP_MEASUREMENT_GAP_S 1

//ilosc bitow na przechowanie wyniku pomiaru (9 - 12)
#define TEMP_ACCURACY 9
//-----------------------------------------------------------------------------
int main (void)
{
  radio_init ();
  morse_init ();

#ifndef FINAL_RELEASE
  led_init ();
  lcd_init ();
  lcd_on ();
#endif //FINAL_RELEASE

  dallas_rom_id_T rom_ids [DALLAS_MAX_DEVICES];
  int16_t temperatures [DALLAS_MAX_DEVICES];

  uint8_t i;
  uint8_t rval;

  char intstr [1 + 5 + 1]; //1 na ew. '-'; +5 == strlen ("32768"); +1 na '\0'

/*
    lcd_yxstr (0, 0, "morse test");
    morse_str ("1234567890");
    morse_str ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    morse_str ("abcdefghijklmnopqrstuvwxyz");
    morse_str ("_ ?=");
*/

#ifndef FINAL_RELEASE
  lcd_clear ();
#endif //FINAL_RELEASE

  uint8_t devsFound = dallasFindDevices (rom_ids);

#ifndef FINAL_RELEASE
  lcd_yxstr (0, 0, "devsFound: ");
  lcd_uint8_t (devsFound);
  waits (1);
  lcd_clear ();
#endif //FINAL_RELEASE

  for (i = 0; i < devsFound; i++)
  {
    rval = ds18b20Setup (& rom_ids [i], TEMP_ACCURACY,
        DS18B20_NO_ALARM_LOW, DS18B20_NO_ALARM_HIGH);

#ifndef FINAL_RELEASE
    if (DALLAS_NO_ERROR != rval)
    {
      lcd_yxstr (0, 0, "rval:");
      lcd_char (rval);

      lcd_yxstr (1, 0, "ds18b20SetupFAIL");

      waits (1);
      lcd_clear ();
    }
#endif //FINAL_RELEASE
  }

  //przykladowy wynik dla 2 czujnikow: "SKIK:T0=-13,T1=31"
  while (true)
  {
    for (i = 0; i < devsFound; i++)
    {
      ds18b20StartAndResult (& rom_ids [i], & temperatures [i]);
      temperatures [i] >>= 4; //obciecie 4 bitow czesci ulamkowej
    }

#ifndef FINAL_RELEASE
    lcd_clear ();
    lcd_yxstr (0, 0, "S:"); //tylko 'S' zeby mozna bylo zmiescic 2 temperatury
#endif //FINAL_RELEASE

    morse_str ("SKIK:");
    waitms (MORSE_CHAR_GAP_MS);

    for (i = 0; i < devsFound; i++)
    {
#ifndef FINAL_RELEASE
      if (0 == i)
      {
        lcd_yxstr (0, 2, "           ");
        lcd_yx (0, 2);
      }
      else if (1 == i)
      {
        lcd_yx (0, 9);
      }
      lcd_str ("T");
      lcd_str (itoa (i, intstr, 10)); //numer czujnika (liczac od 0)
      lcd_char ('=');

      lcd_str (itoa (temperatures [i], intstr, 10));
      if (devsFound - 1 > i)
      {
        lcd_char (',');
      }
#endif //FINAL_RELEASE


      morse_str ("T");
      waitms (MORSE_CHAR_GAP_MS);

      morse_str (itoa (i, intstr, 10)); //numer czujnika (liczac od 0)
      waitms (MORSE_CHAR_GAP_MS);

      morse_str ("=");
      waitms (MORSE_CHAR_GAP_MS);

      morse_str (itoa (temperatures [i], intstr, 10));

      if (devsFound - 1 > i)
      {
        waitms (MORSE_CHAR_GAP_MS);
        morse_str (",");
        waitms (MORSE_CHAR_GAP_MS);
      }
    }

    waitms (MORSE_GROUP_GAP_MS); //minimalny odstep miedzy pomiarami

    waits (TEMP_MEASUREMENT_GAP_S); //dodatkowe oczekiwanie

#ifndef FINAL_RELEASE
    led_switch ();
#endif //FINAL_RELEASE
  }
}
/*---------------------------------------------------------------------------*/
