#include "LED.hpp"
#include <Arduino.h>    /* pinMode(), digitalWrite(), millis() */
#include "generic.h"

void LED::_SetState(const bool value)
{
    digitalWrite(m_Pin, value ? HIGH : LOW);
}

bool LED::GetState(void) const
{
    return DIGITALREAD(m_Pin);
}

void LED::SetState(const bool value)
{
    m_Active = false;
    _SetState(value);
}

void LED::On(void)
{
    m_Active = false;
    _SetState(true);
}

void LED::Off(void)
{
    m_Active = false;
    _SetState(false);
}

void LED::Toggle(void)
{
    m_Active = false;
    _SetState(!GetState());
}

void LED::Blink(unsigned long int interval)
{
    m_InitialState = GetState();
    m_Interval = interval;
    m_PreviousTime = millis();
    m_Active = true;
}

void LED::Stop(const bool reset)
{
    m_Active = false;

    if(reset)
    {
        _SetState(m_InitialState);
    }
}

void LED::Poll(void)
{
    unsigned long int currentTime = millis();
    if(currentTime - m_PreviousTime >= m_Interval)
    {
        Toggle();
        m_PreviousTime = currentTime;
    }
}

LED::LED(const uint8_t pin, const bool state) : LED(pin)
{
    SetState(state);
}

LED::LED(const uint8_t pin)
{
    m_Pin = pin;
    pinMode(m_Pin, OUTPUT);
}
