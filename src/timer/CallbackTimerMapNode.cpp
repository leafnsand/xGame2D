#include "timer/CallbackTimerMapNode.h"
#include "timer/CallbackTimer.h"

namespace xGame2D
{
	CallbackTimerMapNode::CallbackTimerMapNode():
		target(nullptr),
		paused(false)
	{
	}

	CallbackTimerMapNode::~CallbackTimerMapNode()
	{
	}

	void CallbackTimerMapNode::pushTimer(const std::function<void()> &callback, float interval, uint32_t repeat, float delay, const std::string &key)
	{
		for (auto iterator = timers.begin(); iterator != timers.end(); ++iterator)
		{
			if ((*iterator)->key == key)
			{
				Console::log << "Timer already scheduled. Updating interval from : " << (*iterator)->interval << " to " << interval << Console::endl;
				(*iterator)->interval = interval;
				return;
			}
		}

		auto timer = Object::create<CallbackTimer>(target, callback, interval, repeat, delay, key);
		timers.push_back(timer);
		timer->retain();
	}

	void CallbackTimerMapNode::popTimer(std::string &key)
	{
		auto iterator = std::find_if(timers.begin(), timers.end(), [key] (const CallbackTimer *t) { return t->key == key; });
		if (iterator != timers.end())
		{
			auto timer = *iterator;
			timers.erase(iterator);
			timer->release();
		}
	}

	void CallbackTimerMapNode::popAll()
	{
		for (auto iterator = timers.begin(); iterator != timers.end(); ++iterator)
		{
			(*iterator)->release();
		}
		timers.clear();
	}

	void CallbackTimerMapNode::update(float delta)
	{
		if (!paused)
		{
			this->retain();
			auto iterator = timers.begin();
			while (iterator != timers.end())
			{
				auto timer = *iterator;
				++iterator;
				timer->update(delta);
			}
			this->release();
		}
	}
}
