#include "geom/Matrix.h"
#include "geom/Point.h"

namespace xGame2D
{
	Matrix::Matrix()
		: a(1.0f)
		, b(0.0f)
		, c(0.0f)
		, d(1.0f)
		, tx(0.0f)
		, ty(0.0f)
	{
	}

	Matrix::~Matrix()
	{
	}

	bool Matrix::init(float a, float b, float c, float d, float tx, float ty)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->tx = tx;
		this->ty = ty;
		return true;
	}

	bool Matrix::init(float angle)
	{
		return init(cosf(angle), sinf(angle), -sinf(angle), cosf(angle), 0.0f, 0.0f);
	}

	bool Matrix::init(float value1, float value2, bool forScale)
	{
		if (forScale)
		{
			return init(value1, 0.0f, 0.0f, value2, 0.0f, 0.0f);
		}
		else // for translation
		{
			return init(1.0f, 0.0f, 0.0f, 1.0f, value1, value2);
		}
	}

	void Matrix::set(float a, float b, float c, float d, float tx, float ty)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->tx = tx;
		this->ty = ty;
	}

	bool Matrix::isEqualTo(Matrix *matrix)
	{
		if (matrix == this)
			return true;
		else if (!matrix)
			return false;
		else
			return IsFloatEqual(a, matrix->a) && IsFloatEqual(b, matrix->b)
				&& IsFloatEqual(c, matrix->c) && IsFloatEqual(d, matrix->d)
				&& IsFloatEqual(tx, matrix->tx) && IsFloatEqual(ty, matrix->ty);
	}

	void Matrix::appendMatrix(Matrix *left)
	{
		set(left->a * a + left->c * b,
			left->b * a + left->d * b,
			left->a * c + left->c * d,
			left->b * c + left->d * d,
			left->a * tx + left->c * ty + left->tx,
			left->b * tx + left->d * ty + left->ty);
	}

	void Matrix::prependMatrix(Matrix *right)
	{
		set(a * right->a + c * right->b,
			b * right->a + d * right->b,
			a * right->c + c * right->d,
			b * right->c + d * right->d,
			tx + a * right->tx + c * right->ty,
			ty + b * right->tx + d * right->ty);
	}

	void Matrix::translate(float x, float y)
	{
		tx += x;
		ty += y;
	}

	void Matrix::scale(float x, float y)
	{
		if (x != 1.0f)
		{
			a *= x;
			c *= x;
			tx *= x;
		}
		if (y != 1.0f)
		{
			b *= y;
			d *= y;
			ty *= y;
		}
	}

	void Matrix::scale(float scale)
	{
		this->scale(scale, scale);
	}

	void Matrix::skew(float x, float y)
	{
		auto sinX = sinf(x);
		auto cosX = cosf(x);
		auto sinY = sinf(y);
		auto cosY = cosf(y);
		set(a  * cosY - b  * sinX,
			a  * sinY + b  * cosX,
			c  * cosY - d  * sinX,
			c  * sinY + d  * cosX,
			tx * cosY - ty * sinX,
			tx * sinY + ty * cosX);
	}

	void Matrix::rotate(float angle)
	{
		if (angle == 0.0f) return;
		auto cos = cosf(angle);
		auto sin = sinf(angle);
		set(a * cos - b * sin, a * sin + b * cos,
			c * cos - d * sin, c * sin + d * cos,
			tx * cos - ty * sin, tx * sin + ty * cos);
	}

	void Matrix::identity()
	{
		a = 1.0f;
		b = 0.0f;
		c = 0.0f;
		d = 1.0f;
		tx = 0.0f;
		ty = 0.0f;
	}

	void Matrix::invert()
	{
		auto det = getDeterminant();
		set(d / det,
			-b / det,
			-c / det,
			a / det,
			(c*ty - d*tx) / det,
			(b*tx - a*ty) / det);
	}

	void Matrix::clone(Matrix *matrix)
	{
		a = matrix->a;
		b = matrix->b;
		c = matrix->c;
		d = matrix->d;
		tx = matrix->tx;
		ty = matrix->ty;
	}

	Point *Matrix::transform(Point *point)
	{
		return Object::create<Point>(a * point->x + c * point->y + tx, b * point->x + d * point->y + ty);
	}

	Point *Matrix::transform(float x, float y)
	{
		return Object::create<Point>(a * x + c * y + tx, b * x + d * y + ty);
	}

	Matrix *Matrix::copy()
	{
		return Object::generate<Matrix>(a, b, c, d, tx, ty);
	}

	float Matrix::getDeterminant()
	{
		return a * d - c * b;
	}

	float Matrix::getRotation()
	{
		return atan2f(b, a);
	}

	float Matrix::getScaleX()
	{
		return a / cosf(getSkewY());
	}

	float Matrix::getScaleY()
	{
		return d / cosf(getSkewX());
	}

	float Matrix::getSkewX()
	{
		return atanf(-c / d);
	}

	float Matrix::getSkewY()
	{
		return atanf(b / a);
	}
}