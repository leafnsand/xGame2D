#ifndef __X_DISPLAYOBJECT_H__
#define __X_DISPLAYOBJECT_H__

#include "base/Object.h"
#include "base/Math.h"
#include "util/Align.h"
#include <string>

namespace xGame2D
{
	class DisplayObjectContainer;
	class Matrix;
	class Point;
	class Rectangle;
	class RenderSupport;
	class Stage;

	class DisplayObject : public Object
	{
	constructor:
		DisplayObject();
		virtual ~DisplayObject();

	public:
		virtual void render(RenderSupport *support) = 0;
		void removeFromParent();
		void alignPivotToCenter();
		void alignPivot(HAlign hAlign, VAlign vAlign);
		Matrix *transformationMatrixToSpace(DisplayObject *targetSpace);
		virtual Rectangle *boundsInSpace(DisplayObject *targetSpace) = 0;
		Point *localToGlobal(Point *localPoint);
		Point *globalToLocal(Point *globalPoint);
		virtual DisplayObject *hitTestPoint(Point *localPoint);
		// event

		void setX(float value);
		float getX();
		void setY(float value);
		float getY();
		void setPivotX(float value);
		float getPivotX();
		void setPivotY(float value);
		float getPivotY();
		void setScale(float value);
		float getScale();
		void setScaleX(float value);
		float getScaleX();
		void setScaleY(float value);
		float getScaleY();
		void setSkewX(float value);
		float getSkewX();
		void setSkewY(float value);
		float getSkewY();
		float getWidth();
		void setWidth(float value);
		float getHeight();
		void setHeight(float value);
		float getRotation();
		void setRotation(float value);
		float getAlpha();
		virtual void setAlpha(float value);
		bool getVisible();
		void setVisible(bool value);
		bool getTouchable();
		void setTouchable(bool value);
		Rectangle *getBounds();
		DisplayObjectContainer *getParent();
		DisplayObject *getRoot();
		DisplayObject *getBase();
		Stage *getStage();
		Matrix *getTransformationMatrix();
		void setTransformationMatrix(Matrix *matrix);
		void setName(std::string &value);
		std::string getName();
		uint32_t getBlendMode();
		void setBlendMode(uint32_t value);
		bool hasVisibleArea();

	protected:
		float x, y, pivotX, pivotY, scaleX, scaleY, skewX, skewY, rotation, alpha;
		uint32_t blendMode;
		bool visible, touchable, orientationChanges;
		DisplayObjectContainer *parent;
		Matrix *transformationMatrix;
		// lastTouchTimestamp
		std::string name;
		// filter

		friend class DisplayObjectContainer;
	};
}

#endif
