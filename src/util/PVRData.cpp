#include "base/Data.h"
#include "util/PVRData.h"
#include "util/Console.h"
#include <cstdlib>

#define PVRTEX_IDENTIFIER 0x21525650 // = the characters 'P', 'V', 'R'

namespace xGame2D
{
	typedef struct
	{
		uint32_t headerSize; // size of the structure
		uint32_t height; // height of surface to be created
		uint32_t width; // width of input surface
		uint32_t numMipmaps; // number of mip-map levels requested
		uint32_t pfFlags; // pixel format flags
		uint32_t textureDataSize; // total size in bytes
		uint32_t bitCount; // number of bits per pixel
		uint32_t rBitMask; // mask for red bit
		uint32_t gBitMask; // mask for green bits
		uint32_t bBitMask; // mask for blue bits
		uint32_t alphaBitMask; // mask for alpha channel
		uint32_t pvr; // magic number identifying pvr file
		uint32_t numSurfs; // number of surfaces present in the pvr
	} PVRTextureHeader;

	enum PVRPixelType
	{
		OGL_RGBA_4444 = 0x10,
		OGL_RGBA_5551,
		OGL_RGBA_8888,
		OGL_RGB_565,
		OGL_RGB_555,
		OGL_RGB_888,
		OGL_I_8,
		OGL_AI_88,
		OGL_PVRTC2,
		OGL_PVRTC4,
		OGL_BGRA_8888,
		OGL_A_8
	};

	PVRData::PVRData():
		rawData(nullptr)
	{
	}

	PVRData::~PVRData()
	{
		if (rawData)
		{
			rawData->release();
		}
	}

	bool PVRData::init(Data *data)
	{
		return init(data, false);
	}

	bool PVRData::init(Data *data, bool compressed)
	{
		rawData = data;
		rawData->retain();
		auto header = static_cast<PVRTextureHeader *>(rawData->getBuffer());
		width = header->width;
		height = header->height;
		numMipmaps = header->numMipmaps;
		switch (header->pfFlags & 0xff)
		{
			case OGL_RGB_565: format = TextureFormat565;
				break;
			case OGL_RGB_888: format = TextureFormat888;
				break;
			case OGL_RGBA_5551: format = TextureFormat5551;
				break;
			case OGL_RGBA_4444: format = TextureFormat4444;
				break;
			case OGL_RGBA_8888: format = TextureFormatRGBA;
				break;
			case OGL_A_8: format = TextureFormatAlpha;
				break;
			case OGL_I_8: format = TextureFormatI8;
				break;
			case OGL_AI_88: format = TextureFormatAI88;
				break;
#if COMPRESSED_TEXTURE
			case OGL_PVRTC2:
				format = header->alphaBitMask ? TextureFormatPvrtcRGBA2 : TextureFormatPvrtcRGB2;
				break;
			case OGL_PVRTC4:
				format = header->alphaBitMask ? TextureFormatPvrtcRGBA4 : TextureFormatPvrtcRGB4;
				break;
#endif
			default:
				Console::Error("Unsupported PVR image format");
		}
		return true;
	}

	void *PVRData::getImageData()
	{
		PVRTextureHeader *header = (PVRTextureHeader *)rawData->getBuffer();
		return (uint8_t *)header + header->headerSize;
	}
}
