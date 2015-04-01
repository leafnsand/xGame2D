#ifndef __X_PVRDATA_H__
#define __X_PVRDATA_H__

#include "base/Object.h"
#include "textures/Texture.h"

namespace xGame2D
{
	class Data;

	class PVRData : public Object
	{
	constructor:
		PVRData();
		virtual ~PVRData();

	public:
		OVERRIDE_DEFAULT_INIT
		bool init(Data *data);
		bool init(Data *data, bool compressed);

		void *getImageData();

	public:
		int32_t width, height, numMipmaps;
		TextureFormat format;

	protected:
		Data *rawData;
	};
}

#endif