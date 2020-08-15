/********************************************************************
* Description: led support for avr
* Created at: Mon Jul 18 00:14:18 CEST 2005
* System: Linux 2.6.10-helios-5 on i686
********************************************************************/

#include "led.h"
#include "debug4avr.h"
#include <stdbool.h>
/*---------------------------------------------------------------------------*/
static volatile bool led_is_initialized; //=false
/*---------------------------------------------------------------------------*/
bool led_initialized (void)
{
  return led_is_initialized;
}
/*---------------------------------------------------------------------------*/
void led_init (void)
{
  REQUIRE (! led_is_initialized);

  sbi (LED_DDRx, LED_DDxn); // for output
  cbi (LED_PORTx, LED_PORTxn); //low state on init

  led_is_initialized = true;
}
/*---------------------------------------------------------------------------*/
void led_on (void)
{
  REQUIRE (led_is_initialized);

  sbi (LED_PORTx, LED_PORTxn);
}
/*---------------------------------------------------------------------------*/
void led_off (void)
{
  REQUIRE (led_is_initialized);

  cbi (LED_PORTx, LED_PORTxn);
}
/*---------------------------------------------------------------------------*/
void led_switch (void)
{
  REQUIRE (led_is_initialized);

  if (bit_is_clear (LED_PORTx, LED_PORTxn)) //a moze sprawdzac pin a nie port ?
  {
    led_on ();
  }
  else
  {
    led_off ();
  }
}
/*---------------------------------------------------------------------------*/
