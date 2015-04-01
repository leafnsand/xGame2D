#ifndef __X_STAGE_H__
#define __X_STAGE_H__

#include "display/DisplayObjectContainer.h"

namespace xGame2D
{
	class Stage : public DisplayObjectContainer
	{
	constructor:
		Stage();
		~Stage();

	public:
		OVERRIDE_DEFAULT_INIT
		bool init(float width, float height, uint32_t color);

		virtual void render(RenderSupport *support) override;
		virtual DisplayObject *hitTestPoint(Point *localPoint) override;

		float width, height;
		uint32_t color;
	};
}

#endif