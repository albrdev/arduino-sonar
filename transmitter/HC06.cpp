#include "HC06.hpp"

const char *HC06::SendCommand(const char *const command, const int responseLength)
{
    unsigned long int start;
    int availableBytes;

    SoftwareSerial::print(command);
    SoftwareSerial::flush();

    start = millis();
    while((availableBytes = SoftwareSerial::available()) < responseLength)
    {
        if(millis() - start >= m_Timeout)
        {
            return nullptr;
        }
    }

    if(availableBytes > (int)HC06_BUFFER_LENGTH)
    {
        return nullptr;
    }

    size_t readLen = SoftwareSerial::readBytes(m_CommandBuffer, availableBytes);
    if(readLen <= 0U || readLen > (int)HC06_BUFFER_LENGTH)
    {
        return nullptr;
    }

    m_CommandBuffer[readLen] = '\0';
    return m_CommandBuffer;
}

bool HC06::Ping(void)
{
    const char *result = SendCommand(HC06_PING_COMMAND, STRLEN(HC06_PING_RESPONSE));
    return result != nullptr && strcmp(result, HC06_PING_RESPONSE) == 0;
}

const char *HC06::GetVersion(void)
{
    return SendCommand(HC06_VERSION_COMMAND, 12);
}

bool HC06::SetBaudRate(const HC06BaudRate value)
{
    const char *cmd;
    const char *rsp;

    switch(value)
    {
        case HC06BaudRate::HC06_BR_1200:
            cmd = HC06_BAUD1200_COMMAND;
            rsp = HC06_BAUD1200_RESPONSE;
            break;

        case HC06BaudRate::HC06_BR_2400:
            cmd = HC06_BAUD2400_COMMAND;
            rsp = HC06_BAUD2400_RESPONSE;
            break;

        case HC06BaudRate::HC06_BR_4800:
            cmd = HC06_BAUD4800_COMMAND;
            rsp = HC06_BAUD4800_RESPONSE;
            break;

        case HC06BaudRate::HC06_BR_9600:
            cmd = HC06_BAUD9600_COMMAND;
            rsp = HC06_BAUD9600_RESPONSE;
            break;

        case HC06BaudRate::HC06_BR_19200:
            cmd = HC06_BAUD19200_COMMAND;
            rsp = HC06_BAUD19200_RESPONSE;
            break;

        case HC06BaudRate::HC06_BR_38400:
            cmd = HC06_BAUD38400_COMMAND;
            rsp = HC06_BAUD38400_RESPONSE;
            break;

        case HC06BaudRate::HC06_BR_57600:
            cmd = HC06_BAUD57600_COMMAND;
            rsp = HC06_BAUD57600_RESPONSE;
            break;

        case HC06BaudRate::HC06_BR_115200:
            cmd = HC06_BAUD115200_COMMAND;
            rsp = HC06_BAUD115200_RESPONSE;
            break;

        case HC06BaudRate::HC06_BR_230400:
            cmd = HC06_BAUD230400_COMMAND;
            rsp = HC06_BAUD230400_RESPONSE;
            break;

        case HC06BaudRate::HC06_BR_460800:
            cmd = HC06_BAUD460800_COMMAND;
            rsp = HC06_BAUD460800_RESPONSE;
            break;

        case HC06BaudRate::HC06_BR_921600:
            cmd = HC06_BAUD921600_COMMAND;
            rsp = HC06_BAUD921600_RESPONSE;
            break;

        case HC06BaudRate::HC06_BR_1382400:
            cmd = HC06_BAUD1382400_COMMAND;
            rsp = HC06_BAUD1382400_RESPONSE;
            break;

        default:
            return false;
    }

    const char *result = SendCommand(cmd, strlen(rsp));
    return result != nullptr && strcmp(result, rsp) == 0;
}

bool HC06::SetName(const char *const value)
{
    size_t len = strlen(value);
    if(len > HC06_NAME_MAXLENGTH)
    {
        return false;
    }

    strcpy(m_CommandBuffer, HC06_NAME_COMMAND);
    strncpy(m_CommandBuffer + STRLEN(HC06_NAME_COMMAND), value, HC06_NAME_MAXLENGTH);
    m_CommandBuffer[STRLEN(HC06_NAME_COMMAND) + len] = '\0';

    const char *result = SendCommand(m_CommandBuffer, STRLEN(HC06_NAME_RESPONSE));
    return result != nullptr && strcmp(result, HC06_NAME_RESPONSE) == 0;
}

bool HC06::SetPIN(const char *const value)
{
    size_t len = strlen(value);
    if(len != HC06_PIN_LENGTH)
    {
        return false;
    }

    strcpy(m_CommandBuffer, HC06_PIN_COMMAND);
    strncpy(m_CommandBuffer + STRLEN(HC06_PIN_COMMAND), value, HC06_PIN_LENGTH);
    m_CommandBuffer[STRLEN(HC06_PIN_COMMAND) + len] = '\0';

    const char *result = SendCommand(m_CommandBuffer, STRLEN(HC06_PIN_RESPONSE));
    return result != nullptr && strcmp(result, HC06_PIN_RESPONSE) == 0;
}

void HC06::Begin(const HC06BaudRate baudRate)
{
    SoftwareSerial::begin((long int)baudRate);
}

size_t HC06::Write(const uint8_t *const buffer, const size_t length)
{
    return SoftwareSerial::write(buffer, length);
}

size_t HC06::Write(const char *const buffer, const size_t length)
{
    return SoftwareSerial::write(buffer, length);
}

size_t HC06::Write(const char *const str)
{
    return SoftwareSerial::write(str);
}

size_t HC06::Write(const uint8_t byte)
{
    return SoftwareSerial::write(byte);
}

void HC06::Flush(void)
{
    SoftwareSerial::flush();
}

HC06::operator bool(void) { return SoftwareSerial::operator bool(); }

HC06::HC06(const uint8_t rxPin, const uint8_t txPin, const unsigned long int timeout, const bool inverseLogic) : SoftwareSerial(rxPin, txPin, inverseLogic)
{
    m_Timeout = timeout;
}
