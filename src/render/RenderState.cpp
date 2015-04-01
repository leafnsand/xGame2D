#include "render/RenderState.h"
#include "display/BlendMode.h"
#include "geom/Matrix.h"

namespace xGame2D
{
	RenderState::RenderState()
		: modelviewMatrix(Object::generate<Matrix>())
		, alpha(1.0f)
		, blendMode(BlendModeNormal)
	{
	}

	RenderState::~RenderState()
	{
		modelviewMatrix->release();
	}

	void RenderState::setupDerived(RenderState *state, Matrix *matrix, float alpha, uint32_t blendMode)
	{
		this->alpha = alpha * state->alpha;
		this->blendMode = blendMode == BlendModeAuto ? state->blendMode : blendMode;
		modelviewMatrix->clone(state->modelviewMatrix);
		modelviewMatrix->prependMatrix(matrix);
	}
}