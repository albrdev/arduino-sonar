#ifndef __EXTENDEDSTEPPER_HPP__
#define __EXTENDEDSTEPPER_HPP__

#include <stdint.h>     /* uint8_t */
#include <Arduino.h>
#include <Stepper.h>

typedef void(*endrotationcallback_t)(void);

class ExtendedStepper : public Stepper
{
private:
    const int STEPS_PER_REVOLUTION = 4096 / 2;
    const double STEPS_PER_DEGREE = STEPS_PER_REVOLUTION / 360.0;

    int m_StepsMax = 0;
    int m_Steps = 0;
    int8_t m_StepDirection = 0;

    endrotationcallback_t m_OnEndRotationCallback;

public:
    double GetAngle(void) const;
    int8_t GetStepDirection(void) const;

    void SetOnEndRotationCallback(const endrotationcallback_t value);

    void Rotate(const double degrees);
    void UpdateRotation(void);

    ExtendedStepper(const int stepsPerRevolution, uint8_t motorPin1, uint8_t motorPin2);
    ExtendedStepper(const int stepsPerRevolution, uint8_t motorPin1, uint8_t motorPin2, uint8_t motorPin3, uint8_t motorPin4);
    ExtendedStepper(const int stepsPerRevolution, uint8_t motorPin1, uint8_t motorPin2, uint8_t motorPin3, uint8_t motorPin4, uint8_t motorPin5);
};

#endif // __EXTENDEDSTEPPER_HPP__
