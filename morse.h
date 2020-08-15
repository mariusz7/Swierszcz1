/********************************************************************
* Description: Morse support for avr
* Created at: Mon Jul 18 00:14:18 CEST 2005
* System: Linux 2.6.10-helios-5 on i686
********************************************************************/

#ifndef MORSE_H
#define MORSE_H
//-----------------------------------------------------------------------------
#ifdef SHOW_WARNINGS
# warning FIXME ustawic zadana czasy dla alfabetu morse'a
#endif //SHOW_WARNINGS

//wszystkie wartosci w ms

//czas trwania kropki
#define MORSE_DOT_MS 100

//czas trwania kreski
#define MORSE_DASH_MS (3 * MORSE_DOT_MS)

//czas miedzy kropkami/kreskami
#define MORSE_DOTDASH_GAP_MS MORSE_DOT_MS

//czas miedzy znakami
#define MORSE_CHAR_GAP_MS MORSE_DASH_MS

//czas miedzy grupami
#define MORSE_GROUP_GAP_MS (3 * MORSE_DASH_MS)
//-----------------------------------------------------------------------------
//wykorzystywany znak o najwiekszej liczbie kropek i kresek
#define MORSE_LONGEST_CODE 12
//-----------------------------------------------------------------------------
void morse_init (void);
void morse_str (char *str);
//-----------------------------------------------------------------------------
#endif //MORSE_H
