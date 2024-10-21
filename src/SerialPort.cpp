#include "SerialPort.h"

SerialPort::SerialPort()
{
}

SerialPort::SerialPort(std::string port, unsigned int baud)
    : port_name(port), baud_rate(baud)
{
    serial.openDevice(port_name.c_str(), baud_rate);
}

SerialPort::~SerialPort()
{
    serial.closeDevice();
}

const char *SerialPort::ReadData()
{
    if (serial.isDeviceOpen() && serial.available() > 0)
    {
        memset(buffer, 0, 1024);
        serial.readString(buffer, '\n', 1024);
        printf("%s\n", buffer);
        return buffer;
    }
    else
        return nullptr;
}

bool SerialPort::IsConnectionOpen()
{
    return serial.isDeviceOpen();
}

bool SerialPort::OpenConnection()
{
    if (!serial.isDeviceOpen() && serial.openDevice(port_name.c_str(), baud_rate))
        return true;
    return false;
}

bool SerialPort::CloseConnection()
{
    if (!serial.isDeviceOpen())
        return false;
    serial.closeDevice();
    return true;
}

bool SerialPort::SendData(std::string msg)
{
    if (msg.size() == 0)
        return false;

    serial.writeString(msg.c_str());
    return true;
}

void SerialPort::SetPortName(std::string port_name)
{
    this->port_name = port_name;
}

void SerialPort::SetBaudRate(unsigned int baud_rate)
{
    this->baud_rate = baud_rate;
}

unsigned int SerialPort::GetBaudRate()
{
    return this->baud_rate;
}

std::string SerialPort::GetPortName()
{
    if (port_name.empty())
        return std::string("None");

    return port_name;
}

std::vector<std::string> SerialPort::DetectSerialPorts(const std::string port, std::vector<std::string> &port_names)
{
    serialib tester;
    int baud = 115200;
    char generated_port_name[64];
    port_names.clear();

    for (int i = 1; i < 100; i++)
    {
// Prepare the port name (Windows)`
#if defined(_WIN32) || defined(_WIN64)
        sprintf(generated_port_name, "\\\\.\\COM%d", i);
        if (port == std::string(generated_port_name))
            port_names.push_back(generated_port_name);
        else if (tester.openDevice(generated_port_name, baud) == 1)
        {
            port_names.push_back(generated_port_name);
            tester.closeDevice();
        }
#endif

// Prepare the port names (Linux)
#ifdef __linux__

        // Check for /dev/ttyUSB*
        sprintf(generated_port_name, "/dev/ttyUSB%d", i - 1);
        if (port == std::string(generated_port_name))
            port_names.push_back(generated_port_name);
        else if (tester.openDevice(generated_port_name, baud) == 1)
        {
            port_names.push_back(generated_port_name);
            tester.closeDevice();
        }

        // Check for /dev/ttyACM*
        sprintf(generated_port_name, "/dev/ttyACM%d", i - 1);
        if (port == std::string(generated_port_name))
            port_names.push_back(generated_port_name);
        else if (tester.openDevice(generated_port_name, baud) == 1)
        {
            port_names.push_back(generated_port_name);
            tester.closeDevice();
        }

        // Check for /dev/ttyAMA*
        sprintf(generated_port_name, "/dev/ttyAMA%d", i - 1);
        if (port == std::string(generated_port_name))
            port_names.push_back(generated_port_name);
        else if (tester.openDevice(generated_port_name, baud) == 1)
        {
            port_names.push_back(generated_port_name);
            tester.closeDevice();
        }
#endif
        // Prepare the port names (macOS)
#ifdef __APPLE__
        // Check for /dev/tty.*
        sprintf(generated_port_name, "/dev/tty.%d", i);
        if (port == std::string(generated_port_name))
            port_names.push_back(generated_port_name);
        else if (tester.openDevice(generated_port_name, baud) == 1)
        {
            port_names.push_back(generated_port_name);
            tester.closeDevice();
        }

        // Check for /dev/cu.*
        sprintf(generated_port_name, "/dev/cu.%d", i);
        if (port == std::string(generated_port_name))
            port_names.push_back(generated_port_name);
        else if (tester.openDevice(generated_port_name, baud) == 1)
        {
            port_names.push_back(generated_port_name);
            tester.closeDevice();
        }
#endif
    } /* end of for loop */

    return port_names;
}
