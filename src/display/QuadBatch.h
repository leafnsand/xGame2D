#ifndef __X_QUADBATCH_H__
#define __X_QUADBATCH_H__

#include "display/DisplayObject.h"
#include <vector>

namespace xGame2D {
    class Quad;
	class Texture;
	class BaseEffect;
	class VertexData;
    
    class QuadBatch : public DisplayObject
    {
    constructor:
        QuadBatch();
        ~QuadBatch();

	public:
		virtual bool init() override;
		bool init(int32_t capacity);
        
        void reset();
        void addQuad(Quad *quad);
        void addQuad(Quad *quad, float alpha);
        void addQuad(Quad *quad, float alpha, uint32_t blendMode);
        void addQuad(Quad *quad, float alpha, uint32_t blendMode, Matrix *matrix);
        void addQuadBatch(QuadBatch *quadBatch);
        void addQuadBatch(QuadBatch *quadBatch, float alpha);
        void addQuadBatch(QuadBatch *quadBatch, float alpha, uint32_t blendMode);
        void addQuadBatch(QuadBatch *quadBatch, float alpha, uint32_t blendMode, Matrix *matrix);
        bool isStateChangeWithTinted(bool tinted, Texture *texture, float alpha, bool pma, uint32_t blendMode, int32_t numQuads);
        void renderWithMvpMatrix(Matrix *matrix);
        void renderWithMvpMatrix(Matrix *matrix, float alpha, uint32_t blendMode);
        
        static std::vector<QuadBatch *> *compileObject(DisplayObject *object);
        static std::vector<QuadBatch *> *compileObject(DisplayObject *object, std::vector<QuadBatch *> *quadBatches);
        
        virtual void render(RenderSupport *support) override;
        virtual Rectangle *boundsInSpace(DisplayObject *targetSpace) override;
        
        int32_t getNumQuads();
        bool getTinted();
        Texture *getTexture();
        bool getPremultipliedAlpha();
        int32_t getCapacity();
        void setCapacity(int32_t value);

	private:
		void expand();
		void createBuffers();
        void destroyBuffers();
		void syncBuffers();
        
        static int32_t compile(DisplayObject * object, std::vector<QuadBatch *> *quadBatches, int32_t quadBatchID, Matrix *transformationMatrix, float alpha, uint32_t blendMode);
        
	protected:
		int32_t numQuads, capacity;
		bool syncRequired, premultipliedAlpha, tinted;
		Texture *texture;
		BaseEffect *baseEffect;
		VertexData *vertexData;
		uint32_t vertexBufferName, indexBufferName;
		unsigned short *indexData;
    };
}

#endif