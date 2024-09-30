#include "Application.h"
#include "serialib.h"

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
};

Application *CreateApplication()
{
    return new SerialApplication(WIN_WIDTH, WIN_WIDTH, APP_TITLE);
}
