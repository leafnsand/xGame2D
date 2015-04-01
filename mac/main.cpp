#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Game.h>
#include <util/Console.h>

#define WIDTH 1024
#define HEIGHT 768

static GLFWwindow *window = NULL;

static void onGLFWError(int errorID, const char* errorDesc)
{
    xGame2D::Console::Error("glfw error, code: %d, string: %s.\n", errorID, errorDesc);
}

//////////////////////////////////////////////////////////////////////////

#include <display/Stage.h>
#include <display/Image.h>
#include <textures/Texture.h>

using namespace xGame2D;

static void init()
{
    Game::getInstance()->init(WIDTH, HEIGHT, ColorGray);
    
    std::string name = "test.png";
    Image *image = Object::create<Image>(name);
    image->setScale(0.5f);
    Game::getInstance()->stage->addChild(image);
}

static void loop()
{
    Game::getInstance()->update();
}

static void destroy()
{
    
}

//////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[]) {
    glfwSetErrorCallback(onGLFWError);
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "xGame2D", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        xGame2D::Console::Error("glew init error.\n");
    }
    
    init();
    
    while(!glfwWindowShouldClose(window))
    {
        
        loop();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    destroy();
    
    glfwTerminate();
    return 0;
}