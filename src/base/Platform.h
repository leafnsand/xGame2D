#ifndef __X_PLATFORM_H__
#define __X_PLATFORM_H__

#if defined(WIN32) || defined(_WIN32)
#	define X_PLATFORM_WIN
#elif defined(__APPLE__)
#	include "TargetConditionals.h"
#	if TARGET_IPHONE_SIMULATOR
#		define X_PLATFORM_IOS
#	elif TARGET_OS_IPHONE
#		define X_PLATFORM_IOS
#	elif TARGET_OS_MAC
#		define X_PLATFORM_MACOSX
#	endif
#elif defined(__ANDROID__)
#	define X_PLATFORM_ANDROID
#endif

#endif