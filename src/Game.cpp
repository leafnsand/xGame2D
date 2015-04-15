#include "Game.h"
#include "display/Stage.h"
#include "render/ProgramCache.h"
#include "render/RenderSupport.h"
#include "render/Context.h"
#include "timer/TimerHandler.h"
#include "textures/TextureCache.h"

namespace xGame2D
{
	Game *Game::instance = nullptr;

	Game::Game()
	{
	}

	Game::~Game()
	{
		programCache->release();
		textureCache->release();
		support->release();
		stage->release();
		AutoreleasePool::getInstance()->destroy();
	}

	bool Game::init(float width, float height, uint32_t color)
	{
		programCache = Object::generate<ProgramCache>();
		textureCache = Object::generate<TextureCache>();
		context = Object::generate<Context>();
		support = Object::generate<RenderSupport>();
		stage = Object::generate<Stage>(width, height, color);
		timerHandler = Object::generate<TimerHandler>();
		return true;
	}

	void Game::update()
	{
		support->prepare();
		support->nextFrame();
		stage->render(support);
		support->finishQuadBatch();
#if DEBUG || _DEBUG
		RenderSupport::checkForOpenGLError();
		AutoreleasePool::getInstance()->dump();
#endif
		AutoreleasePool::getInstance()->clear();
	}
}