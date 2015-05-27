#ifndef __X_APPLICATIONWINDOWS_H__
#define __X_APPLICATIONWINDOWS_H__

#include "platform/Application.h"
#include <windows.h>

namespace xGame2D
{
    class ApplicationWindows : public Application
    {
    public:
        ApplicationWindows();
        ~ApplicationWindows();

        virtual int32_t run(const IApplication *app) override;

    private:
        void setFrameRate(int32_t frameRate);

    protected:
        LARGE_INTEGER deltaTimePerFrame;
    };
}

#endif