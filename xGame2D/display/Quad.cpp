#include "display/Quad.h"
#include "display/DisplayObjectContainer.h"
#include "render/RenderSupport.h"
#include "util/VertexData.h"
#include "util/Console.h"

#define MIN_SIZE 0.01f

namespace xGame2D
{
	Quad::Quad()
	{
	}

	Quad::~Quad()
	{
		vertexData->release();
	}

	bool Quad::init()
	{
		return init(32, 32);
	}

	bool Quad::init(float width, float height)
	{
		return init(width, height, ColorWhite);
	}

	bool Quad::init(float width, float height, uint32_t color)
	{
		return init(width, height, color, true);
	}

	bool Quad::init(float width, float height, uint32_t color, bool pma)
	{
		if (width <= MIN_SIZE) width = MIN_SIZE;
		if (height <= MIN_SIZE) height = MIN_SIZE;
		tinted = color != 0xffffff;
		vertexData = Object::generate<VertexData>(4, pma);
		vertexData->setPositionAtIndex(width, 0.0f, 1);
		vertexData->setPositionAtIndex(0.0f, height, 2);
		vertexData->setPositionAtIndex(width, height, 3);
		for (auto i = 0; i < 4; ++i)
			vertexData->setColorAtIndex(color, 1.0f, i);
		vertexDataDidChange();
		return true;
	}

	void Quad::setColorOfVertex(uint32_t color, int32_t vertexID)
	{
		vertexData->setColorAtIndex(color, vertexID);
		vertexDataDidChange();
		if (color != 0xffffff) tinted = true;
		else tinted = (alpha != 1.0f) || vertexData->getTinted();
	}

	uint32_t Quad::colorOfVertex(uint32_t vertexID)
	{
		return vertexData->colorAtIndex(vertexID);
	}

	void Quad::setAlphaOfVertex(float alpha, int32_t vertexID)
	{
		vertexData->setAlphaAtIndex(alpha, vertexID);
		vertexDataDidChange();
		if (alpha != 1.0) tinted = true;
		else tinted = (alpha != 1.0f) || vertexData->getTinted();
	}

	float Quad::alphaOfVertex(int32_t vertexID)
	{
		return vertexData->alphaAtIndex(vertexID);
	}

	void Quad::copyVertexDataTo(VertexData *target, int32_t index)
	{
		vertexData->copyTo(target, index);
	}

	void Quad::vertexDataDidChange()
	{
	}

	void Quad::render(RenderSupport *support)
	{
		support->batchQuad(this);
	}

	Rectangle *Quad::boundsInSpace(DisplayObject *targetSpace)
	{
		if (targetSpace == this)
		{
			auto bottomRight = vertexData->positionAtIndex(3);
			return Object::create<Rectangle>(0.0f, 0.0f, bottomRight->x, bottomRight->y);
		}
		else if (targetSpace == parent && rotation == 0.0f)
		{
			auto bottomRight = vertexData->positionAtIndex(3);
			auto resultRect = Object::create<Rectangle>(x - pivotX * scaleX, y - pivotY * scaleY, bottomRight->x * scaleX, bottomRight->y * scaleY);
			if (scaleX < 0.0f)
			{
				resultRect->width *= -1.0f;
				resultRect->x -= resultRect->width;
			}
			if (scaleY < 0.0f)
			{
				resultRect->height *= -1.0f;
				resultRect->y -= resultRect->height;
			}
			return resultRect;
		}
		else
		{
			auto transformationMatrix = transformationMatrixToSpace(targetSpace);
			return vertexData->boundsAfterTransformationOfVertices(transformationMatrix, 0, 4);
		}
	}

	void Quad::setAlpha(float value)
	{
		alpha = CLAMP(value, 0.0f, 1.0f);
		if (alpha != 1.0) tinted = true;
		else tinted = (alpha != 1.0f) || vertexData->getTinted();
	}

	uint32_t Quad::getColor()
	{
		return colorOfVertex(0);
	}

	void Quad::setColor(uint32_t color)
	{
		for (auto i = 0; i < 4; ++i)
			vertexData->setColorAtIndex(color, i);
		vertexDataDidChange();
		if (color != 0xffffff) tinted = true;
		else tinted = (getAlpha() != 1.0f) || vertexData->getTinted();
	}

	bool Quad::getPremultipliedAlpha()
	{
		return vertexData->getPremultipliedAlpha();
	}

	void Quad::setPremultipliedAlpha(bool pma)
	{
		return vertexData->setPremultipliedAlpha(pma);
	}

	bool Quad::getTinted()
	{
		return tinted;
	}

	Texture *Quad::getTexture()
	{
		return nullptr;
	}
}
