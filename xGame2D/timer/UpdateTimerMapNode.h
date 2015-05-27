#ifndef __X_UPDATETIMERMAPNODE_H__
#define __X_UPDATETIMERMAPNODE_H__

#include "base/Object.h"
#include <list>

namespace xGame2D
{
	class UpdateTimer;

	class UpdateTimerMapNode : public Object
	{
	constructor:
		UpdateTimerMapNode();
		virtual ~UpdateTimerMapNode();

	public:
		bool init(std::list<UpdateTimer *> *list, UpdateTimer *timer);
		void remove();

	protected:
		std::list<UpdateTimer *> *list;
		UpdateTimer *timer;

		friend class TimerHandler;
	};
}

#endif //__X_UPDATETIMERMAPNODE_H__

