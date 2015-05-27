#ifndef __X_CONSOLE_H__
#define __X_CONSOLE_H__

#include <iostream>

namespace xGame2D
{
	class Console
	{
	private:
		explicit Console() {};

	public:
		template <typename T>
		const Console &operator<<(const T &t) const
		{
			std::cout << t;
			return *this;
		}

		const Console &operator<<(const std::string &endl) const
		{
			std::cout << endl;
			if (this == &error && endl == Console::endl)
			{
				exit(1);
			}
			return *this;
		}

		static const std::string endl;
		static const Console debug;
		static const Console log;
		static const Console error;
	};
}

#endif
