#ifndef __MATRIX3_H__
#define __MATRIX3_H__

#include "maths/Vector3.h"

namespace cut
{
	typedef union
	{
		struct
		{
			Vector3 a, b, c;
		} value;
		float data[3*3];
	} Matrix3;
}

#endif /* __MATRIX3_H__ */
