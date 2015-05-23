#include "platform/windows/OpenGLWindows.h"

namespace xGame2D
{
    OpenGLWindows::OpenGLWindows():
    window(nullptr)
    {
        glfwSetErrorCallback(onGLFWError);
        glfwInit();
    }

    OpenGLWindows::~OpenGLWindows()
    {
        glfwTerminate();
    }

    bool OpenGLWindows::init(const std::string &name, int32_t width, int32_t height)
    {
        if (OpenGL::init(name, width, height))
        {
            glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
            window = glfwCreateWindow(width, height, this->name.c_str(), nullptr, nullptr);
            glfwMakeContextCurrent(window);
            if (glewInit() != GLEW_OK)
            {
                Console::Error("GLEW init error.");
                return false;
            }
        }
        return true;
    }

    void OpenGLWindows::pollEvents()
    {
        glfwPollEvents();
    }

    void OpenGLWindows::swapBuffers()
    {
        if (window)
        {
            glfwSwapBuffers(window);
        }
    }

    bool OpenGLWindows::windowShouldClose()
    {
        if (window)
        {
            return glfwWindowShouldClose(window) != 0;
        }
        else
        {
            return true;
        }
    }

    void OpenGLWindows::onGLFWError(int errorID, const char *errorDesc)
    {
        Console::Error("glfw error, code: %d, string: %s.", errorID, errorDesc);
    }
}