#include "geom/Rectangle.h"
#include "geom/Matrix.h"
#include "display/DisplayObject.h"
#include "display/DisplayObjectContainer.h"
#include "display/Stage.h"
#include "display/BlendMode.h"
#include "util/Console.h"

#define MAX_DISPLAY_TREE_DEPTH 32

namespace xGame2D
{
	DisplayObject::DisplayObject()
		: x(0.0f)
		, y(0.0f)
		, pivotX(0.0f)
		, pivotY(0.0f)
		, scaleX(1.0f)
		, scaleY(1.0f)
		, skewX(0.0f)
		, skewY(0.0f)
		, rotation(0.0f)
		, alpha(1.0f)
		, blendMode(BlendModeAuto)
		, visible(true)
		, touchable(true)
		, orientationChanges(false)
		, parent(nullptr)
		, transformationMatrix(Object::generate<Matrix>())
		, name("")
	{
	}

	DisplayObject::~DisplayObject()
	{
		transformationMatrix->release();
	}

	void DisplayObject::removeFromParent()
	{
		if (parent) parent->removeChild(this);
	}

	void DisplayObject::alignPivotToCenter()
	{
		alignPivot(HAlignCenter, VAlignCenter);
	}

	void DisplayObject::alignPivot(HAlign hAlign, VAlign vAlign)
	{
		Rectangle *bounds = boundsInSpace(this);
		orientationChanges = true;
		switch (hAlign)
		{
			case HAlignLeft:
				pivotX = bounds->x;
				break;
			case HAlignCenter:
				pivotX = bounds->x + bounds->width / 2.0f;
				break;
			case HAlignRight:
				pivotX = bounds->x + bounds->width;
				break;
			default:
				Console::Error("Invalid horizontal alignment");
				break;
		}
		switch (vAlign)
		{
			case VAlignTop:
				pivotY = bounds->y;
				break;
			case VAlignCenter:
				pivotY = bounds->y + bounds->height / 2.0f;
				break;
			case VAlignBottom:
				pivotY = bounds->y + bounds->height;
				break;
			default:
				Console::Error("Invalid vertical alignment");
				break;
		}
	}

	Matrix *DisplayObject::transformationMatrixToSpace(DisplayObject *targetSpace)
	{
		if (targetSpace == this)
		{
			return Object::create<Matrix>();
		}
		else if (targetSpace == parent || (!targetSpace && !parent))
		{
			Matrix *value = getTransformationMatrix()->copy();
			value->autorelease();
			return value;
		}
		else if (!targetSpace || targetSpace == getBase())
		{
			Matrix *selfMatrix = Object::create<Matrix>();
			DisplayObject *currentObject = this;
			while (currentObject != targetSpace)
			{
				selfMatrix->appendMatrix(currentObject->getTransformationMatrix());
				currentObject = currentObject->parent;
			}
			return selfMatrix;
		}
		else if (targetSpace->parent == this)
		{
			Matrix *targetMatrix = targetSpace->getTransformationMatrix()->copy();
			targetSpace->autorelease();
			targetMatrix->invert();
			return targetMatrix;
		}
		static DisplayObject *ancestors[MAX_DISPLAY_TREE_DEPTH];
		int count = 0;
		DisplayObject *commonParent = nullptr;
		DisplayObject *currentObject = this;
		while (currentObject && count < MAX_DISPLAY_TREE_DEPTH)
		{
			ancestors[count++] = currentObject;
			currentObject = currentObject->parent;
		}
		currentObject = targetSpace;
		while (currentObject && !commonParent)
		{
			for (int i = 0; i < count; i++)
			{
				if (currentObject == ancestors[i])
				{
					commonParent = ancestors[i];
					break;
				}
			}
			currentObject = currentObject->parent;
		}
		if (!commonParent)
			Console::Error("Object not connected to target");
		Matrix *selfMatrix = Object::create<Matrix>();
		currentObject = this;
		while (currentObject != commonParent)
		{
			selfMatrix->appendMatrix(currentObject->getTransformationMatrix());
			currentObject = currentObject->parent;
		}
		Matrix *targetMatrix = Object::create<Matrix>();
		currentObject = targetSpace;
		while (currentObject && currentObject != commonParent)
		{
			targetMatrix->appendMatrix(currentObject->getTransformationMatrix());
			currentObject = currentObject->parent;
		}
		targetMatrix->invert();
		selfMatrix->appendMatrix(targetMatrix);
		return selfMatrix;
	}
    
