#ifndef __VECTOR4_H__
#define __VECTOR4_H__

namespace cut
{
	typedef union
	{
		struct
		{
			float x, y, z, w;
		};
		float data[4];
	} Vector4;
}
	
#endif /* __VECTOR4_H__ */
