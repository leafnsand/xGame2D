#ifndef __MY_APPLICATION_H__
#define __MY_APPLICATION_H__

#include <xGame2D.h>

using namespace xGame2D;

class MyApplication : public IApplication
{
public:
    MyApplication();
    virtual ~MyApplication();

    virtual int32_t getFrameRate() const override;
    virtual int32_t getWindowWidth() const override;
    virtual int32_t getWindowHeight() const override;
    virtual std::string getWindowName() const override;
    virtual Sprite *getRootSprite() const override;
    virtual int32_t getWindowBackgroundColor() const override;
};

#endif