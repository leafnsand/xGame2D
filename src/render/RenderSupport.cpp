#include "render/RenderSupport.h"
#include "render/RenderState.h"
#include "render/Context.h"
#include "geom/Matrix.h"
#include "geom/Rectangle.h"
#include "display/BlendMode.h"
#include "display/Quad.h"
#include "display/QuadBatch.h"
#include "textures/Texture.h"
#include "util/Console.h"
#include "Game.h"

namespace xGame2D
{
	RenderSupport::RenderSupport():
		projectionMatrix(Object::generate<Matrix>()), mvpMatrix(Object::generate<Matrix>()),
		numDrawCalls(0),
		stateStackIndex(0),
		stateStackSize(1),
		quadBatchIndex(0),
		quadBatchSize(1),
		clipRectStackSize(0),
		stateStackTop(Object::generate<RenderState>()),
		quadBatchTop(Object::generate<QuadBatch>())
	{
	}

	RenderSupport::~RenderSupport()
	{
		projectionMatrix->release();
		mvpMatrix->release();
		// TODO
	}

	bool RenderSupport::init()
	{
		stateStack.push_back(stateStackTop);
		quadBatches.push_back(quadBatchTop);
		setupOrthographicProjection(0, 320, 0, 480);
		return true;
	}

	void RenderSupport::prepare()
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
	}

	void RenderSupport::nextFrame()
	{
		trimQuadBatches();
		clipRectStackSize = 0;
		stateStackIndex = 0;
		quadBatchIndex = 0;
		numDrawCalls = 0;
		quadBatchTop = quadBatches[0];
		stateStackTop = stateStack[0];
	}

	void RenderSupport::batchQuad(Quad *quad)
	{
		auto alpha = stateStackTop->alpha;
		auto blendMode = stateStackTop->blendMode;
		auto modelviewMatrix = stateStackTop->modelviewMatrix;
		if (quadBatchTop->isStateChangeWithTinted(quad->getTinted(), quad->getTexture(), quad->getAlpha(), quad->getPremultipliedAlpha(), quad->getBlendMode(), 1))
		{
			finishQuadBatch();
		}
		quadBatchTop->addQuad(quad, alpha, blendMode, modelviewMatrix);
	}

	void RenderSupport::finishQuadBatch()
	{
		if (quadBatchTop->getNumQuads())
		{
			quadBatchTop->renderWithMvpMatrix(projectionMatrix);
			quadBatchTop->reset();
			if (quadBatchSize == quadBatchIndex + 1)
			{
				quadBatches.push_back(Object::generate<QuadBatch>());
				++quadBatchSize;
			}
			++numDrawCalls;
			quadBatchTop = quadBatches[++quadBatchIndex];
		}
	}

	void RenderSupport::purgeBuffers()
	{
		for (auto quad : quadBatches)
		{
			quad->release();
		}
		quadBatches.clear();
		quadBatchTop = Object::generate<QuadBatch>();
		quadBatches.push_back(quadBatchTop);
		quadBatchSize = 1;
		quadBatchTop = 0;
	}

	void RenderSupport::clear()
	{
		clear(0, 0.0f);
	}

	void RenderSupport::clear(uint32_t color)
	{
		clear(color, 1.0f);
	}

	void RenderSupport::clear(uint32_t color, float alpha)
	{
		auto red = ColorGetRed(color) / 255.0f;
		auto green = ColorGetGreen(color) / 255.0f;
		auto blue = ColorGetBlue(color) / 255.0f;
		glClearColor(red, green, blue, alpha);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	uint32_t RenderSupport::checkForOpenGLError()
	{
		GLenum error;
		while ((error = glGetError()))
		{
			std::string string = "UNKNOWN_ERROR";
			switch (error)
			{
				case GL_NO_ERROR: string = "GL_NO_ERROR";
				case GL_INVALID_ENUM: string = "GL_INVALID_ENUM";
				case GL_INVALID_OPERATION: string = "GL_INVALID_OPERATION";
				case GL_INVALID_VALUE: string = "GL_INVALID_VALUE";
				case GL_INVALID_FRAMEBUFFER_OPERATION: string = "GL_INVALID_FRAMEBUFFER_OPERATION";
				case GL_OUT_OF_MEMORY: string = "GL_OUT_OF_MEMORY";
			}
			Console::Log("There was an OpenGL error: %s", string.c_str());
		}
		return error;
	}

	void RenderSupport::addDrawCalls(int32_t count)
	{
		numDrawCalls += count;
	}

	void RenderSupport::setupOrthographicProjection(float left, float right, float top, float bottom)
	{
		projectionMatrix->set(2.0f / (right - left), 0.0f, 0.0f, 2.0f / (top - bottom), -(right + left) / (right - left), -(top + bottom) / (top - bottom));
		applyClipRect();
	}

	void RenderSupport::pushState(Matrix *matrix, float alpha, uint32_t blendMode)
	{
		auto previousState = stateStackTop;
		if (stateStackSize == stateStackIndex + 1)
		{
			stateStack.push_back(Object::generate<RenderState>());
			++stateStackSize;
		}
		stateStackTop = stateStack[++stateStackIndex];
		stateStackTop->setupDerived(previousState, matrix, alpha, blendMode);
	}

	void RenderSupport::popState()
	{
		X_ASSERT(stateStackIndex != 0, "The state stack must not be empty");
		stateStackTop = stateStack[--stateStackIndex];
	}

	void RenderSupport::applyBlendModeForPremultipliedAlpha(bool pma)
	{
		BlendMode::applyBlendFactorsForBlendMode(stateStackTop->blendMode, pma);
	}

	Rectangle *RenderSupport::pushClipRect(Rectangle *clipRect)
	{
		if (static_cast<int32_t>(clipRectStack.size()) < clipRectStackSize + 1)
		{
			clipRectStack.push_back(Object::generate<Rectangle>());
		}
		auto rectangle = clipRectStack[clipRectStackSize];
		if (clipRectStackSize > 0)
		{
			rectangle = rectangle->intersectionWithRectangle(clipRectStack[clipRectStackSize - 1]);
		}
		++clipRectStackSize;
		applyClipRect();
		return rectangle;
	}

	void RenderSupport::popClipRect()
	{
		if (clipRectStackSize > 0)
		{
			--clipRectStackSize;
			applyClipRect();
		}
	}

	void RenderSupport::applyClipRect()
	{
		finishQuadBatch();
		if (!Game::getInstance()) return;
		auto context = Game::getInstance()->context;
		if (!context) return;
		if (clipRectStackSize > 0)
		{
			// TODO
		}
		else
		{
			context->setScissorBox(nullptr);
		}
	}

	Matrix *RenderSupport::getProjectionMatrix()
	{
		return projectionMatrix;
	}

	void RenderSupport::setProjectionMatrix(Matrix *value)
	{
		projectionMatrix->clone(value);
		applyClipRect();
	}

	Matrix *RenderSupport::getMvpMatrix()
	{
		mvpMatrix->clone(stateStackTop->modelviewMatrix);
		mvpMatrix->appendMatrix(projectionMatrix);
		return mvpMatrix;
	}

	Matrix *RenderSupport::getModelviewMatrix()
	{
		return stateStackTop->modelviewMatrix;
	}

	float RenderSupport::getAlpha()
	{
		return stateStackTop->alpha;
	}

	void RenderSupport::setAlpha(float value)
	{
		stateStackTop->alpha = value;
	}

	uint32_t RenderSupport::getBlendMode()
	{
		return stateStackTop->blendMode;
	}

	void RenderSupport::setBlendMode(uint32_t blendMode)
	{
		if (blendMode != BlendModeAuto)
		{
			stateStackTop->blendMode = blendMode;
		}
	}

	Texture *RenderSupport::getRenderTarget()
	{
		return Game::getInstance()->context->getRenderTarget();
	}

	void RenderSupport::setRenderTarget(Texture *value)
	{
		applyClipRect();
		Game::getInstance()->context->setRenderTarget(value);
	}

	int32_t RenderSupport::getNumDrawCalls()
	{
		return numDrawCalls;
	}

	void RenderSupport::trimQuadBatches()
	{
		auto numUsedBatches = quadBatchIndex + 1;
		if (quadBatchSize >= 16 && quadBatchSize > 2 * numUsedBatches)
		{
			for (auto i = quadBatchSize - 1; i > quadBatchIndex; ++i)
			{
				auto batch = quadBatches[i];
				batch->release();
				quadBatches.pop_back();
			}
			quadBatchSize = static_cast<int32_t>(quadBatches.size());
		}
	}
}
