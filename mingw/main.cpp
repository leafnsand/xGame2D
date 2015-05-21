#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Game.h>
#include <util/Console.h>

#define WIDTH 1024
#define HEIGHT 768

static GLFWwindow *window = nullptr;

static void onGLFWError(int errorID, const char *errorDesc)
{
	xGame2D::Console::Error("glfw error, code: %d, string: %s.\n", errorID, errorDesc);
}

//////////////////////////////////////////////////////////////////////////

#include <display/Stage.h>
#include <display/Image.h>
#include <display/Sprite.h>
#include <textures/Texture.h>
#include <timer/TimerHandler.h>

using namespace xGame2D;

class TestSprite : public Sprite
{
constructor:
	TestSprite() {}
	~TestSprite() {}

public:
	virtual bool init() override
	{
		std::string name = "test.png";
		auto image = Object::create<Image>(name);
		addChild(image);
		Game::getInstance()->timerHandler->registerUpdateTimer(this, UPDATE(TestSprite::update), 0, false);
		Game::getInstance()->timerHandler->registerCallbackTimer(this, CALLBACK(TestSprite::callback), 1, UINT32_MAX, 0, false, name);
		return true;
	}

	void update(float delta)
	{
//		Console::Log("update, %d", Game::getInstance()->timerHandler->getTotalFrame());
	}

	void callback()
	{
		Console::Log("callback, %d", Game::getInstance()->timerHandler->getTotalFrame());
	}
};

static void init()
{
	Game::getInstance()->init(WIDTH, HEIGHT, ColorGray);
	Game::getInstance()->setFrameRate(60);

	auto testSprite = Object::create<TestSprite>();
	Game::getInstance()->stage->addChild(testSprite);
}

static void loop()
{
	Game::getInstance()->loop();
}

static void destroy()
{
}

//////////////////////////////////////////////////////////////////////////

int main(int argc, const char *argv[])
{
	glfwSetErrorCallback(onGLFWError);
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "xGame2D", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		xGame2D::Console::Error("glew init error.\n");
	}

	init();

	while (!glfwWindowShouldClose(window))
	{
		loop();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	destroy();

	glfwTerminate();
	return 0;
}
