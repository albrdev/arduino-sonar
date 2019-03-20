#ifndef _LED_HPP_
#define _LED_HPP_

#include <stdint.h> /* uint8_t */

class LED
{
private:
    uint8_t m_Pin;
    bool m_InitialState;
    bool m_Active = false;

    unsigned long int m_Interval;
    unsigned long int m_PreviousTime;

    void _SetState(const bool value);

public:
    bool GetState(void) const;
    void SetState(const bool value);

    void On(void);
    void Off(void);
    void Toggle(void);

    void Blink(const unsigned long int interval);

    void Stop(const bool reset = true);
    void Poll(void);

    LED(const uint8_t pin, const bool state);
    LED(const uint8_t pin);
};

#endif // _LED_HPP_
