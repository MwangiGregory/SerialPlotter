#include "Application.h"

// Main code
int main(int, char **)
{
    Application *app = CreateApplication();

    app->Init();
    auto win = app->CreateWindow();
    app->CreateImGuiContext();
    app->Setup();

    // Main loop
    while (!glfwWindowShouldClose(win))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(win, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        app->Update();
        app->Render();
    }
    app->Destroy();
    delete app;

    return EXIT_SUCCESS;
}