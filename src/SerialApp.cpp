#include "Application.h"

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define APP_TITLE "Serial Plotter"

class SerialApplication : public Application
{
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
        bool show_demo_window = true;

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
    }
};

Application *CreateApplication()
{
    return new SerialApplication(WIN_WIDTH, WIN_WIDTH, APP_TITLE);
}