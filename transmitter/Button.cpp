#include "Button.hpp"
#include <Arduino.h>    /* pinMode(), digitalRead() */

bool Button::GetState(void) const
{
    return digitalRead(m_Pin);
}

void Button::SetPushCallback(const button_triggercallback_t value)
{
    m_PushCallback = value;
}

void Button::SetReleaseCallback(const button_triggercallback_t value)
{
    m_ReleaseCallback = value;
}

void Button::SetHoldCallback(const button_triggercallback_t value)
{
    m_HoldCallback = value;
}

void Button::Poll(void)
{
    bool state = digitalRead(m_Pin);
    if(state != m_PreviousState)
    {
        m_PreviousState = state;

        if(state)
        {
            if(m_PushCallback != nullptr) { m_PushCallback(); }
        }
        else
        {
            if(m_ReleaseCallback != nullptr) { m_ReleaseCallback(); }
        }
    }
    else
    {
        if(state)
        {
            if(m_HoldCallback != nullptr) { m_HoldCallback(); }
        }
    }
}

Button::Button(const uint8_t pin)
{
    m_Pin = pin;

    pinMode(m_Pin, INPUT);
    m_PreviousState = digitalRead(m_Pin);
}
