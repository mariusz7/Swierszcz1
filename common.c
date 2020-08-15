/********************************************************************
* Description: common avr routines
* Created at: Mon Jul 18 00:14:18 CEST 2005
* System: Linux 2.6.10-helios-5 on i686
********************************************************************/

#include "common.h"

#ifndef FINAL_RELEASE
# include "led.h"
#endif //FINAL_RELEASE

#include "debug4avr.h"

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
/*---------------------------------------------------------------------------*/
#ifndef __GNUC__
#error avr-gcc jest wymagany
#endif

void waitus (uint16_t us) __attribute__ ((naked));
void waitus (uint16_t us)
{
#ifdef SHOW_WARNINGS
# warning FIXME zmienic XTAL i liczbe nop przy innym kwarcu
#endif //SHOW_WARNINGS
  REQUIRE (XTAL == 8000000);

  if (0 == us)
  {
    return;
  }
  else
  {
    __asm__ __volatile__ (
        "1: sbiw %0,1\n"  //2
        "   nop\n"        //1
        "   nop\n"        //1
        "   nop\n"        //1
        "   nop\n"        //1
        "   brne 1b\n"    //2
        :   "=w"  (us)
        :   "0"   (us));
  }
}
/*---------------------------------------------------------------------------*/
//void waitms (uint16_t ms) __attribute__ ((naked)); //dlaczego nie dziala ?
void waitms (uint16_t ms)
{
  while (ms--)
  {
    waitus (1000);
  }
}
/*---------------------------------------------------------------------------*/
//void waits (uint8_t s) __attribute__ ((naked)); //dlaczego nie dziala ?
void waits (uint8_t s)
{
  while (s--)
  {
    waitms (1000);
  }
}
/*---------------------------------------------------------------------------*/
void panic (void)
{
  cli ();

#ifndef FINAL_RELEASE
  if (! led_initialized ())
  {
    led_init ();
  }
#endif //FINAL_RELEASE

//  REQUIRE (led_initialized ()); //uwaga na rekurencje !!!

  #define REPEATS 50

  uint8_t i;

  while (true)
  {
    for (i = 0; i < REPEATS; i++)
    {
#ifndef FINAL_RELEASE
      led_switch ();
#endif //FINAL_RELEASE
      waitms (i);
    }

    for (i = REPEATS; i > 0; i--)
    {
#ifndef FINAL_RELEASE
      led_switch ();
#endif //FINAL_RELEASE
      waitms (i);
    }
#ifndef FINAL_RELEASE
    led_on ();
#endif //FINAL_RELEASE
    waitms (10);
  }
}
/*---------------------------------------------------------------------------*/
