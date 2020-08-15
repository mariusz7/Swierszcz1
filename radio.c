/********************************************************************
* Description: tx radio support for avr
* Created at: Mon Jul 18 00:14:18 CEST 2005
* System: Linux 2.6.10-helios-5 on i686
********************************************************************/

#include "radio.h"

#include "debug4avr.h"

#include <stdbool.h>
//-----------------------------------------------------------------------------
static volatile bool radio_is_initialized; //=false
/*---------------------------------------------------------------------------*/
void radio_init (void)
{
  REQUIRE (! radio_is_initialized);

  sbi (TX_RADIO_DDRx, TX_RADIO_DDxn); //jako wyjscie
  cbi (TX_RADIO_PORTx, TX_RADIO_PORTxn); //niski stan na poczatku

  radio_is_initialized = true;
}
/*---------------------------------------------------------------------------*/
void tx_radio_on (void)
{
  REQUIRE (radio_is_initialized);

  sbi (TX_RADIO_PORTx, TX_RADIO_PORTxn);
}
/*---------------------------------------------------------------------------*/
void tx_radio_off (void)
{
  REQUIRE (radio_is_initialized);

  cbi (TX_RADIO_PORTx, TX_RADIO_PORTxn);
}
/*---------------------------------------------------------------------------*/
