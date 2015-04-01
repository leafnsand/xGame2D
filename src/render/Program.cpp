#include "render/Program.h"
#include "Util/Console.h"
#include <cstdlib>

namespace xGame2D
{
	Program::Program()
		: name(0)
		, vertexShader("")
		, fragmentShader("")
	{
	}

	Program::~Program()
	{
		if (name)
		{
			glDeleteProgram(name);
		}
	}

	bool Program::init(std::string &vertexShader, std::string &fragmentShader)
	{
		this->vertexShader = vertexShader;
		this->fragmentShader = fragmentShader;
		compile();
		updateUniforms();
		updateAttributes();
		return true;
	}

	int32_t Program::uniform(std::string &name)
	{
		auto iterator = uniforms.find(name);
		if (iterator == uniforms.end()) return -1;
		else return iterator->second;
	}

	int32_t Program::attribute(std::string &name)
	{
		auto iterator = attributes.find(name);
		if (iterator == attributes.end()) return -1;
		else return iterator->second;
	}

	uint32_t Program::getName()
	{
		return name;
	}

	std::string Program::getVertexShader()
	{
		return vertexShader;
	}

	std::string Program::getFragmentShader()
	{
		return fragmentShader;
	}

	void Program::compile()
	{
		auto program = glCreateProgram();
		auto vertexShader = compileShader(this->vertexShader, GL_VERTEX_SHADER);
		auto fragmentShader = compileShader(this->fragmentShader, GL_FRAGMENT_SHADER);
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
#if (DEBUG || _DEBUG)
		auto linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			auto length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			if (length)
			{
				auto log = static_cast<char *>(malloc(sizeof(char)* length));
				glGetProgramInfoLog(program, length, nullptr, log);
				Console::Error("Error linking program: %s", log);
				free(log);
			}
		}
#endif
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		name = program;
	}

	uint32_t Program::compileShader(std::string &source, GLenum type)
	{
		auto shader = glCreateShader(type);
		if (!shader) return shader;
		const auto string = source.c_str();
		glShaderSource(shader, 1, &string, nullptr);
		glCompileShader(shader);
#if (DEBUG || _DEBUG)
		auto compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			auto logLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength)
			{
				auto log = static_cast<char *>(malloc(sizeof(char)* logLength));
				glGetShaderInfoLog(shader, logLength, nullptr, log);
				Console::Error("Error compiling %s shader: %s",
					type == GL_VERTEX_SHADER ? "vertex" : "fragment", log);
				free(log);
			}
			glDeleteShader(shader);
			return 0;
		}
#endif
		return shader;
	}

	void Program::updateUniforms()
	{
		const auto MAX_NAME_LENGTH = 64;
		char rawName[MAX_NAME_LENGTH];
		auto numUniforms = 0;
		glGetProgramiv(name, GL_ACTIVE_UNIFORMS, &numUniforms);
		uniforms.clear();
		for (auto i = 0; i < numUniforms; i++)
		{
			GLsizei length;
			GLint size;
			GLenum type;
			glGetActiveUniform(name, i, MAX_NAME_LENGTH, &length, &size, &type, rawName);
			uniforms[std::string(rawName)] = glGetUniformLocation(name, rawName);
		}
	}

	void Program::updateAttributes()
	{
		const auto MAX_NAME_LENGTH = 64;
		char rawName[MAX_NAME_LENGTH];
		auto numAttributes = 0;
		glGetProgramiv(name, GL_ACTIVE_ATTRIBUTES, &numAttributes);
		attributes.clear();
		for (auto i = 0; i < numAttributes; i++)
		{
			GLsizei length;
			GLint size;
			GLenum type;
			glGetActiveAttrib(name, i, MAX_NAME_LENGTH, &length, &size, &type, rawName);
			attributes[std::string(rawName)] = glGetAttribLocation(name, rawName);
		}
	}
}