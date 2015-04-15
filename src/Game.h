#ifndef __X_GAME2D_H__
#define __X_GAME2D_H__

#include "base/Object.h"
#include <cstdint>

namespace xGame2D
{
	class RenderSupport;
	class ProgramCache;
	class TextureCache;
	class Stage;
	class Context;
	class TimerHandler;

	class Game : public Object
	{
	constructor:
		Game();
		virtual ~Game();

	public:
		static inline Game *getInstance() { if (instance == nullptr) instance = Object::generate<Game>(); return instance;}
		
		OVERRIDE_DEFAULT_INIT
		bool init(float width, float height, uint32_t color);
		void update();

	public:
		ProgramCache *programCache;
		TextureCache *textureCache;
		Stage *stage;
		Context *context;
		TimerHandler *timerHandler;

	private:
		static Game *instance;
		RenderSupport *support;
	};
}

#endif