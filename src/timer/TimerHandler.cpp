#include "timer/TimerHandler.h"

namespace xGame2D
{
    TimerHandler::TimerHandler()
    : currentCallbackTimerSalvaged(false)
    , updateLock(false)
    {
    }

    TimerHandler::~TimerHandler()
    {
        unregisterAll();
    }

    void TimerHandler::update(float delta)
    {

    }

    void TimerHandler::registerCallbackTimer(void *target, const std::function<void()> &callback, float interval, uint32_t repeat, float delay, bool paused, std::string &key)
    {
        X_ASSERT(target, "target must be non-nullptr");
        X_ASSERT(!key.empty(), "key should not be empty!");
        if (!allCallbackTimers.find(target))
        {

        }

    }

    void TimerHandler::unregisterCallbackTimer(void *target, std::string &key)
    {

    }

    void TimerHandler::registerUpdateTimer(void *target, const std::function<void(float)> &update, int32_t priority, bool paused)
    {

    }

    void TimerHandler::unregisterUpdateTimer(void *target)
    {

    }

    void TimerHandler::unregister(void *target)
    {

    }

    void TimerHandler::unregisterAll()
    {

    }
}