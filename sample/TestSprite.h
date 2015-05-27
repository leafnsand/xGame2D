//
// Created by leafnsand on 2015/5/23.
//

#ifndef XGAME2D_TESTSPRITE_H
#define XGAME2D_TESTSPRITE_H

#include <xGame2D.h>

using namespace xGame2D;

class TestSprite : public Sprite
{
constructor:
    TestSprite()
    {
    };

    virtual ~TestSprite()
    {
    }

public:
    virtual bool init() override
    {
        if (Sprite::init())
        {
            std::string name = "resources/mario.png";
            auto image = Object::create<Image>(name);
            addChild(image);

            Application::getInstance()->timerHandler->registerCallbackTimer(this, X_CALLBACK(callback), 1, false, name);
            Application::getInstance()->timerHandler->registerUpdateTimer(this, X_UPDATE(update));

            return true;
        }
        return false;
    }

    int32_t total, count;

    void update(float delta)
    {
        total += 1 / delta;
        ++count;
    }

    void callback()
    {
        Console::log << "frame rate: " << total / count << Console::endl;
        total = count = 0;
    }
};


#endif //XGAME2D_TESTSPRITE_H
