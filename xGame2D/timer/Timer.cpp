#include "timer/Timer.h"

namespace xGame2D
{
	Timer::Timer():
		elapsed(-1),
		delay(0.0f),
		interval(0.0f),
		runForever(false),
		useDelay(false),
		timesExecuted(0),
		repeat(0)
	{
	}

	Timer::~Timer()
	{
	}

	bool Timer::init(float interval, uint32_t repeat, float delay)
	{
		elapsed = -1;
		this->interval = interval;
		this->delay = delay;
		useDelay = delay > 0.0f;
		this->repeat = repeat;
		runForever = repeat == UINT32_MAX;
		return true;
	}

	void Timer::update(float delta)
	{
		if (elapsed == -1)
		{
			elapsed = 0;
			timesExecuted = 0;
		}
		else
		{
			if (runForever && !useDelay)
			{
				elapsed += delta;
				if (elapsed >= interval)
				{
					trigger();
					elapsed = 0;
				}
			}
			else
			{
				elapsed += delta;
				if (useDelay)
				{
					if (elapsed >= delay)
					{
						trigger();
						elapsed = elapsed - delay;
						timesExecuted += 1;
						useDelay = false;
					}
				}
				else
				{
					if (elapsed >= interval)
					{
						trigger();
						elapsed = 0;
						timesExecuted += 1;
					}
				}
				if (!runForever && timesExecuted > repeat)
				{
					cancel();
				}
			}
		}
	}
}
