#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <Arduino.h>    /* millis() */

typedef unsigned long int(*timer_timecallback_t)(void);
typedef void(*timer_triggercallback_t)(void);

class Timer
{
private:
    unsigned long int m_Interval;
    bool m_DelayCompensation;
    bool m_Active;
    timer_timecallback_t m_TimeCallback = nullptr;
    timer_triggercallback_t m_TriggerCallback = nullptr;

    unsigned long int m_PreviousTime = 0UL;

public:
    unsigned long int GetInterval(void) const;
    void SetInterval(const unsigned long int value);

    void SetTriggerCallback(const timer_triggercallback_t value);

    bool Active(void) const;
    void Start(const unsigned long int interval, const bool reset = true);
    void Start(const bool reset = true);
    void Stop(void);

    bool Check(void);
    void Poll(void);

    operator bool(void);
    bool operator!(void);

    Timer(const unsigned long int interval, const bool delayCompensation = false, const bool active = true, const timer_timecallback_t timeCallback = millis);
};

#endif // __TIMER_HPP__
