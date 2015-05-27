#include "display/Sprite.h"
#include "display/QuadBatch.h"
#include "display/DisplayObject.h"
#include "display/BlendMode.h"
#include "display/Stage.h"
#include "geom/Rectangle.h"
#include "geom/Matrix.h"
#include "geom/Point.h"
#include "render/RenderSupport.h"

namespace xGame2D
{
	Sprite::Sprite(): 
		flattenContents(nullptr), 
		flattenRequested(false), 
		clipRect(nullptr)
	{
	}

	Sprite::~Sprite()
	{
		if (flattenContents)
		{
			for (auto i = flattenContents->begin(); i != flattenContents->end(); ++i)
			{
				(*i)->release();
			}
			flattenContents->clear();
			flattenContents = nullptr;
		}
		if (clipRect)
		{
			clipRect->release();
		}
	}

	void Sprite::flatten()
	{
		flattenRequested = true;
	}

	void Sprite::unflatten()
	{
		flattenRequested = false;
		if (flattenContents)
		{
			for (auto i = flattenContents->begin(); i != flattenContents->end(); ++i)
			{
				(*i)->release();
			}
			flattenContents->clear();
			flattenContents = nullptr;
		}
	}

	Rectangle *Sprite::clipRectInSpace(DisplayObject *targetSpace)
	{
		if (!clipRect) return nullptr;
		float minX = FLT_MAX;
		float maxX = -FLT_MAX;
		float minY = FLT_MAX;
		float maxY = -FLT_MAX;
		auto clipLeft = clipRect->left();
		auto clipRight = clipRect->right();
		auto clipTop = clipRect->top();
		auto clipBottom = clipRect->bottom();
		auto transform = transformationMatrixToSpace(targetSpace);
		auto x = 0.0f, y = 0.0f;
		for (auto i = 0; i < 4; ++i)
		{
			switch (i)
			{
				case 0: x = clipLeft;
					y = clipTop;
					break;
				case 1: x = clipLeft;
					y = clipBottom;
					break;
				case 2: x = clipRight;
					y = clipTop;
					break;
				case 3: x = clipRight;
					y = clipBottom;
					break;
			}
			auto transformedPoint = transform->transform(x, y);
			if (minX > transformedPoint->x) minX = transformedPoint->x;
			if (maxX < transformedPoint->x) maxX = transformedPoint->x;
			if (minY > transformedPoint->y) minY = transformedPoint->y;
			if (maxY < transformedPoint->y) maxY = transformedPoint->y;
		}
		return Object::create<Rectangle>(minX, minY, maxX - minX, maxY - minY);
	}

	void Sprite::render(RenderSupport *support)
	{
		if (clipRect)
		{
			auto stageClipRect = support->pushClipRect(clipRectInSpace(dynamic_cast<DisplayObject *>(getStage())));
			if (!stageClipRect || stageClipRect->isEmpty())
			{
				support->popClipRect();
				return;
			}
		}
		if (flattenRequested)
		{
			flattenContents = QuadBatch::compileObject(this, flattenContents);
			flattenRequested = false;
		}
		if (flattenContents)
		{
			support->finishQuadBatch();
			support->addDrawCalls(static_cast<int32_t>(flattenContents->size()));
			auto mvpMatrix = support->getMvpMatrix();
			auto alpha = support->getAlpha();
			auto supportBlendMode = support->getBlendMode();
			for (auto i = flattenContents->begin(); i != flattenContents->end(); ++i)
			{
				auto blendMode = (*i)->getBlendMode();
				if (blendMode == BlendModeAuto) blendMode = supportBlendMode;
				(*i)->renderWithMvpMatrix(mvpMatrix, alpha, blendMode);
			}
		}
		else
		{
			DisplayObjectContainer::render(support);
		}
		if (clipRect)
		{
			support->popClipRect();
		}
	}

	Rectangle *Sprite::boundsInSpace(DisplayObject *targetSpace)
	{
		auto bounds = DisplayObjectContainer::boundsInSpace(targetSpace);
		if (clipRect)
		{
			bounds = bounds->intersectionWithRectangle(clipRectInSpace(targetSpace));
		}
		return bounds;
	}

	DisplayObject *Sprite::hitTestPoint(Point *localPoint)
	{
		if (clipRect && !clipRect->containsPoint(localPoint))
		{
			return nullptr;
		}
		else
		{
			return DisplayObjectContainer::hitTestPoint(localPoint);
		}
	}

	bool Sprite::getIsFlatten()
	{
		return flattenContents || flattenRequested;
	}

	Rectangle *Sprite::getClipRect()
	{
		return clipRect;
	}

	void Sprite::setClipRect(Rectangle *value)
	{
		clipRect = value->copy();
		clipRect->retain();
	}
}
