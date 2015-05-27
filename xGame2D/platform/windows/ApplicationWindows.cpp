#include "platform/windows/ApplicationWindows.h"
#include "platform/windows/OpenGLWindows.h"

namespace xGame2D
{
    Application *Application::getInstance()
    {
        if (instance == nullptr)
        {
            instance = new ApplicationWindows();
        }
        return instance;
    }

    ApplicationWindows::ApplicationWindows()
    {
    }

    ApplicationWindows::~ApplicationWindows()
    {
        if (gl)
        {
            gl->release();
        }
    }

    int32_t ApplicationWindows::run(const IApplication *app)
    {
        Application::run(app);

        LARGE_INTEGER lastUpdate;
        LARGE_INTEGER now;

        setFrameRate(this->app->getFrameRate());
        QueryPerformanceCounter(&lastUpdate);

        gl = Object::generate<OpenGLWindows>(this->app->getWindowName(), this->app->getWindowWidth(), this->app->getWindowHeight());
        addRootSprite();
        while (!gl->windowShouldClose())
        {
            QueryPerformanceCounter(&now);
            if (now.QuadPart - lastUpdate.QuadPart > deltaTimePerFrame.QuadPart)
            {
                lastUpdate.QuadPart = now.QuadPart - (now.QuadPart % deltaTimePerFrame.QuadPart);
                mainloop();
            }
            else
            {
                Sleep(1);
            }
        }
        return 0;
    }

    void ApplicationWindows::setFrameRate(int32_t frameRate)
    {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        deltaTimePerFrame.QuadPart = frequency.QuadPart / frameRate;
    }
}
