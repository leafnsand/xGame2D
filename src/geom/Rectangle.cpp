#include "geom/Rectangle.h"
#include "geom/Point.h"

namespace xGame2D
{
	Rectangle::Rectangle()
		: x(0.0f)
		, y(0.0f)
		, width(0.0f)
		, height(0.0f)
	{
	}

	Rectangle::~Rectangle()
	{
	}

	bool Rectangle::init(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		return true;
	}

	bool Rectangle::contains(float x, float y)
	{
		return x >= this->x && y >= this->y && x <= this->x + width && y <= this->y + height;
	}

	bool Rectangle::containsPoint(Point *point)
	{
		return contains(point->x, point->y);
	}

	bool Rectangle::containsRectangle(Rectangle *rectangle)
	{
		return rectangle
			&& rectangle->x >= this->x && rectangle->x + rectangle->width <= this->x + this->width
			&& rectangle->y >= this->y && rectangle->y + rectangle->height <= this->y + this->height;
	}

	bool Rectangle::intersectsRectangle(Rectangle *rectangle)
	{
		if (!rectangle) return false;
		auto rX = rectangle->x;
		auto rY = rectangle->y;
		auto rWidth = rectangle->width;
		auto rHeight = rectangle->height;
		auto outside =
			(rX <= x && rX + rWidth <= x) || (rX >= x + height && rX + rWidth >= x + width) ||
			(rY <= y && rY + rHeight <= y) || (rY >= y + height && rY + rHeight >= y + height);
		return !outside;
	}

	Rectangle *Rectangle::intersectionWithRectangle(Rectangle *rectangle)
	{
		if (!rectangle) return nullptr;
		auto left = MAX(x, rectangle->x);
		auto right = MIN(x + width, rectangle->x + rectangle->width);
		auto top = MAX(y, rectangle->y);
		auto bottom = MIN(y + height, rectangle->y + rectangle->height);
		if (left > right || top > bottom)
			return Object::create<Rectangle>();
		else
			return Object::create<Rectangle>(left, top, right - left, bottom - top);
	}

	Rectangle *Rectangle::uniteWithRectangle(Rectangle *rectangle)
	{
		if (!rectangle)
		{
			auto value = copy();
			value->autorelease();
			return value;
		}
		auto left = MIN(x, rectangle->x);
		auto right = MAX(x + width, rectangle->x + rectangle->width);
		auto top = MIN(y, rectangle->y);
		auto bottom = MAX(y + height, rectangle->y + rectangle->height);
		return Object::create<Rectangle>(left, top, right - left, bottom - top);
	}

	void Rectangle::inflateX(float x, float y)
	{
		this->x -= x;
		this->width += 2 * x;
		this->y -= y;
		this->height += 2 * y;
	}

	void Rectangle::set(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	void Rectangle::setEmpty()
	{
		x = y = width = height = 0;
	}

	void Rectangle::clone(Rectangle *rectangle)
	{
		x = rectangle->x;
		y = rectangle->y;
		width = rectangle->width;
		height = rectangle->height;
	}

	bool Rectangle::isEqualTo(Rectangle *rectangle)
	{
		if (rectangle == this) return true;
		else if (!rectangle) return false;
		else return IsFloatEqual(x, rectangle->x) && IsFloatEqual(y, rectangle->y) 
			&& IsFloatEqual(width, rectangle->width) && IsFloatEqual(height, rectangle->height);
	}

	void Rectangle::normalize()
	{
		if (width < 0.0f)
		{
			width = -width;
			x -= width;
		}
		if (height < 0.0f)
		{
			height = -height;
			y -= height;
		}
	}

	Rectangle *Rectangle::copy()
	{
		return Object::create<Rectangle>(x, y, width, height);
	}

	float Rectangle::top()
	{
		return y;
	}

	void Rectangle::setTop(float top)
	{
		y = top;
	}

	float Rectangle::bottom()
	{
		return y + height;
	}

	void Rectangle::setBottom(float bottom)
	{
		height = bottom - y;
	}

	float Rectangle::left()
	{
		return x;
	}

	void Rectangle::setLeft(float left)
	{
		x = left;
	}

	float Rectangle::right()
	{
		return x + width;
	}

	void Rectangle::setRight(float right)
	{
		width = right - x;
	}

	Point *Rectangle::topLeft()
	{
		return Object::create<Point>(x, y);
	}

	void Rectangle::setTopLeft(Point *topLeft)
	{
		x = topLeft->x;
		y = topLeft->y;
	}

	Point *Rectangle::topRight()
	{
		return Object::create<Point>(x + width, y);
	}

	Point *Rectangle::bottomLeft()
	{
		return Object::create<Point>(x, y + height);
	}

	Point *Rectangle::bottomRight()
	{
		return Object::create<Point>(x + width, y + height);
	}

	void Rectangle::setBottomRight(Point *bottomRight)
	{
		width = bottomRight->x - x;
		height = bottomRight->y - y;
	}

	Point *Rectangle::size()
	{
		return Object::create<Point>(width, height);
	}

	void Rectangle::setSize(Point *size)
	{
		width = size->x;
		height = size->y;
	}

	bool Rectangle::isEmpty()
	{
		return width == 0.0f || height == 0.0f;
	}
}