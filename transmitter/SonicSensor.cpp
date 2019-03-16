#include "SonicSensor.hpp"
#include <Arduino.h>

bool SonicSensor::PulseIn(const uint8_t value, unsigned long int &result) const
{
    unsigned long int start;
    unsigned long int tmpTime;

    start = micros();
    while(digitalRead(m_EchoPin) != value)
    {
        if((tmpTime = (micros() - start)) >= m_Timeout)
        {
            result = tmpTime;
            return false;
        }
    }

    start = micros();
    while(digitalRead(m_EchoPin) == value)
    {
        if((tmpTime = (micros() - start)) >= m_Timeout)
        {
            result = tmpTime;
            return false;
        }
    }

    result = micros() - start;
    return true;
}

unsigned long int SonicSensor::GetTimeout(void) const { return m_Timeout; }
void SonicSensor::SetTimeout(const unsigned long int value) { m_Timeout = value; }

double SonicSensor::GetDistance(void) const
{
    unsigned long int duration;

    digitalWrite(m_TriggerPin, LOW);
    delayMicroseconds(2);

    digitalWrite(m_TriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(m_TriggerPin, LOW);

    if(!PulseIn(HIGH, duration))
    {
        return -1.0;
    }

    return (duration * (SPEED_OF_SOUND / 1000000.0)) / 2.0;
}

SonicSensor::SonicSensor(const uint8_t triggerPin, const uint8_t echoPin, const unsigned long int timeout)
{
    m_TriggerPin = triggerPin;
    m_EchoPin = echoPin;
    m_Timeout = timeout;

    pinMode(m_TriggerPin, OUTPUT);
    pinMode(m_EchoPin, INPUT);
}
