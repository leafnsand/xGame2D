#include "timer/UpdateTimer.h"
#include "timer/TimerHandler.h"
#include "platform/Application.h"

namespace xGame2D
{
	UpdateTimer::UpdateTimer():
		target(nullptr),
		updateFunction(nullptr),
		priority(0)
	{
	}

	UpdateTimer::~UpdateTimer()
	{
	}

	bool UpdateTimer::init(void *target, const std::function<void(float)> &updateFunction, int32_t priority, bool paused)
	{
		if (Timer::init(0.0f, UINT32_MAX, 0.0f))
		{
			this->target = target;
			this->updateFunction = updateFunction;
			this->priority = priority;
			this->paused = paused;
			return true;
		}
		return false;
	}

	void UpdateTimer::trigger()
	{
		if (!paused && updateFunction)
		{
			updateFunction(elapsed);
		}
	}

	void UpdateTimer::cancel()
	{
		Application::getInstance()->timerHandler->unregisterUpdateTimer(target);
	}
}
