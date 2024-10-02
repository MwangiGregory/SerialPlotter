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
    SerialApplication(int width, int height, const char *title)
        : Application(width, height, title)
    {
    }

    void Setup()
    {
        ports = DetectSerialPorts();
    }

    void Destroy()
    {
        serial.closeDevice();
    }

    void Update()
    {
        ports = DetectSerialPorts();

        UpdateSerialMonitor();
        SerialMonitor();

        SerialPlotter();
        DebugWindow();
    }

    void UpdateSerialMonitor()
    {
        // Serial Monitor text
        if (start_monitor && serial.isDeviceOpen() && serial.available() > 0)
        {
            serial.readString(buffer, '\n', 1024);
            Buf.appendf("[Serial]: %s\n", buffer);
        }
        else if (start_monitor == false)
            serial.closeDevice();
        else if (!serial.isDeviceOpen() && start_monitor)
        {
            // serial.closeDevice();
            if (selected_port_idx > -1 && selected_baud_rate_idx > 0) // Check for a selected item
            {
                serial_port_name = ports[selected_port_idx];
                if (serial.openDevice(serial_port_name.c_str(), rates[selected_baud_rate_idx]) != 1)
                    Buf.appendf("Serial device is not open. Retrying at %s\n", serial_port_name.c_str());
                else
                    Buf.appendf("Successful connection to %s\n", serial_port_name.c_str());
            }
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

    void DebugWindow()
    {
        bool show_demo_window;
        ImPlot::ShowDemoWindow(&show_demo_window);
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

            if (ImGui::TreeNode("Select a Serial Port"))
            {
                if (ImGui::BeginListBox(""))
                {
                    for (int n = 0; n < (int)ports.size(); n++)
                    {
                        const bool is_selected = (selected_port_idx == n);
                        if (ImGui::Selectable(ports[n].c_str(), is_selected))
                            selected_port_idx = n;
                    }
                    ImGui::EndListBox();
                }
                ImGui::TreePop();
            }

            ImGui::SameLine();
            if (ImGui::TreeNode("Select a Baud Rate"))
            {
                if (ImGui::BeginListBox(""))
                {
                    for (int n = 0; n < IM_ARRAYSIZE(rates); n++)
                    {
                        const bool is_selected = (selected_baud_rate_idx == n);
                        if (ImGui::Selectable(std::to_string(rates[n]).c_str(), is_selected))
                            selected_baud_rate_idx = n;
                    }
                    ImGui::EndListBox();
                }
                ImGui::TreePop();
            }

            ImGui::Separator();

            ImGui::Text("Connected to: %40s", serial_port_name.c_str());
            ImGui::SameLine();
            static bool start_button_show = true;

            if (start_button_show)
                if (ImGui::Button("Start Monitor"))
                    start_monitor = true;

            start_button_show = start_monitor ? false : true;

            if (start_button_show == false)
                if (ImGui::Button("Stop Monitor"))
                    start_monitor = false;

            ImGui::Separator();

            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::SameLine();
            bool clear = ImGui::Button("Clear");
            ImGui::SameLine();
            bool copy = ImGui::Button("Copy");

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
        serialib tester;
        int baud = 115200;
        char port_name[64];

        std::vector<std::string> port_names;

        for (int i = 1; i < 100; i++)
        {
// Prepare the port name (Windows)`
#if defined(_WIN32) || defined(_WIN64)
            sprintf(port_name, "\\\\.\\COM%d", i);
            if (serial_port_name == std::string(port_name))
            {
                port_names.push_back(port_name);
                continue;
            }
            else if (tester.openDevice(port_name, baud) == 1)
            {
                port_names.push_back(port_name);
                tester.closeDevice();
            }
#endif

// Prepare the port names (Linux)
#ifdef __linux__

            // Check for /dev/ttyUSB*
            sprintf(port_name, "/dev/ttyUSB%d", i - 1);
            if (serial_port_name == std::string(port_name))
            {
                port_names.push_back(port_name);
                continue;
            }
            else if (tester.openDevice(port_name, baud) == 1)
            {
                port_names.push_back(port_name);
                tester.closeDevice();
            }

            // Check for /dev/ttyACM*
            sprintf(port_name, "/dev/ttyACM%d", i - 1);
            if (serial_port_name == std::string(port_name))
            {
                port_names.push_back(port_name);
                continue;
            }
            else if (tester.openDevice(port_name, baud) == 1)
            {
                port_names.push_back(port_name);
                tester.closeDevice();
            }

            // Check for /dev/ttyAMA*
            sprintf(port_name, "/dev/ttyAMA%d", i - 1);
            if (serial_port_name == std::string(port_name))
            {
                port_names.push_back(port_name);
                continue;
            }
            else if (tester.openDevice(port_name, baud) == 1)
            {
                port_names.push_back(port_name);
                tester.closeDevice();
            }
#endif
            // Prepare the port names (macOS)
#ifdef __APPLE__
            // Check for /dev/tty.*
            sprintf(device_name, "/dev/tty.%d", i);
            if (serial_port_name == std::string(port_name))
            {
                port_names.push_back(port_name);
                continue;
            }
            else if (tester.openDevice(port_name, baud) == 1)
            {
                port_names.push_back(port_name);
                tester.closeDevice();
            }

            // Check for /dev/cu.*
            sprintf(device_name, "/dev/cu.%d", i);
            if (serial_port_name == std::string(port_name))
            {
                port_names.push_back(port_name);
                continue;
            }
            else if (tester.openDevice(port_name, baud) == 1)
            {
                port_names.push_back(port_name);
                tester.closeDevice();
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
