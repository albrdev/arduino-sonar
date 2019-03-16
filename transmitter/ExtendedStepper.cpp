#include "ExtendedStepper.hpp"
#include "math.h"

double ExtendedStepper::GetAngle(void) const
{
    int steps = m_Steps;
    if(steps >= m_StepsMax)
    {
        steps = steps - ((int)(steps / 360.0) * 360.0);
    }

    double angle = (m_StepDirection >= 0 && steps ? steps : m_StepsMax - steps) / STEPS_PER_DEGREE;
    return angle;
}

int8_t ExtendedStepper::GetStepDirection(void) const
{
    return m_StepDirection;
}

void ExtendedStepper::SetOnEndRotationCallback(const endrotationcallback_t value)
{
    m_OnEndRotationCallback = value;
}

void ExtendedStepper::Rotate(const double degrees)
{
    m_StepDirection = sgn(degrees);
    m_StepsMax = abs(degrees) * STEPS_PER_DEGREE;
    m_Steps = 0;
}

void ExtendedStepper::UpdateRotation(void)
{
    if(m_Steps >= m_StepsMax)
    {
        return;
    }

    step(m_StepDirection);
    m_Steps++;

    if(m_Steps >= m_StepsMax)
    {
        if(m_OnEndRotationCallback != nullptr)
        {
            (*m_OnEndRotationCallback)();
        }
    }
}

ExtendedStepper::ExtendedStepper(const int stepsPerRevolution, uint8_t motorPin1, uint8_t motorPin2) : Stepper::Stepper(stepsPerRevolution, motorPin1, motorPin2) { }
ExtendedStepper::ExtendedStepper(const int stepsPerRevolution, uint8_t motorPin1, uint8_t motorPin2, uint8_t motorPin3, uint8_t motorPin4) : Stepper::Stepper(stepsPerRevolution, motorPin1, motorPin2, motorPin3, motorPin4) { }
ExtendedStepper::ExtendedStepper(const int stepsPerRevolution, uint8_t motorPin1, uint8_t motorPin2, uint8_t motorPin3, uint8_t motorPin4, uint8_t motorPin5) : Stepper::Stepper(stepsPerRevolution, motorPin1, motorPin2, motorPin3, motorPin4, motorPin5) { }
