#ifndef __X_MATH_H__
#define __X_MATH_H__

#include <cstdint>
#include <cfloat>
#include <cmath>
#include <algorithm>

#define PI                          3.14159265359f
#define PI_HALF                     1.57079632679f
#define TWO_PI                      6.28318530718f
#define FLOAT_EPSILON               0.0001f

#define MAX(x, y)                   std::max(x, y)
#define MIN(x, y)                   std::min(x, y)
#define CLAMP(value, min, max)      MIN((max), MAX((value), (min)))
#define SWAP(x, y, T)               do { T temp##x##y = x; x = y; y = temp##x##y; } while (0)
#define SQUARE(x)                   ((x) * (x))

namespace xGame2D
{
	static const uint32_t ColorWhite = 0xffffff;
	static const uint32_t ColorSilver = 0xc0c0c0;
	static const uint32_t ColorGray = 0x808080;
	static const uint32_t ColorBlack = 0x000000;
	static const uint32_t ColorRed = 0xff0000;
	static const uint32_t ColorMaroon = 0x800000;
	static const uint32_t ColorYellow = 0xffff00;
	static const uint32_t ColorOlive = 0x808000;
	static const uint32_t ColorLime = 0x00ff00;
	static const uint32_t ColorGreen = 0x008000;
	static const uint32_t ColorAqua = 0x00ffff;
	static const uint32_t ColorTeal = 0x008080;
	static const uint32_t ColorBlue = 0x0000ff;
	static const uint32_t ColorNavy = 0x000080;
	static const uint32_t ColorFuchsia = 0xff00ff;
	static const uint32_t ColorPurple = 0x800080;

	static inline int32_t Sign(int32_t value)
	{
		if (value > 0)
			return  1;
		else if (value < 0)
			return -1;
		else
			return  0;
	}

	static inline float RadiansToDegrees(float radians)
	{
		return radians / PI * 180.0f;
	}

	static inline float DegreesToRadians(float degrees)
	{
		return degrees / 180.0f * PI;
	}

	static inline bool IsFloatEqual(float f1, float f2)
	{
		return fabsf(f1 - f2) < FLOAT_EPSILON;
	}

	static inline uint32_t ColorMake(uint8_t r, uint8_t g, uint8_t b)
	{
		return static_cast<uint32_t>((r) << 16) | static_cast<uint32_t>((g) << 8) | static_cast<uint32_t>(b);
	}

	static inline uint32_t ColorMakeARGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		return static_cast<uint32_t>((a) << 24) | static_cast<uint32_t>((r) << 16) | static_cast<uint32_t>((g) << 8) | static_cast<uint32_t>(b);
	}

	static inline uint8_t ColorGetAlpha(uint32_t color)
	{
		return (color >> 24) & 0xff;
	}

	static inline uint8_t ColorGetRed(uint32_t color)
	{
		return (color >> 16) & 0xff;
	}

	static inline uint8_t ColorGetGreen(uint32_t color)
	{
		return (color >> 8) & 0xff;
	}

	static inline uint8_t ColorGetBlue(uint32_t color)
	{
		return (color & 0xff);
	}
}

#endif