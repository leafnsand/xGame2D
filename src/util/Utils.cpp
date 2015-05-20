#include "base/Platform.h"
#include "base/Data.h"
#include "util/Utils.h"
#include <climits>
#include <cstdlib>
#include <sys/stat.h>
#include <iostream>
#include <fstream>

#if defined(X_PLATFORM_WIN)
#	include <direct.h>
#	include <Windows.h>
#else
#	include <unistd.h>
#endif

namespace xGame2D
{
	static std::string currentDir = "";
	static float frequency = 0;

	int32_t Utils::nextPowerOfTwo(int32_t number)
	{
		auto result = 1;
		while (result < number) result *= 2;
		return result;
	}

	bool Utils::isPowerOfTwo(int32_t number)
	{
		return (number != 0) && !(number & (number - 1));
	}

	int32_t Utils::randomIntBetween(int32_t min, int32_t max)
	{
		return static_cast<int32_t>(min + randomFloat() * (max - min));
	}

	float Utils::randomFloatBetween(float min, float max)
	{
		return static_cast<float>(min + randomFloat() * (max - min));
	}

	float Utils::randomFloat()
	{
		return static_cast<float>(std::rand()) / UINT_MAX ;
	}

	Data *Utils::readFile(std::string &path)
	{
#if defined(X_PLATFORM_WIN)
		if (currentDir.empty())
		{
			char cwd[256];
			_getcwd(cwd, sizeof(cwd));
			currentDir = cwd;
			currentDir.append("\\");
		}
		if (!(path.length() > 2
			&& ((path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z'))
			&& path[1] == ':'))
		{
			path.insert(0, currentDir);
		}
#else
		if (currentDir.empty())
		{
			char cwd[256];
			getcwd(cwd, sizeof(cwd));
			currentDir = cwd;
			currentDir.append("/");
		}
		if (path[0] != '/')
		{
			path.insert(0, currentDir);
		}
#endif
		struct stat s;
		if (stat(path.c_str(), &s) != -1 && ((s.st_mode & S_IFMT) != 0))
		{
			std::ifstream file(path.c_str(), std::ios::binary);
			file.seekg(0, file.end);
			auto size = static_cast<size_t>(file.tellg());
			file.seekg(0, file.beg);
			auto buffer = static_cast<char *>(malloc(size));
			file.read(buffer, size);
			file.close();
			return Object::create<Data>(buffer, size);
		}
		return nullptr;
	}

	float Utils::now() {
		float t;
#if defined(X_PLATFORM_WIN)
		LARGE_INTEGER time;
		if (frequency == 0)
		{
			QueryPerformanceFrequency(&time);
			frequency = static_cast<float>(time.QuadPart);
		}
		QueryPerformanceCounter(&time);
		t = static_cast<float>(time.QuadPart);
		t /= frequency;
#elif defined(X_PLATFORM_LINUX)
#elif defined(X_PLATFORM_OSX)
#endif
		return t;
	}
}
