#ifndef __X_RENDERSTATE_H__
#define __X_RENDERSTATE_H__

#include "base/Object.h"

namespace xGame2D
{
	class Matrix;

	class RenderState : public Object
	{
	constructor:
		RenderState();
		virtual ~RenderState();

	public:
		void setupDerived(RenderState *state, Matrix *matrix, float alpha, uint32_t blendMode);

		Matrix *modelviewMatrix;
		float alpha;
		uint32_t blendMode;
	};
}

#endif
