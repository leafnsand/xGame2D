#ifndef __X_IMAGE_H__
#define __X_IMAGE_H__

#include "display/Quad.h"

namespace xGame2D {
    class Image : public Quad
    {
    constructor:
        Image();
		virtual ~Image();
        
	public:
		virtual bool init() override { return Quad::init(); }
		bool init(Texture *texture);
		bool init(std::string &path);
		bool init(std::string &path, bool mipmaps);
		virtual bool init(float width, float height) override;

		void setTexcoordsAtIndex(Point *coords, int32_t vertexID);
		void setTexcoordsAtIndex(float x, float y, int32_t vertexID);
		Point *texcoordsAtIndex(int32_t vertexID);
        void readjustSize();

		virtual void copyVertexDataTo(VertexData *target, int32_t index) override;
		virtual void vertexDataDidChange() override;
        
        virtual Texture *getTexture() override;
        void setTexture(Texture *value);
        
    protected:
        Texture *texture;
		VertexData *vertexDataCache;
		bool vertexDataCacheInvalid;
    };
}

#endif