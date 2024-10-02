#ifndef APPLICATION_H
#define APPLICATION_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <stdlib.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "implot.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

// extern void glfw_error_callback(int error, const char *description);

class Application
{
private:
    /* data */
    GLFWwindow *window = nullptr;
    const char *glsl_version = nullptr;

public:
    int display_w = 640;
    int display_h = 480;
    const char *app_title = "Awesome App";

    Application(/* args */);
    Application(int width, int height, const char *app_title);
    virtual ~Application() = 0;

    void Init();
    GLFWwindow *CreateWindow();
    void CreateImGuiContext();
    virtual void Setup() = 0;
    virtual void Update() = 0;
    void Render();
    void Destroy();
};

Application *CreateApplication();

#endif /* APPLICATION_H */