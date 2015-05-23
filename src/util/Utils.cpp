#include "base/Data.h"
#include "platform/FileUitls.h"
#include "util/Utils.h"
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <ctime>

namespace xGame2D
{
	static std::string currentDir = "";

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
		path = FileUtils::getInstance()->getFullPath(path);
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
}
