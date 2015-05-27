#include "display/Sprite.h"
#include "display/Stage.h"
#include "platform/Application.h"
#include "render/ProgramCache.h"
#include "render/RenderSupport.h"
#include "render/Context.h"
#include "timer/TimerHandler.h"
#include "textures/TextureCache.h"

namespace xGame2D
{
    Application *Application::instance = nullptr;

    Application::Application()
    {
    }

    Application::~Application()
    {
        if (app)
        {
            delete app;
        }
        if (programCache)
        {
            programCache->release();
        }
        if (textureCache)
        {
            textureCache->release();
        }
        if (context)
        {
            context->release();
        }
        if (support)
        {
            support->release();
        }
        if (stage)
        {
            stage->release();
        }
        if (timerHandler)
        {
            timerHandler->release();
        }
        AutoreleasePool::getInstance()->destroy();
    }

    void Application::addRootSprite()
    {
        stage->addChild(app->getRootSprite());
    }

    int32_t Application::run(const IApplication *app)
    {
        this->app = app;
        this->programCache = Object::generate<ProgramCache>();
        this->textureCache = Object::generate<TextureCache>();
        this->context = Object::generate<Context>();
        this->support = Object::generate<RenderSupport>();
        this->stage = Object::generate<Stage>(this->app->getWindowWidth(), this->app->getWindowHeight(), this->app->getWindowBackgroundColor());
        this->timerHandler = Object::generate<TimerHandler>();
        lastUpdate = std::chrono::system_clock::now();
        return 0;
    }

    void Application::mainloop()
    {
        auto now = std::chrono::system_clock::now();
        auto duration = now - lastUpdate;
        auto delta = std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();
#if DEBUG || _DEBUG
        if (delta > 0.2f)
        {
            delta = 1 / 60.0f;
        }
#endif
        support->prepare();
        support->nextFrame();
        stage->render(support);
        support->finishQuadBatch();
        timerHandler->update(delta);
#if DEBUG || _DEBUG
        RenderSupport::checkForOpenGLError();
        AutoreleasePool::getInstance()->dump();
#endif
        AutoreleasePool::getInstance()->clear();
        lastUpdate = now;
        gl->swapBuffers();
        gl->pollEvents();
    }
}