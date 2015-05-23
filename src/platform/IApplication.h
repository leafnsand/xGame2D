#ifndef __X_IAPPLICATION_H__
#define __X_IAPPLICATION_H__

#include <cstdint>
#include <string>

namespace xGame2D
{
    class Sprite;
    class IApplication
    {
    public:
        virtual int32_t getFrameRate() const = 0;
        virtual int32_t getWindowWidth() const = 0;
        virtual int32_t getWindowHeight() const = 0;
        virtual std::string getWindowName() const = 0;
        virtual Sprite *getRootSprite() const = 0;
        virtual int32_t getWindowBackgroundColor() const = 0;
    };
}

#endif