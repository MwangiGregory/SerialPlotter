#ifndef SERIAL_APP_H
#define SERIAL_APP_H

#include "Application.h"
#include "serialib.h"
#include <string>
#include <vector>

class SerialApplication : public Application
{
private:
    char buffer[1024] = {0};

    serialib serial;
    std::vector<std::string> ports;

    std::string serial_port_name;
    int selected_port_idx = -1;

    const unsigned int baud_rate = 0;
    const int rates[28] = {110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400,
                           56000, 57600, 115200, 128000, 256000, 230400, 460800, 500000, 576000, 921600,
                           1000000, 1152000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000};
    int selected_baud_rate_idx = -1;

    bool start_monitor = false;

    bool serial_plotter_close = false;
    bool serial_monitor_close = false;
    ImGuiTextBuffer Buf;
    bool AutoScroll = true;

public:
    SerialApplication(int width, int height, const char *title);
    void Setup() override;
    void Update() override;
    void Destroy();
    void UpdateSerialMonitor();
    void SerialPlotter();
    void DebugWindow();
    void SerialMonitor();
    std::vector<std::string> DetectSerialPorts();
    // static Application *CreateApplication();
};

#endif /* SERIAL_APP_H */