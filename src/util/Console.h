#ifndef __X_CONSOLE_H__
#define __X_CONSOLE_H__

namespace xGame2D
{
	class Console
	{
	public:
		static void Log(const char *format, ...);
		static void Error(const char *format, ...);
	};
}

#endif
