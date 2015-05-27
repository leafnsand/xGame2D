#include "geom/Point.h"
#include "base/Math.h"

namespace xGame2D
{
	Point::Point(): 
		x(0.0f), 
		y(0.0f)
	{
	}

	Point::~Point()
	{
	}

	bool Point::init(float x, float y)
	{
		this->x = x;
		this->y = y;
		return true;
	}

	Point *Point::pointWithLengthAndAngle(float length, float angle)
	{
		return Object::create<Point>(cosf(angle) * length, sinf(angle) * length);
	}

	Point *Point::add(Point *value)
	{
		return Object::create<Point>(x + value->x, y + value->y);
	}

	Point *Point::sub(Point *value)
	{
		return Object::create<Point>(x - value->x, y - value->y);
	}

	Point *Point::scale(float scale)
	{
		return Object::create<Point>(x * scale, y * scale);
	}

	Point *Point::rotate(float angle)
	{
		auto sina = sinf(angle);
		auto cosa = cosf(angle);
		return Object::create<Point>(x * cosa - y * sina, x * sina + y * cosa);
	}

	Point *Point::normalize()
	{
		if (isOrigin()) return Object::create<Point>(1.0f, 0.0f);
		auto inverseLength = 1.0f / length();
		return Object::create<Point>(x * inverseLength, y * inverseLength);
	}

	Point *Point::invert()
	{
		return Object::create<Point>(-x, -y);
	}

	Point *Point::perpendicular()
	{
		return Object::create<Point>(-y, -x);
	}

	Point *Point::truncateLength(float maxLength)
	{
		auto maxLengthSquared = maxLength * maxLength;
		auto vecLengthSquared = lengthSquared();
		if (vecLengthSquared <= maxLengthSquared)
		{
			auto value = copy();
			value->autorelease();
			return value;
		}
		else
		{
			return scale(maxLength / sqrtf(vecLengthSquared));
		}
	}

	float Point::dot(Point *value)
	{
		return x * value->x + y * value->y;
	}

	bool Point::isEqualTo(Point *value)
	{
		if (value == this) return true;
		else if (!value) return false;
		else return IsFloatEqual(x, value->x) && IsFloatEqual(y, value->y);
	}

	void Point::clone(Point *value)
	{
		x = value->x;
		y = value->y;
	}

	void Point::set(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float Point::distanceBetween(Point *p1, Point *p2)
	{
		return sqrtf(SQUARE(p2->x - p1->x) + SQUARE(p2->y - p1->y));
	}

	float Point::angleBetween(Point *p1, Point *p2)
	{
		auto cos = p1->dot(p2) / (p1->length() * p2->length());
		return cos >= 1.0f ? 0.0f : acosf(cos);
	}

	Point *Point::interpolateBetween(Point *p1, Point *p2, float ratio)
	{
		auto invRatio = 1.0f - ratio;
		return Object::create<Point>(invRatio * p1->x + ratio * p2->x, invRatio * p1->y + ratio * p2->y);
	}

	Point *Point::copy()
	{
		return Object::create<Point>(x, y);
	}

	float Point::length()
	{
		return sqrtf(SQUARE(x) + SQUARE(y));
	}

	float Point::lengthSquared()
	{
		return SQUARE(x) + SQUARE(y);
	}

	float Point::angle()
	{
		return atan2f(y, x);
	}

	bool Point::isOrigin()
	{
		return x == 0.0f && y == 0.0f;
	}
}
