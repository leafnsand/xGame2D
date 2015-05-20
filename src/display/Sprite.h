#ifndef __X_SPRITE_H__
#define __X_SPRITE_H__

#include "display/DisplayObjectContainer.h"
#include <vector>

namespace xGame2D
{
	class QuadBatch;

	class Sprite : public DisplayObjectContainer
	{
	constructor:
		Sprite();
		virtual ~Sprite();

	public:
		void flatten();
		void unflatten();
		Rectangle *clipRectInSpace(DisplayObject *targetSpace);

		virtual void render(RenderSupport *support) override;
		virtual Rectangle *boundsInSpace(DisplayObject *targetSpace) override;
		virtual DisplayObject *hitTestPoint(Point *localPoint) override;

		bool getIsFlatten();
		Rectangle *getClipRect();
		void setClipRect(Rectangle *value);

	private:
		std::vector<QuadBatch *> *flattenContents;
		bool flattenRequested;
		Rectangle *clipRect;
	};
}

#endif
