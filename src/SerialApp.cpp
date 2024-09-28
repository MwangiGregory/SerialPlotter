#include "Application.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define APP_TITLE "Serial Plotter"

class SerialApplication : public Application
{
private:
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
    }

    void Update()
    {
        static unsigned long int count = 0;

        ImGui::Begin("Overall");
        ImGui::ShowDemoWindow();
        SerialPlotter();

        // Serial Monitor text
        count++;
        Buf.appendf("[Count var]: %lu\n", count);

        SerialMonitor();

        ImGui::End();
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
