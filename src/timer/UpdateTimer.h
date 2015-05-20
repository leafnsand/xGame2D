#ifndef __X_UPDATETIMER_H__
#define __X_UPDATETIMER_H__

#include "timer/Timer.h"
#include <functional>

namespace xGame2D
{
	class UpdateTimer : public Timer
	{
	constructor:
		UpdateTimer();
		virtual ~UpdateTimer();

	public:
		bool init(void *target, const std::function<void(float)> &updateFunction, int32_t priority, bool paused);
		virtual void trigger() override;
		virtual void cancel() override;

	protected:
		void *target;
		std::function<void(float)> updateFunction;
		int32_t priority;
		bool paused;

		friend class TimerHandler;
		friend class UpdateTimerMapNode;
	};
}

#endif //__X_UPDATELISTNODE_H__

