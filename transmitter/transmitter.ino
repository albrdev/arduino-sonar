#include <limits.h>
#include <assert.h>
#include <Arduino.h>
#include "SonicSensor.hpp"
#include "ExtendedStepper.hpp"
#include "math.h"
#include "packet.h"
#include "Timer.hpp"
#include "Button.hpp"

float getangle(const int steps, const int maxSteps)
{
    float angle = 360.0f * normalize01((float)steps, 0, maxSteps);
    return angle < 360.0f ? angle : 0.0f;
}

SonicSensor sonicSensor(2, 3, 50000UL);

ExtendedStepper stepMotor(64, 8, 9, 10, 11);
//const double ROTATION_MAX = 360.0;
const double ROTATION_MAX = 180.0;

Timer timer(50UL, true, false);

Button pauseButton(4);

void OnEndRotation(void)
{
    stepMotor.Rotate(ROTATION_MAX * -stepMotor.GetStepDirection());
}

bool isPaused = true;
void togglePause(void)
{
    isPaused = !isPaused;

    if(isPaused) timer.Stop();
    else timer.Start();

    delay(125UL);
}

void setup(void)
{
    // 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200
    Serial.begin(9600, SERIAL_8N1);

    stepMotor.setSpeed(120);
    stepMotor.SetOnEndRotationCallback(OnEndRotation);
    stepMotor.Rotate(ROTATION_MAX / 2.0);

    pauseButton.SetReleaseCallback(togglePause);
}

void loop(void)
{
    pauseButton.Poll();
    if(isPaused)
    {
        return;
    }

    if(timer)
    {
        float distance = (float)sonicSensor.GetDistance();
        if(distance >= 0.0)
        {
            float angle = (float)stepMotor.GetAngle();

            packet_sonardata_t packet;
            packet_mksonardata(&packet, distance, angle);
            Serial.write((char *)&packet, sizeof(packet_sonardata_t));
            Serial.flush();
        }
    }

    stepMotor.UpdateRotation();
}
