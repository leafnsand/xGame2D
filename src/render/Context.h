#ifndef __X_CONTEXT_H__
#define __X_CONTEXT_H__

#include "base/Object.h"

namespace xGame2D
{
	class Rectangle;
	class Texture;

	class Context : public Object
	{
	constructor:
		Context();
		~Context();

	public:
		void renderToBackBuffer();
		void presentBufferForDisplay();
		bool makeCurrentContext();
		
		static bool setCurrentContext(Context *context);
		static Context *currentContext();

		Rectangle *getViewport();
		void setViewport(Rectangle *value);
		Rectangle *getScissorBox();
		void setScissorBox(Rectangle *value);
		Texture *getRenderTarget();
		void setRenderTarget(Texture *value);

	private:
		Texture *renderTarget;
	};
}

#endif