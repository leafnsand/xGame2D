#ifndef __X_POINT_H__
#define __X_POINT_H__

#include "base/Object.h"
#include "base/Copying.h"
#include "base/Math.h"

namespace xGame2D
{
	class Point : public Object, Copying
	{
	constructor:
		Point();
		virtual ~Point();

	public:
		OVERRIDE_DEFAULT_INIT

		bool init(float x, float y);

		static Point *pointWithLengthAndAngle(float length, float angle);

		Point *add(Point *value);
		Point *sub(Point *value);
		Point *scale(float scale);
		Point *rotate(float angle);
		Point *normalize();
		Point *invert();
		Point *perpendicular();
		Point *truncateLength(float maxLength);
		float dot(Point *value);
		bool isEqualTo(Point *value);
		void clone(Point *poing);
		void set(float x, float y);

		static float distanceBetween(Point *p1, Point *p2);
		static float angleBetween(Point *p1, Point *p2);
		static Point *interpolateBetween(Point *p1, Point *p2, float ratio);

		virtual Point *copy() override;

		float length();
		float lengthSquared();
		float angle();
		bool isOrigin();

	public:
		float x, y;
	};
}

#endif
