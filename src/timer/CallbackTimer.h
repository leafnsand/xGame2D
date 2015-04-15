#ifndef __X_CALLBACKTIMER_H__
#define __X_CALLBACKTIMER_H__

#include "timer/Timer.h"
#include <functional>

namespace xGame2D
{
    class CallbackTimer : public Timer
    {
    constructor:
        CallbackTimer();
        virtual ~CallbackTimer();

    public:
        bool init(void *target, std::function<void()> &callback, float interval, uint32_t repeat, float delay, std::string &key);
        virtual void trigger() override;
        virtual void cancel() override;

    protected:
        void *target;
        std::function<void()> callback;
        std::string key;
    };

}

#endif //__X_CALLBACKTIMER_H__
