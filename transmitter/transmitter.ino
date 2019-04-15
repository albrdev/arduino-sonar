#include <limits.h>
#include <assert.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SonicSensor.hpp"
#include "ExtendedStepper.hpp"
#include "Timer.hpp"
#include "LED.hpp"
#include "InterruptIn.hpp"
#include "HC06.hpp"
#include "math.h"
#include "custom_packets.h"

const double SONICSENSOR_MINRANGE = 0.02;
const double SONICSENSOR_MAXRANGE = 4.0;
const double SONICSENSOR_RESOLUTION = 0.003;
const double SONICSENSOR_RANGEFACTOR = 1000000.0;
const unsigned long int SONICSENSOR_TIMEOUT = 50000UL;
SonicSensor sonicSensor(2, 3, SONICSENSOR_RANGEFACTOR, SONICSENSOR_TIMEOUT);

ExtendedStepper stepMotor(64, 8, 9, 10, 11);
//const double ROTATION_MAX = 360.0;
const double ROTATION_MAX = 180.0;

HC06 bluetooth(4, 5);
Timer timer(50UL, true, false);
LED led(6, false);
InterruptIn pauseButton(7);

void restartRotation(void)
{
    Serial.println("Max. rotation reached. Reversing...");
    Serial.flush();

    stepMotor.Rotate(ROTATION_MAX * -stepMotor.GetStepDirection());
}

double lastDistance = 0.0;
void transmitData(void)
{
    float distance = (float)sonicSensor.GetDistance();
    if(distance < SONICSENSOR_MINRANGE || distance > SONICSENSOR_MAXRANGE) { return; }

    if(absdiff(distance, lastDistance) < SONICSENSOR_RESOLUTION)
    {
        distance = lastDistance;
    }
    else
    {
        lastDistance = distance;
    }

    float angle = (float)stepMotor.GetAngle();

    packet_sonardata_t packet;
    packet_mksonardata(&packet, distance, angle);
    bluetooth.Write((char *)&packet, sizeof(packet_sonardata_t));
    bluetooth.Flush();
}

bool isPaused = true;
void togglePause(const bool state)
{
    if(state) { return; }

    isPaused = !isPaused;
    if(isPaused)
    {
        Serial.println("Paused");
        timer.Stop();
    }
    else
    {
        Serial.println("Unpaused");
        timer.Start();
    }

    Serial.flush();
    delay(125UL);
}

void setupBluetooth(void)
{
    Serial.println("Initializing Bluetooth device");
    bluetooth.Begin(HC06_BR_9600);

    Serial.println("Verifying device");
    while(!bluetooth.Ping())
    {
        Serial.println("Failed");
        led.On();
        delay(500UL);
    }

    delay(500UL);
    Serial.print("Version: ");
    const char *version = bluetooth.GetVersion();
    if(version != nullptr)
    {
        Serial.println(version);
    }
    else
    {
        Serial.println("N/A");
        led.On();
    }

    delay(500UL);
    Serial.println("Setting baud rate");
    while(!bluetooth.SetBaudRate(HC06BaudRate::HC06_BR_9600))
    {
        Serial.println("Failed");
        led.On();
        delay(500UL);
    }

    delay(500UL);
    Serial.println("Setting PIN");
    while(!bluetooth.SetPIN("1357"))
    {
        Serial.println("Failed");
        led.On();
        delay(500UL);
    }

    delay(500UL);
    Serial.println("Setting name");
    while(!bluetooth.SetName("ArduinoSonar"))
    {
        Serial.println("Failed");
        led.On();
        delay(500UL);
    }

    Serial.println("");
    led.Off();
}

void setup(void)
{
    // 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200
    Serial.begin(9600, SERIAL_8N1);

    setupBluetooth();

    stepMotor.setSpeed(120);
    stepMotor.SetOnEndRotationCallback(restartRotation);
    stepMotor.Rotate(ROTATION_MAX / 2.0);

    timer.SetTriggerCallback(transmitData);
    pauseButton.SetOnStateChangedCallback(togglePause);

    Serial.println("Press pause button to start");
    Serial.println("");
    Serial.flush();
}

void loop(void)
{
    pauseButton.Poll();
    if(isPaused)
    {
        return;
    }

    timer.Poll();
    stepMotor.UpdateRotation();
}
