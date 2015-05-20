#ifndef __X_CALLBACKTIMERMAPNODE_H__
#define __X_CALLBACKTIMERMAPNODE_H__

#include "base/Object.h"
#include <list>
#include <functional>

namespace xGame2D
{
	class CallbackTimer;

	class CallbackTimerMapNode : public Object
	{
	constructor:
		CallbackTimerMapNode();
		virtual ~CallbackTimerMapNode();

	public:
		void pushTimer(const std::function<void()> &callback, float interval, uint32_t repeat, float delay, std::string &key);
		void popTimer(std::string &key);
		void popAll();
		void update(float delta);

	protected:
		std::list<CallbackTimer *> timers;
		void *target;
		bool paused;

		friend class TimerHandler;
	};
}

#endif //__X_CALLBACKTIMERMAPNODE_H__

