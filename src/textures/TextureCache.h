#ifndef __X_TEXTURECACHE_H__
#define __X_TEXTURECACHE_H__

#include "base/Object.h"
#include <string>
#include <map>

namespace xGame2D
{
	class Texture;

	class TextureCache : public Object
	{
	constructor:
		TextureCache();
		~TextureCache();

	public:
		Texture *textureForKey(std::string &key);
		void setTexture(Texture *texture, std::string &key);
		void purge();

	private:
		std::map<std::string, Texture *> cache;
	};
}

#endif