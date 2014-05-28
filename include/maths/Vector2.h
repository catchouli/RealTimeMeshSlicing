#ifndef __VECTOR2_H__
#define __VECTOR2_H__

namespace cut
{
	typedef union
	{
		struct
		{
			float x, y;
		};
		float data[2];
	} Vector2;
}
	
#endif /* __VECTOR2_H__ */
