#include "timer/TimerHandler.h"
#include "timer/CallbackTimer.h"
#include "timer/UpdateTimer.h"
#include "timer/CallbackTimerMapNode.h"
#include "timer/UpdateTimerMapNode.h"

namespace xGame2D
{
	TimerHandler::TimerHandler():
		updateLock(false),
		totalFrame(0)
	{
	}

	TimerHandler::~TimerHandler()
	{
		unregisterAll();
	}

	void TimerHandler::update(float delta)
	{
		++totalFrame;
		std::list<UpdateTimer*>::iterator updateIterator;
		UpdateTimer *updateTimer = nullptr;

		updateIterator = negUpdateTimers.begin();
		while (updateIterator != negUpdateTimers.end())
		{
			updateTimer = *updateIterator;
			++updateIterator;
			updateTimer->update(delta);
		}
		updateIterator = zeroUpdateTimers.begin();
		while (updateIterator != zeroUpdateTimers.end())
		{
			updateTimer = *updateIterator;
			++updateIterator;
			updateTimer->update(delta);
		}
		updateIterator = posUpdateTimers.begin();
		while (updateIterator != posUpdateTimers.end())
		{
			updateTimer = *updateIterator;
			++updateIterator;
			updateTimer->update(delta);
		}

		std::map<void *, CallbackTimerMapNode*>::iterator callbackIterator;
		CallbackTimerMapNode *callbackNode = nullptr;

		callbackIterator = allCallbackTimers.begin();
		while (callbackIterator != allCallbackTimers.end())
		{
			callbackNode = callbackIterator->second;
			++callbackIterator;
			callbackNode->update(delta);
		}
	}

	void TimerHandler::registerCallbackTimer(void *target, const std::function<void()> &callback, float interval, uint32_t repeat, float delay, bool paused, std::string &key)
	{
		X_ASSERT(target, "target must be non-nullptr");
		X_ASSERT(!key.empty(), "key should not be empty!");

		if (allCallbackTimers.find(target) == allCallbackTimers.end())
		{
			auto newCallbackTimerMapNode = Object::create<CallbackTimerMapNode>();
			newCallbackTimerMapNode->target = target;
			newCallbackTimerMapNode->paused = paused;
			allCallbackTimers[target] = newCallbackTimerMapNode;
			newCallbackTimerMapNode->retain();
		}

		auto node = allCallbackTimers[target];
		X_ASSERT(node->paused == paused, "node found, but with different pause");
		node->pushTimer(callback, interval, repeat, delay, key);
	}

	void TimerHandler::unregisterCallbackTimer(void *target, std::string &key)
	{
		if (target == nullptr || key.empty())
		{
			return;
		}

		auto iterator = allCallbackTimers.find(target);
		if (iterator != allCallbackTimers.end())
		{
			auto node = iterator->second;
			node->popTimer(key);
			if (node->timers.size() == 0)
			{
				allCallbackTimers.erase(iterator);
				node->release();
			}
		}
	}

	void TimerHandler::registerUpdateTimer(void *target, const std::function<void(float)> &update, int32_t priority, bool paused)
	{
		X_ASSERT(target, "target must be non-nullptr");

		auto iterator = allUpdateTimers.find(target);
		if (iterator != allUpdateTimers.end())
		{
			auto node = iterator->second;
			if (node->timer->priority != priority)
			{
				if (!updateLock)
				{
					unregister(target);
				}
				else
				{
					Console::Log("warning: you CANNOT change update priority in register function");
					node->timer->paused = paused;
					return;
				}
			}
			else
			{
				node->timer->paused = paused;
				return;
			}
		}

		auto newUpdateTimer = Object::create<UpdateTimer>(target, update, priority, paused);

		if (priority == 0)
		{
			appendIn(zeroUpdateTimers, newUpdateTimer);
		}
		else if (priority < 0)
		{
			appendIn(negUpdateTimers, newUpdateTimer);
		}
		else
		{
			appendIn(posUpdateTimers, newUpdateTimer);
		}
	}

	void TimerHandler::unregisterUpdateTimer(void *target)
	{
		if (target == nullptr)
		{
			return;
		}
		auto iterator = allUpdateTimers.find(target);
		if (iterator != allUpdateTimers.end())
		{
			auto node = iterator->second;
			node->remove();
			allUpdateTimers.erase(node->timer->target);
			node->release();
		}
	}

	void TimerHandler::unregister(void *target)
	{
		unregisterUpdateTimer(target);
		auto iterator = allCallbackTimers.find(target);
		if (iterator != allCallbackTimers.end())
		{
			auto node = iterator->second;
			node->popAll();
			allCallbackTimers.erase(iterator);
			node->release();
		}
	}

	void TimerHandler::unregisterAll()
	{
		for (auto updateIterator = allUpdateTimers.begin(); updateIterator != allUpdateTimers.end(); ++updateIterator)
		{
			auto updateNode = updateIterator->second;
			updateNode->remove();
			updateNode->release();
		}
		allUpdateTimers.clear();
		for (auto callbackIterator = allCallbackTimers.begin(); callbackIterator != allCallbackTimers.end(); ++callbackIterator)
		{
			auto callbackNode = callbackIterator->second;
			callbackNode->popAll();
			callbackNode->release();
		}
		allCallbackTimers.clear();
	}


	int32_t TimerHandler::getTotalFrame()
	{
		return totalFrame;
	}

	void TimerHandler::appendIn(std::list<UpdateTimer*> &list, UpdateTimer *timer)
	{
		auto newUpdateTimerMapNode = Object::create<UpdateTimerMapNode>(&list, timer);
		allUpdateTimers[timer->target] = newUpdateTimerMapNode;
		newUpdateTimerMapNode->retain();
	}
}
