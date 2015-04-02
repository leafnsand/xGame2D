#ifndef __X_TIMERHANDLER_H__
#define __X_TIMERHANDLER_H__

#include "base/Object.h"
#include <list>
#include <map>

namespace xGame2D
{
    class UpdateTimer;
    class UpdateTimerMapNode;
    class CallbackTimer;
    class CallbackTimerMapNode;

    class TimerHandler : public Object
    {
    constructor:
        TimerHandler();
        virtual ~TimerHandler();

    public:
        void update(float delta);
        void registerCallbackTimer(void *target, const std::function<void()> &callback, float interval, uint32_t repeat, float delay, bool paused, std::string &key);
        void registerUpdateTimer(void *target, const std::function<void(float)> &update, int32_t priority, bool paused);
        void unregisterCallbackTimer(void *target, std::string &key);
        void unregisterUpdateTimer(void *target);
        void unregister(void *target);
        void unregisterAll();

    protected:
        std::list<UpdateTimer *> negUpdateTimers;
        std::list<UpdateTimer *> zeroUpdateTimers;
        std::list<UpdateTimer *> posUpdateTimers;
        std::map<void *, UpdateTimerMapNode *> allUpdateTimers;
        std::list<CallbackTimer *> callbackTimers;
        std::map<void *, CallbackTimerMapNode *> allCallbackTimers;
        CallbackTimer *currentCallbackTimer;
        bool currentCallbackTimerSalvaged, updateLock;
    };
}

#endif