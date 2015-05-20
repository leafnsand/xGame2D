#include "display/BlendMode.h"
#include "render/OpenGL.h"

namespace xGame2D
{
	extern const uint32_t BlendModeAuto = 0xffffffff;
	extern const uint32_t BlendModeNone = 0x00001010;
	extern const uint32_t BlendModeNormal = 0x00004515;
	extern const uint32_t BlendModeAdd = 0x00004611;
	extern const uint32_t BlendModeMultiply = 0x00008585;
	extern const uint32_t BlendModeScreen = 0x00004113;
	extern const uint32_t BlendModeErase = 0x00000505;

	static inline uint32_t encodeFactor(uint32_t factor)
	{
		if (factor == GL_ZERO || factor == GL_ONE) return factor;
		else return (factor & 0xf) + 2;
	}

	static inline uint32_t decodeFactor(uint32_t factor)
	{
		if (factor == GL_ZERO || factor == GL_ONE) return factor;
		else return factor + 0x0300 - 2;
	}

	uint32_t BlendMode::encodeBlendMode(uint32_t srcFactor, uint32_t dstFactor)
	{
		return encodeBlendMode(srcFactor, dstFactor, srcFactor, dstFactor);
	}

	uint32_t BlendMode::encodeBlendMode(uint32_t srcFactor, uint32_t dstFactor, uint32_t srcFactorPMA, uint32_t dstFactorPMA)
	{
		return ((encodeFactor(srcFactor)) << 12) |
			((encodeFactor(dstFactor)) << 8) |
			((encodeFactor(srcFactorPMA)) << 4) |
			(encodeFactor(dstFactorPMA));
	}

	void BlendMode::decodeBlendMode(uint32_t blendMode, bool pma, uint32_t *srcFactor, uint32_t *dstFactor)
	{
		if (pma)
		{
			*srcFactor = decodeFactor((blendMode & 0x00f0) >> 4);
			*dstFactor = decodeFactor(blendMode & 0x000f);
		}
		else
		{
			*srcFactor = decodeFactor((blendMode & 0xf000) >> 12);
			*dstFactor = decodeFactor((blendMode & 0x0f00) >> 8);
		}
	}

	void BlendMode::applyBlendFactorsForBlendMode(uint32_t blendMode, bool pma)
	{
		uint32_t srcFactor, dstFactor;
		if (blendMode == BlendModeAuto)
		{
			return glDisable(GL_BLEND);
		}
		decodeBlendMode(blendMode, pma, &srcFactor, &dstFactor);
		glEnable(GL_BLEND);
		glBlendFunc(srcFactor, dstFactor);
	}
}