	Point *DisplayObject::localToGlobal(Point *localPoint)
	{
		Matrix *matrix = transformationMatrixToSpace(getBase());
		return matrix->transform(localPoint);
	}

	Point *DisplayObject::globalToLocal(Point *globalPoint)
	{
		Matrix *matrix = transformationMatrixToSpace(getBase());
		matrix->invert();
		return matrix->transform(globalPoint);
	}

	DisplayObject *DisplayObject::hitTestPoint(Point *localPoint)
	{
		if (!visible || !touchable)
			return nullptr;
		if (boundsInSpace(this)->containsPoint(localPoint))
			return this;
		else
			return nullptr;
	}

	void DisplayObject::setX(float value)
	{
		if (value != x)
		{
			x = value;
			orientationChanges = true;
		}
	}

	float DisplayObject::getX()
	{
		return x;
	}

	void DisplayObject::setY(float value)
	{
		if (value != y)
		{
			y = value;
			orientationChanges = true;
		}
	}

	float DisplayObject::getY()
	{
		return y;
	}

	void DisplayObject::setPivotX(float value)
	{
		if (value != pivotX)
		{
			pivotX = value;
			orientationChanges = true;
		}
	}

	float DisplayObject::getPivotX()
	{
		return pivotX;
	}

	void DisplayObject::setPivotY(float value)
	{
		if (value != pivotY)
		{
			pivotY = value;
			orientationChanges = true;
		}
	}

	float DisplayObject::getPivotY()
	{
		return pivotY;
	}

	void DisplayObject::setScale(float value)
	{
		if (value != scaleX || value != scaleY)
		{
			scaleX = scaleY = value;
			orientationChanges = true;
		}
	}

	float DisplayObject::getScale()
	{
		if (!IsFloatEqual(scaleX, scaleX))
			Console::Log("WARNING: Scale is not uniform, Use the approriate scaleX and scaleY properties.");
		return scaleX;
	}

	void DisplayObject::setScaleX(float value)
	{
		if (value != scaleX)
		{
			scaleX = value;
			orientationChanges = true;
		}
	}

	float DisplayObject::getScaleX()
	{
		return scaleX;
	}

	void DisplayObject::setScaleY(float value)
	{
		if (value != scaleY)
		{
			scaleY = value;
			orientationChanges = true;
		}
	}

	float DisplayObject::getScaleY()
	{
		return scaleY;
	}

	void DisplayObject::setSkewX(float value)
	{
		if (value != skewX)
		{
			skewX = value;
			orientationChanges = true;
		}
	}

	float DisplayObject::getSkewX()
	{
		return skewX;
	}

	void DisplayObject::setSkewY(float value)
	{
		if (value != skewY)
		{
			skewY = value;
			orientationChanges = true;
		}
	}

	float DisplayObject::getSkewY()
	{
		return skewY;
	}

	float DisplayObject::getWidth()
	{
		return boundsInSpace(parent)->width;
	}

	void DisplayObject::setWidth(float value)
	{
		setScaleX(1.0f);
		auto actualWidth = getWidth();
		if (actualWidth != 0.0f)
			setScaleX(value / actualWidth);
	}

	float DisplayObject::getHeight()
	{
		return boundsInSpace(parent)->height;
	}

	void DisplayObject::setHeight(float value)
	{
		setScaleY(1.0f);
		auto actualHeight = getHeight();
		if (actualHeight != 0.0f)
			setScaleY(value / actualHeight);
	}

	float DisplayObject::getRotation()
	{
		return rotation;
	}

	void DisplayObject::setRotation(float value)
	{
		value = fmod(value, TWO_PI);
		if (value < -PI) value += TWO_PI;
		if (value > PI) value -= TWO_PI;
		rotation = value;
		orientationChanges = true;
	}

	float DisplayObject::getAlpha()
	{
		return alpha;
	}

