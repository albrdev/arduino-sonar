#ifndef __HC06_HPP__
#define __HC06_HPP__

#include <Arduino.h>    /* millis() */
#include <SoftwareSerial.h> /* SoftwareSerial */

#define STRLEN(arr) ((sizeof(arr) / sizeof(*arr)) - 1U)

#define HC06_BUFFER_LENGTH  (7U + 20U + 1U)

#define HC06_NAME_MAXLENGTH 20U
#define HC06_PIN_LENGTH     4U

#define HC06_PING_COMMAND       "AT"
#define HC06_PING_RESPONSE      "OK"
#define HC06_VERSION_COMMAND    "AT+VERSION"
#define HC06_NAME_COMMAND       "AT+NAME"
#define HC06_NAME_RESPONSE      "OKsetname"
#define HC06_PIN_COMMAND        "AT+PIN"
#define HC06_PIN_RESPONSE       "OKsetPIN"

#define HC06_BAUD1200_COMMAND       "AT+BAUD1"
#define HC06_BAUD1200_RESPONSE      "OK1200"
#define HC06_BAUD2400_COMMAND       "AT+BAUD2"
#define HC06_BAUD2400_RESPONSE      "OK2400"
#define HC06_BAUD4800_COMMAND       "AT+BAUD3"
#define HC06_BAUD4800_RESPONSE      "OK4800"
#define HC06_BAUD9600_COMMAND       "AT+BAUD4"
#define HC06_BAUD9600_RESPONSE      "OK9600"
#define HC06_BAUD19200_COMMAND      "AT+BAUD5"
#define HC06_BAUD19200_RESPONSE     "OK19200"
#define HC06_BAUD38400_COMMAND      "AT+BAUD6"
#define HC06_BAUD38400_RESPONSE     "OK38400"
#define HC06_BAUD57600_COMMAND      "AT+BAUD7"
#define HC06_BAUD57600_RESPONSE     "OK57600"
#define HC06_BAUD115200_COMMAND     "AT+BAUD8"
#define HC06_BAUD115200_RESPONSE    "OK115200"
#define HC06_BAUD230400_COMMAND     "AT+BAUD9"
#define HC06_BAUD230400_RESPONSE    "OK230400"
#define HC06_BAUD460800_COMMAND     "AT+BAUDA"
#define HC06_BAUD460800_RESPONSE    "OK460800"
#define HC06_BAUD921600_COMMAND     "AT+BAUDB"
#define HC06_BAUD921600_RESPONSE    "OK921600"
#define HC06_BAUD1382400_COMMAND    "AT+BAUDC"
#define HC06_BAUD1382400_RESPONSE   "OK1382400"

/*
AT          OK              Used to verify communication
AT+VERSION  OKlinvorV1.8    The firmware version(version might depend on firmware)
AT+NAMExyz  OKsetname       Sets the module name to "xyz"
AT+PIN1234  OKsetPIN        Sets the module PIN to 1234
AT+BAUD1    OK1200          Sets the baud rate to 1200
AT+BAUD2    OK2400          Sets the baud rate to 2400
AT+BAUD3    OK4800          Sets the baud rate to 4800
AT+BAUD4    OK9600          Sets the baud rate to 9600
AT+BAUD5    OK19200         Sets the baud rate to 19200
AT+BAUD6    OK38400         Sets the baud rate to 38400
AT+BAUD7    OK57600         Sets the baud rate to 57600
AT+BAUD8    OK115200        Sets the baud rate to 115200
AT+BAUD9    OK230400        Sets the baud rate to 230400
AT+BAUDA    OK460800        Sets the baud rate to 460800
AT+BAUDB    OK921600        Sets the baud rate to 921600
AT+BAUDC    OK1382400       Sets the baud rate to 1382400
*/

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
    char m_CommandBuffer[7 + 20 + 1];

    const char *SendCommand(const char *const command, const int responseLength);

public:
    bool Ping(void);
    const char *GetVersion(void);
    bool SetBaudRate(const HC06BaudRate value);
    bool SetName(const char *const value);
    bool SetPIN(const char *const value);

    void Begin(const HC06BaudRate baudRate);

    size_t Write(const uint8_t *const buffer, const size_t length);
    size_t Write(const char *const buffer, const size_t length);
    size_t Write(const char *const str);
    size_t Write(const uint8_t byte);
    void Flush(void);

    operator bool(void);

    HC06(const uint8_t rxPin, const uint8_t txPin, const unsigned long int timeout = 1000UL, const bool inverseLogic = false);
};

#endif // __HC06_HPP__
