#ifndef __X_OPENGLWINDOWS_H__
#define __X_OPENGLWINDOWS_H__

#include "platform/OpenGL.h"
#include <GLFW/glfw3.h>

namespace xGame2D
{
    class OpenGLWindows : public OpenGL
    {
    constructor:
        OpenGLWindows();

        virtual ~OpenGLWindows();

    public:
        OVERRIDE_DEFAULT_INIT

        virtual bool init(const std::string &name, int32_t width, int32_t height) override;

        virtual void pollEvents() override;

        virtual void swapBuffers() override;

        virtual bool windowShouldClose() override;

        static void onGLFWError(int errorID, const char* errorDesc);

    protected:
        GLFWwindow *window;
    };
}

#endif