#ifndef __X_RENDERSUPPORT_H__
#define __X_RENDERSUPPORT_H__

#include "base/Object.h"
#include <vector>

namespace xGame2D
{
    class Quad;
    class Matrix;
    class Rectangle;
    class Texture;
	class RenderState;
	class QuadBatch;
    
    class RenderSupport : public Object 
    {
    constructor:
        RenderSupport();
		virtual ~RenderSupport();

	public:
		virtual bool init() override;

		void prepare();
        void nextFrame();
        void batchQuad(Quad *quad);
        void finishQuadBatch();
        void purgeBuffers();
        void clear();
        static void clear(uint32_t color);
        static void clear(uint32_t color, float alpha);
        static uint32_t checkForOpenGLError();
        void addDrawCalls(int32_t count);
        void setupOrthographicProjection(float left, float right, float top, float bottom);
        void pushState(Matrix *matrix, float alpha, uint32_t blendMode);
        void popState();
        void applyBlendModeForPremultipliedAlpha(bool pma);
        Rectangle *pushClipRect(Rectangle *clipRect);
        void popClipRect();
        void applyClipRect();
        
        Matrix *getProjectionMatrix();
        void setProjectionMatrix(Matrix *value);
        Matrix *getMvpMatrix();
        Matrix *getModelviewMatrix();
        float getAlpha();
        void setAlpha(float value);
        uint32_t getBlendMode();
        void setBlendMode(uint32_t blendMode);
        Texture *getRenderTarget();
        void setRenderTarget(Texture *value);
        int32_t getNumDrawCalls();

	private:
		void trimQuadBatches();

	protected:
		Matrix *projectionMatrix, *mvpMatrix;
		int32_t numDrawCalls, stateStackIndex, stateStackSize, quadBatchIndex, quadBatchSize, clipRectStackSize;
		RenderState *stateStackTop;
		QuadBatch *quadBatchTop;
		std::vector<RenderState *> stateStack;
		std::vector<QuadBatch *> quadBatches;
		std::vector<Rectangle *> clipRectStack;
    };
}

#endif