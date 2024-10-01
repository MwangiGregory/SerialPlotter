#include "Application.h"
#include "serialib.h"
#include <string>
#include <vector>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define APP_TITLE "Serial Plotter"

class SerialApplication : public Application
{
private:
    char buffer[1024] = {0};
    const unsigned int baud_rate = 500000;
    const char *serial_port_name = "/dev/ttyUSB0";
    serialib serial;
    std::vector<std::string> ports;

    int selected_port_idx = 0;

    bool serial_plotter_close = false;
    bool serial_monitor_close = false;
    ImGuiTextBuffer Buf;
    bool AutoScroll = true;

public:
    SerialApplication(int width, int height, const char *title)
        : Application(width, height, title)
    {
    }

    void Setup()
    {
        if (serial.openDevice(serial_port_name, baud_rate) != 1)
        {
            printf("Failed to connect to %s\n", serial_port_name);
            std::exit(EXIT_FAILURE);
        }
        printf("Successful connection to %s\n", serial_port_name);

        ports = DetectSerialPorts();
    }

    void Destroy()
    {
        serial.closeDevice();
    }

    void Update()
    {
        UpdateSerialMonitor();
        SerialMonitor();

        SerialPlotter();
    }

    void UpdateSerialMonitor()
    {
        // Serial Monitor text
        if (serial.isDeviceOpen() && serial.available() > 0)
        {
            serial.readString(buffer, '\n', 1024);
            Buf.appendf("[Serial]: %s\n", buffer);
        }
        else if (!serial.isDeviceOpen())
        {
            serial.closeDevice();
            if (serial.openDevice(serial_port_name, baud_rate) != 1)
                Buf.appendf("Serial device is not open. Retrying at %s\n", serial_port_name);
            else
                Buf.appendf("Successful connection to %s\n", serial_port_name);
        }
    }

    void SerialPlotter()
    {
        if (ImGui::Begin("Serial SerialPlotter", &serial_plotter_close))
        {
            ImGui::Text("I am an awesome Serial Plotter");

            for (const auto &port : ports)
            {
                ImGui::Text("%s\n", port.c_str());
            }
        }
        ImGui::End();
    }

    void SerialMonitor()
    {
        if (ImGui::Begin("Serial Monitor", &serial_monitor_close))
        {
            // Options menu
            if (ImGui::BeginPopup("Options"))
            {
                ImGui::Checkbox("Auto-scroll", &AutoScroll);
                ImGui::EndPopup();
            }

            // Main window
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::SameLine();
            bool clear = ImGui::Button("Clear");
            ImGui::SameLine();
            bool copy = ImGui::Button("Copy");

            ImGui::SameLine();
            if (ImGui::TreeNode("Serial Ports"))
            {
                if (ImGui::BeginListBox("Serial Ports"))
                {
                    for (int n = 0; n < (int)ports.size(); n++)
                    {
                        const bool is_selected = (selected_port_idx == n);

                        if (ImGui::Selectable(ports[n].c_str(), is_selected))
                            selected_port_idx = n;

                        // // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        // if (is_selected)
                        //     ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndListBox();
                }
                ImGui::TreePop();
            }

            ImGui::Separator();

            if (ImGui::BeginChild("scrolling", ImVec2(0.0, 0.0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
            {
                if (clear)
                    Buf.clear();
                if (copy)
                    ImGui::LogToClipboard();

                const char *buf_begin = Buf.begin();
                const char *buf_end = Buf.end();

                ImGui::TextUnformatted(buf_begin, buf_end);

                if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0);
            }
            ImGui::EndChild();
        }

        ImGui::End();
    }

    std::vector<std::string> DetectSerialPorts()
    {
#define BUF_SIZE 64
        char port_name[BUF_SIZE] = {0};
        std::vector<std::string> port_names;

        for (int i = 1; i < 100; i++)
        {
// Prepare the port name (Windows)
#if defined(_WIN32) || defined(_WIN64)
            sprintf(port_name, "\\\\.\\COM%d", i);
            if (serial.openDevice(port_name, baud_rate) == 1)
            {
                port_names.push_back(port_name);
                serial.closeDevice();
            }
#endif

// Prepare the port names (Linux)
#ifdef __linux__
            // Check for /dev/ttyS*
            // sprintf(port_name, "/dev/ttyS%d", i - 1);
            // if (serial.openDevice(port_name, baud_rate) == 1)
            // {
            //     port_names.push_back(port_name);
            //     serial.closeDevice();
            // }

            // Check for /dev/ttyUSB*
            sprintf(port_name, "/dev/ttyUSB%d", i - 1);
            if (serial.openDevice(port_name, baud_rate) == 1)
            {
                port_names.push_back(port_name);
                serial.closeDevice();
            }

            // Check for /dev/ttyACM*
            sprintf(port_name, "/dev/ttyACM%d", i - 1);
            if (serial.openDevice(port_name, baud_rate) == 1)
            {
                port_names.push_back(port_name);
                serial.closeDevice();
            }

            // Check for /dev/ttyAMA*
            sprintf(port_name, "/dev/ttyAMA%d", i - 1);
            if (serial.openDevice(port_name, baud_rate) == 1)
            {
                port_names.push_back(port_name);
                serial.closeDevice();
            }
#endif
            // Prepare the port names (macOS)
#ifdef __APPLE__
            // Check for /dev/tty.*
            sprintf(device_name, "/dev/tty.%d", i);
            if (device.openDevice(device_name, 115200) == 1)
            {
                port_names.push_back(device_name);
                device.closeDevice();
            }

            // Check for /dev/cu.*
            sprintf(device_name, "/dev/cu.%d", i);
            if (device.openDevice(device_name, 115200) == 1)
            {
                port_names.push_back(device_name);
                device.closeDevice();
            }
#endif

        } /* end of for loop */

        return port_names;
    }
};

Application *CreateApplication()
{
    return new SerialApplication(WIN_WIDTH, WIN_WIDTH, APP_TITLE);
}
