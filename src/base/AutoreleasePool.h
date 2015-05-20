#ifndef __X_AUTORELEASEPOOL_H__
#define __X_AUTORELEASEPOOL_H__

#include <vector>
#include <algorithm>

namespace xGame2D
{
	class Object;
	class Game;

	class AutoreleasePool
	{
	protected:
		AutoreleasePool();
		virtual ~AutoreleasePool();

		void destroy()
		{
			if (instance) delete instance;
		}

	public:
		static inline AutoreleasePool *getInstance()
		{
			if (instance == nullptr) instance = new AutoreleasePool;
			return instance;
		}

		inline void addObject(Object *object)
		{
			if (std::find(cache.begin(), cache.end(), object) == cache.end()) cache.push_back(object);
		}

		void clear();
		void dump();

	private:
		static AutoreleasePool *instance;
		std::vector<Object *> cache;

		friend class Game;
	};
}

#endif
