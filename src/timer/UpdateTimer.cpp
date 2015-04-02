#include "timer/UpdateTimer.h"

namespace xGame2D
{
    UpdateTimer::UpdateTimer()
    {
    }

    UpdateTimer::~UpdateTimer()
    {
    }

    bool UpdateTimer::init(void *target, const std::function<void(float)> &update, float interval, uint32_t repeat, float delay)
    {
        return false;
    }

    void UpdateTimer::trigger()
    {
    }

    void UpdateTimer::cancel()
    {
    }
}