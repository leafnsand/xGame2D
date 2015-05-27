#ifndef __X_PROGRAMCACHE_H__
#define __X_PROGRAMCACHE_H__

#include "base/Object.h"
#include <string>
#include <map>

namespace xGame2D
{
	class Program;

	class ProgramCache : public Object
	{
	constructor:
		ProgramCache();
		virtual ~ProgramCache();

	public:
		void registerProgram(Program *program, std::string &name);
		Program *getProgram(std::string &name);
		void unregisterProgram(std::string &name);

	private:
		std::map<std::string, Program *> cache;
	};
}

#endif
