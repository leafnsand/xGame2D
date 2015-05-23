#include "platform/OpenGL.h"

namespace xGame2D
{
    OpenGL::OpenGL()
    {
    }

    OpenGL::~OpenGL()
    {
    }

    bool OpenGL::init(const std::string &name, int32_t width, int32_t height)
    {
        if (Object::init())
        {
            setName(name);
            return true;
        }
        return false;
    }

    void OpenGL::setName(const std::string &name)
    {
        this->name = name;
    }

    const std::string &OpenGL::getName() const
    {
        return name;
    }
}