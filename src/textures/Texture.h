#ifndef __X_TEXTURE_H__
#define __X_TEXTURE_H__

#include "base/Object.h"
#include "render/OpenGL.h"
#include <string>

namespace xGame2D
{
	typedef enum
	{
		TextureFormatRGBA,
		TextureFormatAlpha,
#if COMPRESSED_TEXTURE
		TextureFormatPvrtcRGB2,
		TextureFormatPvrtcRGBA2,
		TextureFormatPvrtcRGB4,
		TextureFormatPvrtcRGBA4,
#endif
		TextureFormat565,
		TextureFormat888,
		TextureFormat5551,
		TextureFormat4444,
		TextureFormatAI88,
		TextureFormatI8
	} TextureFormat;

	typedef enum
	{
		TextureSmoothingNone,
		TextureSmoothingBilinear,
		TextureSmoothingTrilinear
	} TextureSmoothing;

	class Data;
	class Rectangle;
	class VertexData;
	class GLTexture;

	class Texture : public Object
	{
	public:
		static Texture *create(float width, float height);
		static Texture *create(float width, float height, Data *data, TextureFormat format);
		static Texture *create(float width, float height, Data *data, bool mipmaps, TextureFormat format);
		static Texture *create(float width, float height, Data *data, bool mipmaps, float scale, TextureFormat format);
		static Texture *create(std::string &path);
		static Texture *create(std::string &path, bool mipmaps);
		static Texture *create(Rectangle *region, Texture *texture);
		static Texture *create(Rectangle *region, Rectangle *frame, Texture *texture);

		virtual void adjustVertexData(VertexData *vertexData, int32_t index, int32_t count) = 0;
		virtual void adjustTexcoords(void *data, int32_t count, int32_t stride) = 0;
		virtual void adjustPositions(void *data, int32_t count, int32_t stride) = 0;

		virtual float getWidth() = 0;
		virtual float getHeight() = 0;
		virtual float getNativeWidth() = 0;
		virtual float getNativeHeight() = 0;
		virtual GLTexture *getRoot() = 0;
		virtual uint32_t getName() = 0;
		virtual bool getPremultipliedAlpha() = 0;
		virtual float getScale() = 0;
		virtual TextureFormat getFormat() = 0;
		virtual bool getMipmaps() = 0;
		virtual Rectangle *getFrame() = 0;
		virtual bool getRepeat() = 0;
		virtual void setRepeat(bool value) = 0;
		virtual TextureSmoothing getSmoothing() = 0;
		virtual void setSmoothing(TextureSmoothing value) = 0;
	};
}

#endif
