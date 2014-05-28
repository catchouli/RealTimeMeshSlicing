#ifndef __VECTOR3_H__
#define __VECTOR3_H__

namespace cut
{
	typedef union
	{
		struct
		{
			float x, y, z;
		};
		float data[3];
	} Vector3;
}
	
#endif /* __VECTOR3_H__ */
