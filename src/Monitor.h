#ifndef MONITOR_H
#define MONITOR_H

#include "imgui.h"
#include "Window.h"
#include "SerialPort.h"
#include <stdio.h>

class Monitor : public Window
{
private:
    SerialPort *device;

    int selected_port_idx = -1;
    std::vector<std::string> ports;

    int selected_baud_rate_idx = -1;
    const int rates[28] = {110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400,
                           56000, 57600, 115200, 128000, 256000, 230400, 460800, 500000, 576000, 921600,
                           1000000, 1152000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000};

    bool serial_open = false;
    ImGuiTextBuffer *window_text;

    void SelectSerialPort();
    void SelectBaudRate();
    bool StartMonitor();
    bool CloseMonitor();
    void DisplayScrollingText(bool auto_scroll = true);
    void MaintainOpenConnection();

public:
    Monitor(SerialPort *dev);
    ~Monitor();

    void SetSerialPort(SerialPort *dev);
    void UpdateWindow() override;
};

#endif /* MONITOR_H */