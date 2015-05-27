#include "base/AutoreleasePool.h"
#include "base/Object.h"
#include "util/Console.h"
#include <typeinfo>

namespace xGame2D
{
	AutoreleasePool *AutoreleasePool::instance = nullptr;

	AutoreleasePool::AutoreleasePool()
	{
		cache.reserve(150);
	}

	AutoreleasePool::~AutoreleasePool()
	{
		clear();
	}

	void AutoreleasePool::clear()
	{
		for (auto object : cache)
		{
			object->release();
		}
		cache.clear();
	}

	void AutoreleasePool::dump()
	{
		//      Console::Log("Pool size: %d.", cache.size());
		for (auto object : cache)
		{
			Console::debug << "Object[" << typeid(*object).name() << "]: " << object << ", reference count: " << object->referenceCount() << Console::endl;
		}
		cache.clear();
	}
}
