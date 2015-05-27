#include "timer/UpdateTimerMapNode.h"
#include "timer/UpdateTimer.h"

namespace xGame2D
{
	UpdateTimerMapNode::UpdateTimerMapNode():
		list(nullptr),
		timer(nullptr)
	{
	}

	UpdateTimerMapNode::~UpdateTimerMapNode()
	{
	}

	bool UpdateTimerMapNode::init(std::list<UpdateTimer*> *list, UpdateTimer *timer)
	{
		this->list = list;
		this->timer = timer;
		auto added = false;
		for (auto iterator = this->list->begin(); iterator != this->list->end(); ++iterator)
		{
			if (this->timer->priority < (*iterator)->priority)
			{
				this->list->insert(iterator, this->timer);
				added = true;
				break;
			}
		}
		if (!added)
		{
			this->list->push_back(this->timer);
		}
		this->timer->retain();
		return true;
	}

	void UpdateTimerMapNode::remove()
	{
		this->list->remove(this->timer);
		timer->release();
	}
}
