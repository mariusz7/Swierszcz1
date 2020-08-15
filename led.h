/********************************************************************
* Description: led support for avr
* Created at: Mon Jul 18 00:14:18 CEST 2005
* System: Linux 2.6.10-helios-5 on i686
********************************************************************/

#ifndef LED_H
#define LED_H
/*---------------------------------------------------------------------------*/
#include <avr/io.h>
#include <stdbool.h>
/*---------------------------------------------------------------------------*/
#define LED_PORTx PORTx (B)
#define LED_DDRx DDRx (B)

#define LED_PORTxn PORTxn (B, 3)
#define LED_DDxn DDxn (B, 3)
/*---------------------------------------------------------------------------*/
bool led_initialized (void); //to comment ?
void led_init (void);
void led_on (void);
void led_off (void);
void led_switch (void);
/*---------------------------------------------------------------------------*/
#endif /*LED_H*/
