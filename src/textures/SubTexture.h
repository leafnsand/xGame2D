#ifndef __X_SUBTEXTURE_H__
#define	__X_SUBTEXTURE_H__

#include "textures/Texture.h"

namespace xGame2D
{
	class Matrix;

	class SubTexture : public Texture
	{
	constructor:
		SubTexture();
		virtual ~SubTexture();

	public:
		OVERRIDE_DEFAULT_INIT
		bool init(Rectangle *region, Texture *texture);
		bool init(Rectangle *region, Rectangle *frame, Texture *texture);
		bool init(Rectangle *region, Rectangle *frame, bool rotated, Texture *texture);

		virtual void adjustVertexData(VertexData *vertexData, int32_t index, int32_t count) override;
		virtual void adjustTexcoords(void *data, int32_t count, int32_t stride) override;
		virtual void adjustPositions(void *data, int32_t count, int32_t stride) override;

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
		virtual Rectangle *getFrame() override;
		virtual bool getRepeat() override;
		virtual void setRepeat(bool value) override;
		virtual TextureSmoothing getSmoothing() override;
		virtual void setSmoothing(TextureSmoothing value) override;

		Texture *getParent();
		Rectangle *getRegion();
		Rectangle *getClipping();
		Matrix *getTransformationMatrix();

	protected:
		Texture *parent;
		Matrix *transformationMatrix;
		Rectangle *frame;
		float width, height;
	};
}

#endif