#ifndef __X_MATRIX_H__
#define __X_MATRIX_H__

#include "base/Object.h"
#include "base/Copying.h"
#include "base/Math.h"

namespace xGame2D
{
	class Point;

	class Matrix : public Object, Copying
	{
	constructor:
		Matrix();
		virtual ~Matrix();

	public:
		OVERRIDE_DEFAULT_INIT
		bool init(float angle);
		bool init(float value1, float value2, bool forScale);
		bool init(float a, float b, float c, float d, float tx, float ty);

		void set(float a, float b, float c, float d, float tx, float ty);
		bool isEqualTo(Matrix *matrix);
		void appendMatrix(Matrix *left);
		void prependMatrix(Matrix *right);
		void translate(float x, float y);
		void scale(float x, float y);
		void scale(float scale);
		void skew(float x, float y);
		void rotate(float angle);
		void identity();
		void invert();
		void clone(Matrix *matrix);
		Point *transform(Point *point);
		Point *transform(float x, float y);

		virtual Matrix *copy() override;

		float getDeterminant();
		float getRotation();
		float getScaleX();
		float getScaleY();
		float getSkewX();
		float getSkewY();

	public:
		float a, b, c, d, tx, ty;
	};
}

#endif