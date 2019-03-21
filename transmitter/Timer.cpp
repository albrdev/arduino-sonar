#include "Timer.hpp"

unsigned long int Timer::GetInterval(void) const { return m_Interval; }
void Timer::SetInterval(const unsigned long int value) { m_Interval = value; }

void Timer::SetTriggerCallback(const timer_triggercallback_t value) { m_TriggerCallback = value; }

bool Timer::Active(void) const { return m_Active; }

void Timer::Start(const unsigned long int interval, const bool reset)
{
    SetInterval(interval);
    Start(reset);
}

void Timer::Start(const bool reset)
{
    m_Active = true;

    if(reset)
    {
        m_PreviousTime = m_TimeCallback();
    }
}

void Timer::Stop(void)
{
    m_Active = false;
}

bool Timer::Check(void)
{
    if(!m_Active) { return false; }

    unsigned long int currentTime = m_TimeCallback();
    bool result = (currentTime - m_PreviousTime) >= m_Interval;
    if(result)
    {
        m_PreviousTime = m_DelayCompensation ? m_PreviousTime + m_Interval : currentTime;
    }

    return result;
}

void Timer::Poll(void)
{
    if(Check())
    {
        if(m_TriggerCallback != nullptr) { m_TriggerCallback(); }
    }
}

Timer::operator bool(void) { return Check(); }
bool Timer::operator!(void) { return !Check(); }

Timer::Timer(const unsigned long int interval, const bool delayCompensation, const bool active, const timer_timecallback_t timeCallback)
{
    m_Interval = interval;
    m_DelayCompensation = delayCompensation;
    m_Active = active;
    m_TimeCallback = timeCallback;

    if(m_Active) { Start(); }
}
