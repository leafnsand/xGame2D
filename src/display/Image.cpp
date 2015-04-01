#include "display/Image.h"
#include "textures/Texture.h"
#include "util/VertexData.h"
#include "util/Console.h"

namespace xGame2D
{
	Image::Image()
		: texture(nullptr)
		, vertexDataCache(nullptr)
		, vertexDataCacheInvalid(false)
	{
	}

	Image::~Image()
	{
		if (texture)
		{
			texture->release();
		}
		if (vertexDataCache)
		{
			vertexDataCache->release();
		}
	}

	bool Image::init(Texture *texture)
	{
		if (!texture)
		{
			Console::Error("texture cannot be null!");
		}
		auto frame = texture->getFrame();
		auto width = frame ? frame->width : texture->getWidth();
		auto height = frame ? frame->height : texture->getHeight();
		auto pma = texture->getPremultipliedAlpha();
		Quad::init(width, height, ColorWhite, pma);
		vertexData->setTexcoordsAtIndex(1.0f, 0.0f, 1);
		vertexData->setTexcoordsAtIndex(0.0f, 1.0f, 2);
		vertexData->setTexcoordsAtIndex(1.0f, 1.0f, 3);
		texture->retain();
		this->texture = texture;
		vertexDataCache = Object::generate<VertexData>(4, pma);
		vertexDataCacheInvalid = true;
		return true;
	}

	bool Image::init(std::string &path)
	{
		return init(Texture::create(path));
	}

	bool Image::init(std::string &path, bool mipmaps)
	{
		return init(Texture::create(path, mipmaps));
	}

	bool Image::init(float width, float height)
	{
		return init(Texture::create(width, height));
	}

	void Image::setTexcoordsAtIndex(Point *coords, int32_t vertexID)
	{
		vertexData->setTexcoordsAtIndex(coords, vertexID);
		vertexDataDidChange();
	}

	void Image::setTexcoordsAtIndex(float x, float y, int32_t vertexID)
	{
		vertexData->setTexcoordsAtIndex(x, y, vertexID);
		vertexDataDidChange();
	}

	Point *Image::texcoordsAtIndex(int32_t vertexID)
	{
		return vertexData->texcoordsAtIndex(vertexID);
	}

	void Image::readjustSize()
	{
		auto frame = texture->getFrame();
		auto width = frame ? frame->width : texture->getWidth();
		auto height = frame ? frame->height : texture->getHeight();
		vertexData->setTexcoordsAtIndex(width, 0.0f, 1);
		vertexData->setTexcoordsAtIndex(0.0f, height, 2);
		vertexData->setTexcoordsAtIndex(width, height, 3);
		vertexDataDidChange();
	}

	void Image::copyVertexDataTo(VertexData *target, int32_t index)
	{
		if (vertexDataCacheInvalid)
		{
			vertexDataCacheInvalid = false;
			vertexData->copyTo(vertexDataCache);
			texture->adjustVertexData(vertexDataCache, 0, 4);
		}
		vertexDataCache->copyTo(target, index, 4);
	}

	void Image::vertexDataDidChange()
	{
		vertexDataCacheInvalid = true;
	}

	Texture *Image::getTexture()
	{
		return texture;
	}

	void Image::setTexture(Texture *value)
	{
		if (value == nullptr)
		{
			Console::Error("texture cannot be null!");
		}
		else if (value != texture)
		{
			texture->release();
			texture = value;
			texture->retain();
			vertexData->setPremultipliedAlpha(texture->getPremultipliedAlpha(), true);
			vertexDataCache->setPremultipliedAlpha(texture->getPremultipliedAlpha(), false);
			vertexDataDidChange();
		}
	}
}