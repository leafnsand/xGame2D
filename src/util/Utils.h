#ifndef __X_UTILS_H__
#define __X_UTILS_H__

#include <cstdint>
#include <string>

namespace xGame2D
{
	class Data;

	class Utils
	{
	public:
		static int32_t nextPowerOfTwo(int32_t number);
		static bool isPowerOfTwo(int32_t number);
		static int32_t randomIntBetween(int32_t min, int32_t max);
		static float randomFloatBetween(float min, float max);
		static float randomFloat();
		static Data *readFile(std::string &path);
	};
}

#endif