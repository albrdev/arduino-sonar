#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include <stdint.h> /* uint8_t */

typedef void(*button_triggercallback_t)(void);

class Button
{
private:
    uint8_t m_Pin;
    bool m_PreviousState;

    button_triggercallback_t m_PushCallback = nullptr;
    button_triggercallback_t m_ReleaseCallback = nullptr;
    button_triggercallback_t m_HoldCallback = nullptr;

public:
    bool GetState(void) const;

    void SetPushCallback(const button_triggercallback_t value);
    void SetReleaseCallback(const button_triggercallback_t value);
    void SetHoldCallback(const button_triggercallback_t value);

    void Poll(void);

    Button(const uint8_t pin);
};

#endif // __BUTTON_HPP__
