#include "MyApplication.h"
#include "TestSprite.h"

MyApplication::MyApplication()
{
}

MyApplication::~MyApplication()
{

}

int32_t MyApplication::getFrameRate() const
{
    return 60;
}

int32_t MyApplication::getWindowWidth() const
{
    return 1024;
}

int32_t MyApplication::getWindowHeight() const
{
    return 768;
}

std::string MyApplication::getWindowName() const
{
    return "xGame2D";
}

Sprite *MyApplication::getRootSprite() const
{
    return Object::create<TestSprite>();
}

int32_t MyApplication::getWindowBackgroundColor() const
{
    return ColorGray;
}
