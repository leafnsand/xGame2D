#ifndef __X_DATA_H__
#define __X_DATA_H__

#include "base/Object.h"
#include "base/Copying.h"
#include <cstdlib>

namespace xGame2D
{
	class Data : public Object, Copying
	{
	constructor:
		Data() : rawData(nullptr), size(0) {}
		virtual ~Data() { if (rawData) { free(rawData); } }

	public:
		OVERRIDE_DEFAULT_INIT

		inline bool init(void *buffer, size_t s) { rawData = buffer; size = s; return true; }

		virtual inline Data *copy() override { return Object::generate<Data>(this->rawData, this->size); }

		inline void *getBuffer() { return rawData; }

		inline size_t getSize() { return size; }

	private:
		void *rawData;
		size_t size;
	};
}

#endif