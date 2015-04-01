#ifndef __X_GLTEXTURE_H__
#define __X_GLTEXTURE_H__

#include "textures/Texture.h"

namespace xGame2D
{
	class Rectangle;

	class PVRData;

	typedef struct
	{
		TextureFormat format;
		float scale;
		int32_t width;
		int32_t height;
		int32_t numMipmaps;
		bool generateMipmaps;
		bool premultipliedAlpha;
	} TextureProperties;

	class GLTexture : public Texture
	{
	constructor:
		GLTexture();
		~GLTexture();

	public:
		virtual bool init() override;
		bool init(uint32_t name, TextureFormat format, float width, float height, bool mipmaps, float scale, bool pma);
		bool init(const void *imageData, TextureProperties properties);
		bool init(PVRData *pvrData, float scale);

		virtual void adjustVertexData(VertexData *vertexData, int32_t index, int32_t count) override{};
		virtual void adjustTexcoords(void *data, int32_t count, int32_t stride) override{};
		virtual void adjustPositions(void *data, int32_t count, int32_t stride) override{};

		virtual float getWidth() override;
		virtual float getHeight() override;
		virtual float getNativeWidth() override;
		virtual float getNativeHeight() override;
		virtual GLTexture *getRoot() override;
		virtual uint32_t getName() override;
		virtual bool getPremultipliedAlpha() override;
		virtual float getScale() override;
		virtual TextureFormat getFormat() override;
		virtual bool getMipmaps() override;
		virtual Rectangle *getFrame() override{return nullptr;};
		virtual bool getRepeat() override;
		virtual void setRepeat(bool value) override;
		virtual TextureSmoothing getSmoothing() override;
		virtual void setSmoothing(TextureSmoothing value) override;

	protected:
		TextureFormat format;
		TextureSmoothing smoothing;
		uint32_t name;
		float width, height, scale;
		bool repeat, premultipliedAlpha, mipmaps;
	};
}

#endif