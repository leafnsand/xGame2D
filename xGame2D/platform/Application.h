#ifndef __X_APPLICATION_H__
#define __X_APPLICATION_H__

#include "platform/IApplication.h"
#include "platform/OpenGL.h"
#include <chrono>

namespace xGame2D
{
    class RenderSupport;
    class ProgramCache;
    class TextureCache;
    class Stage;
    class Context;
    class TimerHandler;

    class Application
    {
    public:
        static Application *getInstance();
        ~Application();

        virtual int32_t run(const IApplication *app);

    protected:
        Application();
        void mainloop();
        void addRootSprite();

    public:
        ProgramCache *programCache;
        TextureCache *textureCache;
        Stage *stage;
        Context *context;
        TimerHandler *timerHandler;

    protected:
        static Application *instance;
        const IApplication *app;
        OpenGL *gl;
        RenderSupport *support;
        std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    };
}

#endif