	void DisplayObject::setAlpha(float value)
	{
		alpha = CLAMP(value, 0.0f, 1.0f);
	}

	bool DisplayObject::getVisible()
	{
		return visible;
	}

	void DisplayObject::setVisible(bool value)
	{
		visible = value;
	}

	bool DisplayObject::getTouchable()
	{
		return touchable;
	}

	void DisplayObject::setTouchable(bool value)
	{
		touchable = value;
	}

	Rectangle *DisplayObject::getBounds()
	{
		return boundsInSpace(parent);
	}

	DisplayObjectContainer *DisplayObject::getParent()
	{
		return parent;
	}

	DisplayObject *DisplayObject::getRoot()
	{
		auto *current = this;
		while (current->parent)
		{
			if (dynamic_cast<Stage *>(current->parent)) return current;
			else current = current->parent;
		}
		return nullptr;
	}

	DisplayObject *DisplayObject::getBase()
	{
		auto *current = this;
		while (current->parent) current = current->parent;
		return current;
	}

	Stage *DisplayObject::getStage()
	{
		return dynamic_cast<Stage *>(getBase());
	}

	Matrix *DisplayObject::getTransformationMatrix()
	{
		if (orientationChanges)
		{
			orientationChanges = false;
			if (skewX == 0.0f && skewY == 0.0f)
			{
				if (rotation == 0.0f)
				{
					transformationMatrix->set(scaleX, 0.0f, 0.0f, scaleY, x - pivotX * scaleX, y - pivotY * scaleY);
				}
				else
				{
					auto cos = cosf(rotation);
					auto sin = sinf(rotation);
					auto a = scaleX * cos;
					auto b = scaleX * sin;
					auto c = scaleY * -sin;
					auto d = scaleY * cos;
					auto tx = x - pivotX * a - pivotY * c;
					auto ty = y - pivotX * b - pivotY * d;
					transformationMatrix->set(a, b, c, d, tx, ty);
				}
			}
			else
			{
				transformationMatrix->identity();
				transformationMatrix->scale(scaleX, scaleY);
				transformationMatrix->skew(skewX, skewY);
				transformationMatrix->rotate(rotation);
				transformationMatrix->translate(x, y);
				if (pivotX != 0.0f || pivotY != 0.0f)
				{
					transformationMatrix->tx = x - transformationMatrix->a * pivotX - transformationMatrix->c * pivotY;
					transformationMatrix->ty = y - transformationMatrix->b * pivotX - transformationMatrix->d * pivotY;
				}
			}
		}
		return transformationMatrix;
	}

	void DisplayObject::setTransformationMatrix(Matrix *matrix)
	{
		static const float PI_Q = PI / 4.0f;
		orientationChanges = false;
		transformationMatrix->clone(matrix);
		pivotX = 0.0f;
		pivotY = 0.0f;
		x = matrix->tx;
		y = matrix->ty;
		skewX = (matrix->d == 0.0f) ? PI_HALF * Sign(static_cast<int32_t>(-matrix->c)) : atanf(-matrix->c / matrix->d);
		skewY = (matrix->a == 0.0f) ? PI_HALF * Sign(static_cast<int32_t>(matrix->b)) : atanf(matrix->b / matrix->a);
		scaleY = (skewX > -PI_Q && skewX < PI_Q) ? matrix->d / cosf(skewX) : -matrix->c / sinf(skewX);
		scaleX = (skewY > -PI_Q && skewY < PI_Q) ? matrix->a / cosf(skewY) : matrix->b / sinf(skewY);
		if (IsFloatEqual(skewX, skewY))
		{
			rotation = skewX;
			skewX = skewY = 0.0f;
		}
		else
		{
			rotation = 0.0f;
		}
	}

	void DisplayObject::setName(std::string &value)
	{
		name = value;
	}

	std::string DisplayObject::getName()
	{
		return name;
	}

	uint32_t DisplayObject::getBlendMode()
	{
		return blendMode;
	}

	void DisplayObject::setBlendMode(uint32_t value)
	{
		blendMode = value;
	}

	bool DisplayObject::hasVisibleArea()
	{
		return alpha != 0.0f && visible && scaleX != 0.0f && scaleY != 0.0f;
	}
}