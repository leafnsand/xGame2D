#include "MyApplication.h"
#include <xGame2D.h>

using namespace xGame2D;

int main(int argc, const char *argv[])
{
	MyApplication app;
	return Application::getInstance()->run(&app);
}
