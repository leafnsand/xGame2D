#ifndef __X_OPENGL_H__
#define __X_OPENGL_H__

#include "base/Platform.h"

#if defined(X_PLATFORM_WIN)
#	define OPENGLES 0
#	define COMPRESSED_TEXTURE 0
#	include <GL/glew.h>
#elif defined(X_PLATFORM_MACOSX)
#	define OPENGLES 0
#	define COMPRESSED_TEXTURE 0
#	import <GL/glew.h>
#elif defined(X_PLATFORM_IOS)
#	define COMPRESSED_TEXTURE 1
#	define OPENGLES 2
#	import <OpenGLES/ES2/gl.h>
#elif defined(X_PLATFORM_ANDROID)
#endif

#endif