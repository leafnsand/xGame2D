#ifndef __X_DISPLAYOBJECTCONTAINER_H__
#define __X_DISPLAYOBJECTCONTAINER_H__

#include "display/DisplayObject.h"
#include <vector>

namespace xGame2D
{
	class DisplayObjectContainer : public DisplayObject
	{
	constructor:
		DisplayObjectContainer();
		~DisplayObjectContainer();
		
	public:
		virtual bool init() override;

		void addChild(DisplayObject *child);
		void addChildAt(DisplayObject *child, int32_t index);
		bool containsChild(DisplayObject *child);
		DisplayObject *childAtIndex(int32_t index);
		DisplayObject *childByName(std::string &name);
		int32_t childIndex(DisplayObject *child);
		void setIndexOf(int32_t index, DisplayObject *child);
		void removeChild(DisplayObject *child);
		void removeChildAtIndex(int32_t index);
		void removeAllChildren();
		void swapChild(DisplayObject *child1, DisplayObject *child2);
		void swapChildAtIndex(int32_t index1, int32_t index2);
		// sort child

		virtual void render(RenderSupport *support) override;
		virtual Rectangle *boundsInSpace(DisplayObject *targetSpace) override;
		virtual DisplayObject *hitTestPoint(Point *localPoint) override;

		int32_t getNumChildren();
		std::vector<DisplayObject *> getChildren();
		void setChildren(std::vector<DisplayObject *> &value);
		bool getTouchGroup();
		void setTouchGroup(bool value);

	protected:
		std::vector<DisplayObject *> children;
		bool touchGroup;
	};
}

#endif