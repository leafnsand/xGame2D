#include "textures/TextureCache.h"
#include "textures/Texture.h"

namespace xGame2D
{
	TextureCache::TextureCache()
	{
	}

	TextureCache::~TextureCache()
	{
		purge();
	}

	Texture *TextureCache::textureForKey(std::string &key)
	{
		auto texture = cache[key];
		texture->retain();
		texture->autorelease();
		return texture;
	}

	void TextureCache::setTexture(Texture *texture, std::string &key)
	{
		texture->retain();
		cache[key] = texture;
	}

	void TextureCache::purge()
	{
		for (auto pairs : cache)
		{
			pairs.second->release();
		}
		cache.clear();
	}
}
