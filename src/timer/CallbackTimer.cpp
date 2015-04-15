#include "timer/CallbackTimer.h"
#include "timer/TimerHandler.h"
#include "Game.h"

namespace xGame2D
{
    CallbackTimer::CallbackTimer()
    : target(nullptr)
    , callback(nullptr)
    {
    }

    CallbackTimer::~CallbackTimer()
    {
    }

    bool CallbackTimer::init(void *target, std::function<void()> &callback, float interval, uint32_t repeat, float delay, std::string &key)
    {
        if (Timer::init(interval, repeat, delay))
        {
            this->target = target;
            this->callback = callback;
            this->key = key;
            return true;
        }
        return false;
    }

    void CallbackTimer::trigger()
    {
        if (callback)
        {
            callback();
        }
    }

    void CallbackTimer::cancel()
    {
        Game::getInstance()->timerHandler->unregisterCallbackTimer(target, key);
    }
}