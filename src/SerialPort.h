#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <vector>

#include "serialib.h"
#include "imgui.h"

[[nodiscard]] int do_something()
{
    return 42;
}

class SerialPort
{
private:
    std::string port_name;
    unsigned int baud_rate{500000};
    char buffer[1024]{};

public:
    serialib serial;
    SerialPort();
    SerialPort(std::string port, unsigned int baud);
    ~SerialPort();

    const char *ReadData();
    bool IsConnectionOpen();
    bool OpenConnection();
    bool CloseConnection();
    bool SendData(std::string msg);
    void SetPortName(std::string port_name);
    std::string GetPortName();
    void SetBaudRate(unsigned int baud_rate);
    unsigned int GetBaudRate();

    static std::vector<std::string> DetectSerialPorts(const std::string port_name, std::vector<std::string> &port_names);
};

#endif /* SERIAL_PORT_H */