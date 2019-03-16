#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include <stdint.h> /* uint8_t */

class Button
{
private:
    uint8_t m_Pin;
    bool m_State;

    void(*m_PushCallback)(void) = nullptr;
    void(*m_ReleaseCallback)(void) = nullptr;
    void(*m_HoldCallback)(void) = nullptr;

public:
    bool GetState(void) const;

    void SetPushCallback(void(*value)(void));
    void SetReleaseCallback(void(*value)(void));
    void SetHoldCallback(void(*value)(void));

    void Poll(void);

    Button(uint8_t pin);
};

#endif // __BUTTON_HPP__
