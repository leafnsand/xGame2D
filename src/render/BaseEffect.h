#ifndef __X_BASEEFFECT_H__
#define __X_BASEEFFECT_H__

#include "base/Object.h"
#include <string>

namespace xGame2D
{
	class Texture;
	class Matrix;
	class Program;

	class BaseEffect : public Object
	{
	constructor:
        BaseEffect();
        ~BaseEffect();

	public:
		void prepareToDraw();

		Matrix *getMvpMatrix();
		void setMvpMatrix(Matrix *value);
		Texture *getTexture();
		void setTexture(Texture *value);
		bool getPremultipliedAlpha();
		void setPremultipliedAlpha(bool value);
		bool getUseTinting();
		void setUseTinting(bool value);
		float getAlpha();
		void setAlpha(float value);
		int32_t getAttribPosition();
		int32_t getAttribTexcoord();
		int32_t getAttribColor();

	private:
		std::string vertexShaderForTexture(Texture *texture, bool useTinting);
		std::string fragmentShaderForTexture(Texture *texture, bool useTinting);

	protected:
		Matrix *mvpMatrix;
		Texture *texture;
		float alpha;
		bool useTinting, premultipliedAlpha;
		int32_t aPosition, aColor, aTexcoords, uMvpMatrix, uAlpha;
		Program *program;
	};
}

#endif