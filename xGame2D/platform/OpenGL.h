#ifndef __X_OPENGL_H__
#define __X_OPENGL_H__

#include "platform/Platform.h"
#include "base/Object.h"

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

namespace xGame2D
{
    class OpenGL : public Object
    {
    constructor:
        OpenGL();
        virtual ~OpenGL();

    public:
        OVERRIDE_DEFAULT_INIT
        virtual bool init(const std::string &name, int32_t width, int32_t height);
        virtual void pollEvents() = 0;
        virtual void swapBuffers() = 0;
        virtual bool windowShouldClose() = 0;

        void setName(const std::string &name);
        const std::string &getName() const;

    protected:
        std::string name;
    };
}

#endif