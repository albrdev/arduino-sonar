#ifndef __SONICSENSOR_HPP__
#define __SONICSENSOR_HPP__

#include <stdint.h> /* uint8_t */

class SonicSensor
{
protected:
    static constexpr double SPEED_OF_SOUND = 343.0;

    uint8_t m_TriggerPin;
    uint8_t m_EchoPin;

    unsigned long int m_Timeout;

    bool PulseIn(const uint8_t value, unsigned long int &result) const;

public:
    unsigned long int GetTimeout(void) const;
    void SetTimeout(const unsigned long int value);

    double GetDistance(void) const;

    SonicSensor(const uint8_t triggerPin, const uint8_t echoPin, const unsigned long int timeout = 50000UL);
};

#endif // __SONICSENSOR_HPP__
