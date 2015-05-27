#include "display/DisplayObjectContainer.h"
#include "display/DisplayObject.h"
#include "render/RenderSupport.h"
#include "geom/Matrix.h"
#include "geom/Rectangle.h"
#include "geom/Point.h"
#include "util/Console.h"

namespace xGame2D
{
	DisplayObjectContainer::DisplayObjectContainer(): 
		touchGroup(false)
	{
	}

	DisplayObjectContainer::~DisplayObjectContainer()
	{
		removeAllChildren();
	}

	bool DisplayObjectContainer::init()
	{
		children.reserve(16);
		return true;
	}

	void DisplayObjectContainer::addChild(DisplayObject *child)
	{
		addChildAt(child, static_cast<int32_t>(children.size()));
	}

	void DisplayObjectContainer::addChildAt(DisplayObject *child, int32_t index)
	{
		X_ASSERT((index >= 0 && index <= static_cast<int32_t>(children.size())) ,"Invalid child index");
		child->retain();
		child->removeFromParent();
		children.insert(children.begin() + MIN(static_cast<int32_t>(children.size()), index), child);
		child->parent = this;
		// dispatch event
	}

	bool DisplayObjectContainer::containsChild(DisplayObject *child)
	{
		while (child)
		{
			if (child == this) return true;
			else child = child->parent;
		}
		return false;
	}

	DisplayObject *DisplayObjectContainer::childAtIndex(int32_t index)
	{
		return children[index];
	}

	DisplayObject *DisplayObjectContainer::childByName(std::string &name)
	{
		for (auto child : children)
		{
			if (child->name == name)
			{
				return child;
			}
		}
		return nullptr;
	}

	int32_t DisplayObjectContainer::childIndex(DisplayObject *child)
	{
		auto size = static_cast<int32_t>(children.size());
		for (auto i = 0; i < size; ++i)
		{
			if (children[i] == child)
			{
				return i;
			}
		}
		return -1;
	}

	void DisplayObjectContainer::setIndexOf(int32_t index, DisplayObject *child)
	{
		auto currentIndex = childIndex(child);
		X_ASSERT(currentIndex != -1, "Not a child of this container");
		child->retain();
		children.erase(children.begin() + currentIndex);
		children.insert(children.begin() + index, child);
		child->release();
	}

	void DisplayObjectContainer::removeChild(DisplayObject *child)
	{
		auto index = childIndex(child);
		if (index != -1)
			removeChildAtIndex(index);
	}

	void DisplayObjectContainer::removeChildAtIndex(int32_t index)
	{
		X_ASSERT((index >= 0 && index <= static_cast<int32_t>(children.size())) ,"Invalid child index");
		auto child = children[index];
		child->parent = nullptr;
		child->release();
		children.erase(children.begin() + index);
	}

	void DisplayObjectContainer::removeAllChildren()
	{
		for (auto child : children)
		{
			child->parent = nullptr;
			child->release();
		}
		children.clear();
	}

	void DisplayObjectContainer::swapChild(DisplayObject *child1, DisplayObject *child2)
	{
		auto index1 = childIndex(child1);
		auto index2 = childIndex(child2);
		swapChildAtIndex(index1, index2);
	}

	void DisplayObjectContainer::swapChildAtIndex(int32_t index1, int32_t index2)
	{
		auto numChildren = static_cast<int32_t>(children.size());
		X_ASSERT(index1 >= 0 && index1 < numChildren && index2 >= 0 && index2 < numChildren, "Invalid child indices");
		auto temp = children[index1];
		children[index1] = children[index2];
		children[index2] = temp;
	}

	int32_t DisplayObjectContainer::getNumChildren()
	{
		return static_cast<int32_t>(children.size());
	}

	std::vector<DisplayObject *> DisplayObjectContainer::getChildren()
	{
		return children;
	}

	void DisplayObjectContainer::setChildren(std::vector<DisplayObject *> &value)
	{
		removeAllChildren();
		auto size = static_cast<int32_t>(children.size());
		for (auto i = 0; i < size; ++i)
		{
			addChild(value[i]);
		}
	}

	bool DisplayObjectContainer::getTouchGroup()
	{
		return touchGroup;
	}

	void DisplayObjectContainer::setTouchGroup(bool value)
	{
		touchGroup = value;
	}

	void DisplayObjectContainer::render(RenderSupport *support)
	{
		for (auto child : children)
		{
			if (child->hasVisibleArea())
			{
				support->pushState(child->getTransformationMatrix(), child->getAlpha(), child->getBlendMode());
				child->render(support);
				support->popState();
			}
		}
	}

	Rectangle *DisplayObjectContainer::boundsInSpace(DisplayObject *targetSpace)
	{
		auto numChildren = static_cast<int32_t>(children.size());
		if (numChildren == 0)
		{
			auto transformationMatrix = transformationMatrixToSpace(targetSpace);
			auto transformedPoint = transformationMatrix->transform(x, y);
			return Object::create<Rectangle>(transformedPoint->x, transformedPoint->y, 0.0f, 0.0f);
		}
		else if (numChildren == 1)
		{
			return children[0]->boundsInSpace(targetSpace);
		}
		else
		{
			float minX = FLT_MAX, maxX = -FLT_MAX, minY = FLT_MAX, maxY = -FLT_MAX;
			for (auto child : children)
			{
				auto childBounds = child->boundsInSpace(targetSpace);
				minX = MIN(minX, childBounds->x);
				maxX = MAX(maxX, childBounds->x + childBounds->width);
				minY = MIN(minY, childBounds->y);
				maxY = MAX(maxY, childBounds->y + childBounds->height);
			}
			return Object::create<Rectangle>(minX, minY, maxX - minX, maxY - minY);
		}
	}

	DisplayObject *DisplayObjectContainer::hitTestPoint(Point *localPoint)
	{
		if (!getVisible() || !getTouchGroup())
		{
			return nullptr;
		}
		for (auto child : children)
		{
			auto transformationMatrix = transformationMatrixToSpace(child);
			auto transformedPoint = transformationMatrix->transform(localPoint);
			auto target = child->hitTestPoint(transformedPoint);
			if (target)
			{
				return touchGroup ? this : target;
			}
		}
		return nullptr;
	}
}
