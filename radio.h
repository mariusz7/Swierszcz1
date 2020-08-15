/********************************************************************
* Description: tx radio support for avr
* Created at: Mon Jul 18 00:14:18 CEST 2005
* System: Linux 2.6.10-helios-5 on i686
********************************************************************/

#ifndef RADIO_H
#define RADIO_H
//-----------------------------------------------------------------------------
#include <avr/io.h>
//-----------------------------------------------------------------------------
#ifdef SHOW_WARNINGS
# warning FIXME zmienic 'A' i '0' na odpowiedni port/pin po zmianie atmela/nozki
#endif //SHOW_WARNINGS

#define TX_RADIO_PORTx PORTx (A)
#define TX_RADIO_DDRx DDRx (A)
#define TX_RADIO_PINx PINx (A)

#define TX_RADIO_PORTxn PORTxn (A, 0)
#define TX_RADIO_DDxn DDxn (A, 0)
#define TX_RADIO_PINxn PINxn (A, 0)
//-----------------------------------------------------------------------------
void radio_init (void);
void tx_radio_on (void);
void tx_radio_off (void);
//-----------------------------------------------------------------------------
#endif //RADIO_H
