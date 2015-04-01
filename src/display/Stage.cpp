#include "display/Stage.h"
#include "render/RenderSupport.h"

namespace xGame2D
{
	Stage::Stage()
	{
	}

	Stage::~Stage()
	{
	}

	bool Stage::init(float width, float height, uint32_t color)
	{
		this->width = width;
		this->height = height;
		this->color = color;
		return true;
	}

	void Stage::render(RenderSupport *support)
	{
		RenderSupport::clear(color, 1.0f);
		support->setupOrthographicProjection(0, width, 0, height);
		DisplayObjectContainer::render(support);
	}

	DisplayObject *Stage::hitTestPoint(Point *localPoint)
	{
		if (!getVisible() || !getTouchable())
		{
			return nullptr;
		}
		auto target = DisplayObjectContainer::hitTestPoint(localPoint);
		if (!target) target = this;
		return target;
	}
}