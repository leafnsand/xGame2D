#include "render/ProgramCache.h"

namespace xGame2D
{
	ProgramCache::ProgramCache()
	{
	}

	ProgramCache::~ProgramCache()
	{
		cache.clear();
	}

	void ProgramCache::registerProgram(Program *program, std::string &name)
	{
		cache[name] = program;
	}

	Program *ProgramCache::getProgram(std::string &name)
	{
		return cache[name];
	}

	void ProgramCache::unregisterProgram(std::string &name)
	{
		cache.erase(name);
	}
}
