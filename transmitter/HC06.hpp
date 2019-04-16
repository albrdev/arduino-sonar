#ifndef __HC06_HPP__
#define __HC06_HPP__

#include <SoftwareSerial.h> /* SoftwareSerial */

#define HC06_BUFFER_SIZE  (7U + 20U + 1U)

typedef enum
{
    HC06_BR_1200 = 1200,
    HC06_BR_2400 = 2400,
    HC06_BR_4800 = 4800,
    HC06_BR_9600 = 9600,
    HC06_BR_19200 = 19200,
    HC06_BR_38400 = 38400,
    HC06_BR_57600 = 57600,
    HC06_BR_115200 = 115200,
    HC06_BR_230400 = 230400,
    HC06_BR_460800 = 460800,
    HC06_BR_921600 = 921600,
    HC06_BR_1382400 = 1382400
} HC06BaudRate;

class HC06 : private SoftwareSerial
{
private:
    unsigned long int m_Timeout = 500UL;
    char m_CommandBuffer[HC06_BUFFER_SIZE];

    const char *SendCommand(const char *const command, const int responseLength);

public:
    bool Ping(void);
    const char *GetVersion(void);
    bool SetBaudRate(const HC06BaudRate value);
    bool SetName(const char *const value);
    bool SetPIN(const char *const value);

    void Begin(const HC06BaudRate baudRate);

    int AvailableBytes(void);
    size_t Write(const uint8_t *const buffer, const size_t length);
    size_t Write(const char *const buffer, const size_t length);
    size_t Write(const char *const str);
    size_t Write(const uint8_t byte);
    void Flush(void);

    operator bool(void);

    HC06(const uint8_t rxPin, const uint8_t txPin, const unsigned long int timeout = 1000UL, const bool inverseLogic = false);
};

#endif // __HC06_HPP__
