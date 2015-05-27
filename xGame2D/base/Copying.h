#ifndef __X_COPYING_H__
#define __X_COPYING_H__

namespace xGame2D
{
	class Copying
	{
	public:
		virtual Copying *copy() = 0;

		virtual ~Copying()
		{
		};
	};
}

#endif
