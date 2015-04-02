#ifndef __X_TIMER_H__
#define __X_TIMER_H__

#include "base/Object.h"

namespace xGame2D
{
    class Timer : public Object
    {
    constructor:
        Timer();
        virtual ~Timer();

    public:
        bool init(float interval, uint32_t repeat, float delay);
        void update(float delta);
        virtual void trigger() = 0;
        virtual void cancel() = 0;

    protected:
        float elapsed, delay, interval;
        bool runForever, useDelay;
        uint32_t timesExecuted, repeat;
    };
}

#endif //__X_TIMER_H__
