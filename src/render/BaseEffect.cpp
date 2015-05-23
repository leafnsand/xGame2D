#include "render/BaseEffect.h"
#include "render/Program.h"
#include "render/ProgramCache.h"
#include "geom/Matrix.h"
#include "platform/Application.h"
#include "textures/Texture.h"

namespace xGame2D
{
	static std::string getProgramName(bool hasTexture, bool useTinting)
	{
		if (hasTexture)
		{
			if (useTinting) return "Quad#11";
			else return "Quad#10";
		}
		else
		{
			if (useTinting) return "Quad#01";
			else return "Quad#00";
		}
	}

	BaseEffect::BaseEffect(): 
		mvpMatrix(Object::generate<Matrix>()), 
		texture(nullptr),
		alpha(1.0f),
		useTinting(true),
		premultipliedAlpha(false),
		aPosition(-1),
		aColor(-1),
		aTexcoords(-1),
		uMvpMatrix(-1),
		uAlpha(-1),
		program(nullptr)
	{
	}

	BaseEffect::~BaseEffect()
	{
		mvpMatrix->release();
		if (texture) texture->release();
		if (program) program->release();
	}

	void BaseEffect::prepareToDraw()
	{
		auto hasTexture = texture != nullptr;
		auto useTinting = this->useTinting || !texture || alpha != 1.0f;
		if (!program)
		{
			auto programName = getProgramName(hasTexture, useTinting);
			program = Application::getInstance()->programCache->getProgram(programName);
			if (!program)
			{
				auto vertexShader = vertexShaderForTexture(texture, useTinting);
				auto fragmentShader = fragmentShaderForTexture(texture, useTinting);
				program = Object::generate<Program>(vertexShader, fragmentShader);
				Application::getInstance()->programCache->registerProgram(program, programName);
			}
			program->retain();
			std::string name;
			name = "aPosition";
			aPosition = program->attribute(name);
			name = "aColor";
			aColor = program->attribute(name);
			name = "aTexcoords";
			aTexcoords = program->attribute(name);
			name = "uMvpMatrix";
			uMvpMatrix = program->uniform(name);
			name = "uAlpha";
			uAlpha = program->uniform(name);
		}

		float matrix[16] =
			{
				mvpMatrix->a, mvpMatrix->b, 0.0f, 0.0f,
				mvpMatrix->c, mvpMatrix->d, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				mvpMatrix->tx, mvpMatrix->ty, 0.0f, 1.0f
			};
		glUseProgram(program->getName());
		glUniformMatrix4fv(uMvpMatrix, 1, false, matrix);
		if (useTinting)
		{
			if (premultipliedAlpha) glUniform4f(uAlpha, alpha, alpha, alpha, alpha);
			else glUniform4f(uAlpha, 1.0f, 1.0f, 1.0f, alpha);
		}
		if (hasTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->getName());
		}
	}

	Matrix *BaseEffect::getMvpMatrix()
	{
		return mvpMatrix;
	}

	void BaseEffect::setMvpMatrix(Matrix *value)
	{
		mvpMatrix->clone(value);
	}

	Texture *BaseEffect::getTexture()
	{
		return texture;
	}

	void BaseEffect::setTexture(Texture *value)
	{
		if ((texture && !value) || (!texture && value))
		{
			if (program) program->release();
			program = nullptr;
		}
		if (value) value->retain();
		texture = value;
	}

	bool BaseEffect::getPremultipliedAlpha()
	{
		return premultipliedAlpha;
	}

	void BaseEffect::setPremultipliedAlpha(bool value)
	{
		premultipliedAlpha = value;
	}

	bool BaseEffect::getUseTinting()
	{
		return useTinting;
	}

	void BaseEffect::setUseTinting(bool value)
	{
		if (value != useTinting)
		{
			useTinting = value;
			if (program) program->release();
			program = nullptr;
		}
	}

	float BaseEffect::getAlpha()
	{
		return alpha;
	}

	void BaseEffect::setAlpha(float value)
	{
		alpha = value;
	}

	int32_t BaseEffect::getAttribPosition()
	{
		return aPosition;
	}

	int32_t BaseEffect::getAttribTexcoord()
	{
		return aTexcoords;
	}

	int32_t BaseEffect::getAttribColor()
	{
		return aColor;
	}

	std::string BaseEffect::vertexShaderForTexture(Texture *texture, bool useTinting)
	{
		auto hasTexture = texture != nullptr;
		std::string source;
#if (OPENGLES == 0)
		source.append("#define lowp\n");
#endif
		source.append("attribute vec4 aPosition;\n");
		if (useTinting)
		{
			source.append("attribute vec4 aColor;\n");
		}
		if (hasTexture)
		{
			source.append("attribute vec2 aTexcoords;\n");
		}
		source.append("uniform mat4 uMvpMatrix;\n");
		if (useTinting)
		{
			source.append("uniform vec4 uAlpha;\n");
			source.append("varying lowp vec4 vColor;\n");
		}
		if (hasTexture)
		{
			source.append("varying lowp vec2 vTexcoords;\n");
		}
		source.append("void main() {\n");
		source.append("  gl_Position = uMvpMatrix * aPosition;\n");
		if (useTinting)
		{
			source.append("  vColor = aColor * uAlpha;\n");
		}
		if (hasTexture)
		{
			source.append("  vTexcoords  = aTexcoords;\n");
		}
		source.append("}");
		return source;
	}

	std::string BaseEffect::fragmentShaderForTexture(Texture *texture, bool useTinting)
	{
		auto hasTexture = texture != nullptr;
		std::string source;
#if (OPENGLES == 0)
		source.append("#define lowp\n");
#endif
		if (useTinting)
		{
			source.append("varying lowp vec4 vColor;\n");
		}
		if (hasTexture)
		{
			source.append("varying lowp vec2 vTexcoords;\n");
			source.append("uniform lowp sampler2D uTexture;\n");
		}
		source.append("void main() {\n");
		if (hasTexture)
		{
			if (useTinting)
			{
				source.append("  gl_FragColor = texture2D(uTexture, vTexcoords) * vColor;\n");
			}
			else
			{
				source.append("  gl_FragColor = texture2D(uTexture, vTexcoords);\n");
			}
		}
		else
		{
			source.append("  gl_FragColor = vColor;\n");
		}
		source.append("}");
		return source;
	}
}
