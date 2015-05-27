#ifndef __X_TIMERHANDLER_H__
#define __X_TIMERHANDLER_H__

#include "base/Object.h"
#include <list>
#include <map>
#include <functional>

#define X_UPDATE(f) std::bind(&f, this, std::placeholders::_1)
#define X_CALLBACK(f) std::bind(&f, this)

namespace xGame2D
{
	class UpdateTimer;
	class UpdateTimerMapNode;
	class CallbackTimerMapNode;

	class TimerHandler : public Object
	{
	constructor:
		TimerHandler();
		virtual ~TimerHandler();

	public:
		void update(float delta);
		void registerCallbackTimer(void *target, const std::function<void()> &callback, float interval, bool paused, const std::string &key);
		void registerCallbackTimer(void *target, const std::function<void()> &callback, float interval, uint32_t repeat, float delay, bool paused, const std::string &key);
		void registerUpdateTimer(void *target, const std::function<void(float)> &update);
		void registerUpdateTimer(void *target, const std::function<void(float)> &update, int32_t priority);
		void registerUpdateTimer(void *target, const std::function<void(float)> &update, int32_t priority, bool paused);
		void unregisterCallbackTimer(void *target, std::string &key);
		void unregisterUpdateTimer(void *target);
		void unregister(void *target);
		void unregisterAll();

		int32_t getTotalFrame();

	private:
		void appendIn(std::list<UpdateTimer *> &list, UpdateTimer *timer);

	protected:
		std::list<UpdateTimer *> negUpdateTimers;
		std::list<UpdateTimer *> zeroUpdateTimers;
		std::list<UpdateTimer *> posUpdateTimers;
		std::map<void *, UpdateTimerMapNode *> allUpdateTimers;
		std::map<void *, CallbackTimerMapNode *> allCallbackTimers;
		bool updateLock;
		int32_t totalFrame;
	};
}

#endif
