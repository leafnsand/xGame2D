#ifndef __X_RECTANGLE_H__
#define __X_RECTANGLE_H__

#include "base/Object.h"
#include "base/Copying.h"

namespace xGame2D
{
	class Point;

	class Rectangle : public Object, Copying
	{
	constructor:
		Rectangle();
		~Rectangle();

	public:
		OVERRIDE_DEFAULT_INIT
		bool init(float x, float y, float width, float height);

		bool contains(float x, float y);
		bool containsPoint(Point *point);
		bool containsRectangle(Rectangle *rectangle);
		bool intersectsRectangle(Rectangle *rectangle);
		Rectangle *intersectionWithRectangle(Rectangle *rectangle);
		Rectangle *uniteWithRectangle(Rectangle *rectangle);
		void inflateX(float x, float y);
		void set(float x, float y, float width, float height);
		void setEmpty();
		void clone(Rectangle *rectangle);
		bool isEqualTo(Rectangle *rectangle);
		void normalize();

		virtual Rectangle *copy() override;

		float top();
		void setTop(float top);
		float bottom();
		void setBottom(float bottom);
		float left();
		void setLeft(float left);
		float right();
		void setRight(float right);
		Point *topLeft();
		void setTopLeft(Point *topLeft);
		Point *topRight();
		Point *bottomLeft();
		Point *bottomRight();
		void setBottomRight(Point *bottomRight);
		Point *size();
		void setSize(Point *size);
		bool isEmpty();

	public:
		float x, y, width, height;
	};
}

#endif