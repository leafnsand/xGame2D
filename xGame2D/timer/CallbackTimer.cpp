#include "timer/CallbackTimer.h"
#include "timer/TimerHandler.h"
#include "platform/Application.h"

namespace xGame2D
{
	CallbackTimer::CallbackTimer():
		target(nullptr),
		callback(nullptr)
	{
	}

	CallbackTimer::~CallbackTimer()
	{
	}

	bool CallbackTimer::init(void *target, const std::function<void()> &callback, float interval, uint32_t repeat, float delay, const std::string &key)
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
		Application::getInstance()->timerHandler->unregisterCallbackTimer(target, key);
	}
}
