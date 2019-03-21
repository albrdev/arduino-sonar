#ifndef __GENERIC_H__
#define __GENERIC_H__

#ifdef __AVR__
#include <Arduino.h>    /* digitalPinToPort(), digitalPinToBitMask(), portOutputRegister() */
#endif

#ifdef __AVR__
#define DIGITALREAD(pin) ((*portOutputRegister(digitalPinToPort(pin)) & digitalPinToBitMask(pin)) != 0 ? HIGH : LOW) // *portOutputRegister(digitalPinToPort(pin)) & digitalPinToBitMask(pin) != 0 ? HIGH : LOW
#endif

#endif // __GENERIC_H__
