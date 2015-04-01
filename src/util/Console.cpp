#include "util/Console.h"
#include <cstdlib>
#include <cstdio>
#include <cstdarg>

namespace xGame2D
{
    void Console::Log(const char *format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vprintf(format, ap);
		va_end(ap);
		printf("\n");
	}
    
    void Console::Error(const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vprintf(format, ap);
        va_end(ap);
		printf("\n");
        exit(1);
    }
}