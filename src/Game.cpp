#include "Game.h"
#include "display/Stage.h"
#include "render/ProgramCache.h"
#include "render/RenderSupport.h"
#include "render/Context.h"
#include "timer/TimerHandler.h"
#include "textures/TextureCache.h"
#include "util/Utils.h"

namespace xGame2D
{
	Game *Game::instance = nullptr;

	Game::Game():
		lastUpdate(0.0f)
	{
		setFrameRate(60);
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

	void Game::setFrameRate(int8_t frameRate)
	{
		this->frameRate = frameRate;
		this->deltaTimePerFrame = 1.0f / this->frameRate;
	}


	void Game::loop()
	{
		auto now = Utils::now();
		if (lastUpdate == 0.0f)
		{
			lastUpdate = now;
		}
		else
		{
			auto delta = now - lastUpdate;
			if (delta >= deltaTimePerFrame)
			{
				lastUpdate = now;
				update(delta);
			}
		}
	}

	void Game::update(float delta)
	{
		support->prepare();
		support->nextFrame();
		stage->render(support);
		support->finishQuadBatch();
		timerHandler->update(delta);
#if DEBUG || _DEBUG
		RenderSupport::checkForOpenGLError();
		AutoreleasePool::getInstance()->dump();
#endif
		AutoreleasePool::getInstance()->clear();
	}
}
