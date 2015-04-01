#ifndef __X_QUAD_H__
#define __X_QUAD_H__

#include "display/DisplayObject.h"

namespace xGame2D
{
	class VertexData;
	class Texture;
	class Quad : public DisplayObject
	{
	constructor:
		Quad();
		~Quad();

	public:
		virtual bool init() override;
		virtual bool init(float width, float height);
		bool init(float width, float height, uint32_t color);
		bool init(float width, float height, uint32_t color, bool pma);

		void setColorOfVertex(uint32_t color, int32_t vertexID);
		uint32_t colorOfVertex(uint32_t vertexID);
		void setAlphaOfVertex(float alpha, int32_t vertexID);
		float alphaOfVertex(int32_t vertexID);
		virtual void copyVertexDataTo(VertexData *target, int32_t index);
		virtual void vertexDataDidChange();

        virtual void render(RenderSupport *support) override;
		virtual Rectangle *boundsInSpace(DisplayObject *targetSpace) override;
		virtual void setAlpha(float value) override;

		uint32_t getColor();
		void setColor(uint32_t color);
		bool getPremultipliedAlpha();
		void setPremultipliedAlpha(bool pma);
		bool getTinted();
		virtual Texture *getTexture();

	protected:
		bool tinted;
		VertexData *vertexData;
	};
}

#endif