#include "timer/UpdateTimer.h"
#include "timer/TimerHandler.h"
#include "Game.h"

namespace xGame2D
{
    UpdateTimer::UpdateTimer()
    : target(nullptr)
    , update(nullptr)
    {
    }

    UpdateTimer::~UpdateTimer()
    {
    }

    bool UpdateTimer::init(void *target, const std::function<void(float)> &update, float interval, uint32_t repeat, float delay)
    {
        if (Timer::init(interval, repeat, delay))
        {
            this->target = target;
            this->update = update;
            return true;
        }
        return false;
    }

    void UpdateTimer::trigger()
    {
        update(elapsed);
    }

    void UpdateTimer::cancel()
    {
        Game::getInstance()->timerHandler->unregisterUpdateTimer(target);
    }
}