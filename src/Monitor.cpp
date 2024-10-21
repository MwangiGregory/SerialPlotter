#include "Monitor.h"

Monitor::Monitor(SerialPort *dev)
    : device{dev}
{
    ports = SerialPort::DetectSerialPorts(device->GetPortName(), ports);
    window_text = new ImGuiTextBuffer();
}

Monitor::~Monitor()
{
    if (window_text)
        delete window_text;
}

void Monitor::SetSerialPort(SerialPort *dev)
{
    this->device = dev;
}

void Monitor::UpdateWindow()
{

    ImGui::Begin("Monitor");

    MaintainOpenConnection();
    SelectSerialPort();
    SelectBaudRate();
    ImGui::Text("%s\n", serial_open ? "opened" : "closed");
    ImGui::Text("%s\n", device->GetPortName().c_str());
    ImGui::Text("%u\n", device->GetBaudRate());
    ImGui::Text("%s\n", device->IsConnectionOpen() ? "connection open" : "connection closed");

    if (serial_open == false)
    {
        device->CloseConnection();
        if (ImGui::Button("Start Monitor"))
        {
            serial_open = StartMonitor();
            ImGui::Text("Monitor opened");
        }
    }

    if (serial_open)
    {
        if (ImGui::Button("Close Monitor"))
        {
            serial_open = !CloseMonitor();
        }
    }

    if (serial_open)
    {
        const char *buf = device->ReadData();
        if (buf != nullptr)
            window_text->appendf("[Serial]: %s\n", buf);
    }
    DisplayScrollingText();

    if (device->IsConnectionOpen() == false && serial_open == true)
    {
        device->CloseConnection();
        device->OpenConnection();
    }

    ImGui::End();
}

void Monitor::SelectSerialPort()
{
    if (ImGui::TreeNode("Select Serial Port"))
    {
        if (ImGui::BeginListBox("##available serial ports"))
        {
            ports = SerialPort::DetectSerialPorts(device->GetPortName(), ports);

            for (size_t n = 0; n < ports.size(); n++)
            {
                const bool is_selected = (selected_port_idx == (int)n);

                ImGui::PushID((int)n);
                if (ImGui::Selectable(ports[n].c_str(), is_selected))
                {
                    selected_port_idx = (int)n;
                    device->SetPortName(ports[selected_port_idx]);
                }
                ImGui::PopID();
            }
            ImGui::EndListBox();
        }
        ImGui::TreePop();
    }
}

void Monitor::SelectBaudRate()
{
    if (ImGui::TreeNode("Select Baud Rate"))
    {
        if (ImGui::BeginListBox("##available baud rates"))
        {
            for (int n = 0; n < IM_ARRAYSIZE(rates); n++)
            {
                const bool is_selected = (selected_baud_rate_idx == n);

                ImGui::PushID(n);
                if (ImGui::Selectable(std::to_string(rates[n]).c_str(), is_selected))
                {
                    selected_baud_rate_idx = n;
                    device->SetBaudRate(rates[selected_baud_rate_idx]);
                    printf("Baud Rate: %d\n", rates[selected_baud_rate_idx]);
                }
                ImGui::PopID();
            }
            ImGui::EndListBox();
        }
        ImGui::TreePop();
    }
}

bool Monitor::StartMonitor()
{
    if (selected_port_idx >= 0 && selected_baud_rate_idx >= 0)
    {
        if (!device->OpenConnection())
        {
            ImGui::Text("Failed to start Serial Connection");
            return false;
        }
        return true;
    }
    return false;
}

bool Monitor::CloseMonitor()
{
    if (device->IsConnectionOpen())
        return device->CloseConnection();
    return false;
}

void Monitor::DisplayScrollingText(bool auto_scroll)
{
    if (window_text != nullptr)
        if (ImGui::BeginChild("Scrolling", ImVec2(0.0, 0.0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (!window_text->empty())
            {
                const char *window_text_start = window_text->begin();
                const char *window_text_end = window_text->end();

                ImGui::TextUnformatted(window_text_start, window_text_end);

                if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0);
            }

            ImGui::EndChild();
        }
}

void Monitor::MaintainOpenConnection()
{
    if (!device->IsConnectionOpen() && serial_open)
    {
        printf("Connection to %s is closed\n", device->GetPortName().c_str());
    }
}
