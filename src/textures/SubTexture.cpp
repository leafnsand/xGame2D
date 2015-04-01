#include "geom/Matrix.h"
#include "geom/Rectangle.h"
#include "textures/SubTexture.h"
#include "textures/GLTexture.h"
#include "util/VertexData.h"
#include "util/Console.h"

namespace xGame2D
{
	SubTexture::SubTexture()
		: parent(nullptr)
		, transformationMatrix(nullptr)
		, frame(nullptr)
		, width(0.0f)
		, height(0.0f)
	{
	}

	SubTexture::~SubTexture()
	{
		if (parent) parent->release();
		if (transformationMatrix) transformationMatrix->release();
		if (frame) frame->release();
	}

	bool SubTexture::init(Rectangle *region, Texture *texture)
	{
		return init(region, nullptr, texture);
	}

	bool SubTexture::init(Rectangle *region, Rectangle *frame, Texture *texture)
	{
		return init(region, frame, false, texture);
	}

	bool SubTexture::init(Rectangle *region, Rectangle *frame, bool rotated, Texture *texture)
	{
		if (!region)
		{
			region = Object::create<Rectangle>(0.0f, 0.0f, texture->getWidth(), texture->getHeight());
		}
		this->parent = texture;
		texture->retain();
		if (frame) this->frame = frame->copy();
		this->transformationMatrix = Object::generate<Matrix>();
		this->width = rotated ? region->height : region->width;
		this->height = rotated ? region->width : region->height;
		if (rotated)
		{
			transformationMatrix->translate(0.0f, -1.0f);
			transformationMatrix->rotate(PI / 2.0f);
		}
		transformationMatrix->scale(region->width / texture->getWidth(), region->height / texture->getHeight());
		transformationMatrix->translate(region->x / texture->getWidth(), region->y / texture->getHeight());
		return true;
	}

	void SubTexture::adjustVertexData(VertexData *vertexData, int32_t index, int32_t count)
	{
		auto vertices = vertexData->getVertices();
		auto stride = static_cast<int32_t>(sizeof(Vertex) - sizeof(Position));
		adjustPositions(&vertices[index].position, count, stride);
		adjustTexcoords(&vertices[index].texcoords, count, stride);
	}

	void SubTexture::adjustTexcoords(void *data, int32_t count, int32_t stride)
	{
		Texture *texture = dynamic_cast<Texture *>(this);
		auto matrix = Object::create<Matrix>();
		do 
		{
			auto subTexture = dynamic_cast<SubTexture *>(texture);
			matrix->appendMatrix(subTexture->transformationMatrix);
			texture = subTexture->parent;
		}
		while (dynamic_cast<SubTexture *>(texture));
		int32_t step = sizeof(Position) + stride;
		auto pointer = static_cast<char *>(data);
		for (auto i = 0; i < count; i++)
		{
			auto position = (Position *)pointer;
			auto x = position->x;
			auto y = position->y;
			position->x = matrix->a * x + matrix->c * y + matrix->tx;
			position->y = matrix->d * y + matrix->b * x + matrix->ty;
			pointer += step;
		}
	}

	void SubTexture::adjustPositions(void *data, int32_t count, int32_t stride)
	{
		if (frame)
		{
			if (count != 4)
			{
				Console::Error("Textures with a frame can only be used on quads");
				return;
			}
			auto deltaRight = frame->width + frame->x - width;
			auto deltaBottom = frame->height + frame->y - height;
			int32_t step = sizeof(Position) + stride;
			auto pointer = static_cast<char *>(data);
			Position *pos = nullptr;

			pos = (Position *)pointer;
			pos->x -= frame->x;
			pos->y -= frame->y;

			pos = (Position *)(pointer + step);
			pos->x -= deltaRight;
			pos->y -= frame->y;

			pos = (Position *)(pointer + 2 * step);
			pos->x -= frame->x;
			pos->y -= deltaBottom;

			pos = (Position *)(pointer + 3 * step);
			pos->x -= deltaRight;
			pos->y -= deltaBottom;
		}
	}

	float SubTexture::getWidth()
	{
		return width;
	}

	float SubTexture::getHeight()
	{
		return height;
	}

	float SubTexture::getNativeWidth()
	{
		return width * getScale();
	}

	float SubTexture::getNativeHeight()
	{
		return height * getScale();
	}

	GLTexture *SubTexture::getRoot()
	{
		return parent->getRoot();
	}

	uint32_t SubTexture::getName()
	{
		return parent->getName();
	}

	bool SubTexture::getPremultipliedAlpha()
	{
		return parent->getPremultipliedAlpha();
	}

	float SubTexture::getScale()
	{
		return parent->getScale();
	}

	TextureFormat SubTexture::getFormat()
	{
		return parent->getFormat();
	}

	bool SubTexture::getMipmaps()
	{
		return parent->getMipmaps();
	}

	Rectangle *SubTexture::getFrame()
	{
		return frame;
	}

	bool SubTexture::getRepeat()
	{
		return parent->getRepeat();
	}

	void SubTexture::setRepeat(bool value)
	{
		parent->setRepeat(value);
	}

	TextureSmoothing SubTexture::getSmoothing()
	{
		return parent->getSmoothing();
	}

	void SubTexture::setSmoothing(TextureSmoothing value)
	{
		parent->setSmoothing(value);
	}

	Texture *SubTexture::getParent()
	{
		return parent;
	}

	Rectangle *SubTexture::getRegion()
	{
		auto clipping = getClipping();
		auto root = getRoot();
		return Object::create<Rectangle>(clipping->x * root->getNativeWidth(), clipping->y * root->getNativeHeight(),
			clipping->width * root->getNativeWidth(), clipping->height * root->getNativeHeight());
	}
	Rectangle *SubTexture::getClipping()
	{
		auto topLeft = transformationMatrix->transform(0.0f, 0.0f);
		auto bottomRight = transformationMatrix->transform(1.0f, 1.0f);
		auto clipping = Object::create<Rectangle>(topLeft->x, topLeft->y, bottomRight->x - topLeft->x, bottomRight->y - topLeft->y);
		clipping->normalize();
		return clipping;
	}

	Matrix *SubTexture::getTransformationMatrix()
	{
		return transformationMatrix;
	}
}