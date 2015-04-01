#ifndef __X_PROGRAM_H__
#define __X_PROGRAM_H__

#include "base/Object.h"
#include "render/OpenGL.h"
#include <string>
#include <map>

namespace xGame2D
{
	class Program : public Object
	{
	constructor:
		Program();
		~Program();

	public:
		OVERRIDE_DEFAULT_INIT
		bool init(std::string &vertexShader, std::string &fragmentShader);

		int32_t uniform(std::string &name);
		int32_t attribute(std::string &name);

		uint32_t getName();
		std::string getVertexShader();
		std::string getFragmentShader();

	private:
		void compile();
		uint32_t compileShader(std::string &source, GLenum type);
		void updateUniforms();
		void updateAttributes();

	protected:
		uint32_t name;
		std::string vertexShader, fragmentShader;
		std::map<std::string, GLint> uniforms, attributes;
	};
}

#endif