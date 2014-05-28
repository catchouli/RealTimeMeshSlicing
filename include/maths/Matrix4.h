#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include "maths/Vector4.h"

namespace cut
{
	typedef union
	{
		struct
		{
			Vector4 a, b, c, d;
		};
		float data[4*4];
	} Matrix4;
}

#endif /* __MATRIX4_H__ */
