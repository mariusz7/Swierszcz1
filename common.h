/********************************************************************
* Description: common avr routines and constants definition
* Created at: Mon Jul 18 00:14:18 CEST 2005
* System: Linux 2.6.10-helios-5 on i686
********************************************************************/

#ifndef COMMON_H
#define COMMON_H
/*---------------------------------------------------------------------------*/
#include <inttypes.h>
/*---------------------------------------------------------------------------*/
#define PORTx(LETTER) (PORT ## LETTER)
#define DDRx(LETTER) (DDR ## LETTER)
#define PINx(LETTER) (PIN ## LETTER)

#define PORTxn(LETTER, NUMBER) (PORT ## LETTER ## NUMBER)
#define DDxn(LETTER, NUMBER) (DD ## LETTER ## NUMBER)
#define PINxn(LETTER, NUMBER) (PIN ## LETTER ## NUMBER)


//ponizsze makra zostaly usuniete z biblioteki standardowej, ale sa pomocne,
//wiec wkleilem ich definicje tu ze starszej wersji biblioteki

/** \def cbi
    \ingroup avr_sfr
    \deprecated
    \code #include <avr/io.h>\endcode
    For backwards compatibility only. This macro will eventually be removed.

    Clear bit \c bit in IO register \c sfr. */

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

/** \def sbi
    \ingroup avr_sfr
    \deprecated
    \code #include <avr/io.h>\endcode
    For backwards compatibility only. This macro will eventually be removed.

    Set bit \c bit in IO register \c sfr. */

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


//#define NOP() __asm__ __volatile__ ("nop\n")

#ifdef SHOW_WARNINGS
# warning FIXME konfiguracja dla kwarcu 8 MHz
#endif //SHOW_WARNINGS

#define XTAL ((uint32_t)8000000)

//#define MIN(a, b) (((a) < (b)) ? (a) : (b))
/*---------------------------------------------------------------------------*/
void waitus (uint16_t us);
void waitms (uint16_t ms);
void waits (uint8_t s);
void panic (void);
/*---------------------------------------------------------------------------*/
#endif /*COMMON_H*/
