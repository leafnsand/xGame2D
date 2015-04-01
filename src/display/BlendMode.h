#ifndef __X_BLENDMODE_H__
#define __X_BLENDMODE_H__

#include <cstdint>
#include <string>

namespace xGame2D
{
	extern const uint32_t BlendModeAuto;
	extern const uint32_t BlendModeNone;
	extern const uint32_t BlendModeNormal;
	extern const uint32_t BlendModeAdd;
	extern const uint32_t BlendModeMultiply;
	extern const uint32_t BlendModeScreen;
	extern const uint32_t BlendModeErase;

    class BlendMode
    {
    public:
        static uint32_t encodeBlendMode(uint32_t srcFactor, uint32_t dstFactor);
        static uint32_t encodeBlendMode(uint32_t srcFactor, uint32_t dstFactor, uint32_t srcFactorPMA, uint32_t dstFactorPMA);
        static void decodeBlendMode(uint32_t blendMode, bool pma, uint32_t *srcFactor, uint32_t *dstFactor);
        static void applyBlendFactorsForBlendMode(uint32_t blendMode, bool pma);
    };
}

#endif