#ifndef __X_UPDATETIMER_H__
#define __X_UPDATETIMER_H__

#include "timer/Timer.h"
#include <functional>

namespace xGame2D
{
    class UpdateTimer : public Timer
    {
    constructor:
        UpdateTimer();
        virtual ~UpdateTimer();

    public:
        bool init(void *target, const std::function<void(float)> &update, float interval, uint32_t repeat, float delay);
        virtual void trigger() override;
        virtual void cancel() override;

    protected:
        void *target;
        std::function<void(float)> update;
    };
}

#endif //__X_UPDATELISTNODE_H__
