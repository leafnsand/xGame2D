#ifndef __X_OBJECT_H__
#define __X_OBJECT_H__

#include "base/AutoreleasePool.h"
#include "util/Console.h"
#include <stdint.h>
#include <utility>

#define constructor friend class Object;protected
#define OVERRIDE_DEFAULT_INIT virtual inline bool init() { return true; }
#if DEBUG || _DEBUG
#	define X_ASSERT(condition, message) do { \
		if (!(condition)) \
		{ \
			Console::Error(message); \
		} \
	} while (0)
#else
#	define X_ASSERT(condition, message)
#endif

namespace xGame2D
{
	class Object
	{
	protected:
		Object():ref(1)
		{
		};

		virtual ~Object()
		{
		};

	public:
		virtual inline bool init()
		{
			return true;
		}

		inline void retain()
		{
			++ref;
		}

		inline void release()
		{
			if (--ref == 0)
				delete this;
		}

		inline void autorelease()
		{
			AutoreleasePool::getInstance()->addObject(this);
		}

		inline int32_t referenceCount()
		{
			return ref;
		}

		template <class T, typename ...Args>
		static inline T *generate(Args && ...args)
		{
			T *v = new T();
			if (v->init(std::forward<Args>(args)...))
			{
				return v;
			}
			return nullptr;
		}

		template <class T, typename ...Args>
		static inline T *create(Args && ...args)
		{
			T *v = new T();
			if (v->init(std::forward<Args>(args)...))
			{
				v->autorelease();
				return v;
			}
			return nullptr;
		}

	protected:
		int32_t ref;
	};
}

#endif
