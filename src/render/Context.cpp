#include "render/Context.h"
#include "platform/OpenGL.h"
#include "geom/Rectangle.h"

namespace xGame2D
{
	Context::Context()
	{
	}

	Context::~Context()
	{
	}

	void Context::renderToBackBuffer()
	{
		setRenderTarget(nullptr);
	}

	void Context::presentBufferForDisplay()
	{
	}

	bool Context::makeCurrentContext()
	{
		return setCurrentContext(this);
	}

	bool Context::setCurrentContext(Context *context)
	{
		return true;
	}

	Context *Context::currentContext()
	{
		return nullptr;
	}

	Rectangle *Context::getViewport()
	{
		int32_t viewport[4];
		glGetIntegerv(GL_VIEWPORT, (int32_t *)&viewport);
		return Object::create<Rectangle>((float)viewport[0], (float)viewport[1], (float)viewport[2], (float)viewport[3]);
	}

	void Context::setViewport(Rectangle *value)
	{
		if (value)
		{
			glViewport((GLint)value->x, (GLint)value->y, (GLsizei)value->width, (GLsizei)value->height);
		}
		else
		{
			// TODO
			glViewport(0, 0, 1024, 768);
		}
	}

	Rectangle *Context::getScissorBox()
	{
		int32_t scissorBox[4];
		glGetIntegerv(GL_SCISSOR_BOX, (int32_t *)&scissorBox);
		return Object::create<Rectangle>((float)scissorBox[0], (float)scissorBox[1], (float)scissorBox[2], (float)scissorBox[3]);
	}

	void Context::setScissorBox(Rectangle *value)
	{
		if (value)
		{
			glEnable(GL_SCISSOR_TEST);
			glScissor((GLint)value->x, (GLint)value->y, (GLsizei)value->width, (GLsizei)value->height);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}
	}

	Texture *Context::getRenderTarget()
	{
		return renderTarget;
	}

	void Context::setRenderTarget(Texture *value)
	{
	}
}
