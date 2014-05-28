#include "maths/Vector.h"

#include <math.h>

namespace cut
{
	void add3(const Vector3* first, const Vector3* second, Vector3* result)
	{
		result->x = first->x + second->x;
		result->y = first->y + second->y;
		result->z = first->z + second->z;
	}
	
	void add4(const Vector4* first, const Vector4* second, Vector4* result)
	{
		result->x = first->x + second->x;
		result->y = first->y + second->y;
		result->z = first->z + second->z;
		result->w = first->w + second->w;
	}
		
	void sub3(const Vector3* first, const Vector3* second, Vector3* result)
	{
		result->x = first->x - second->x;
		result->y = first->y - second->y;
		result->z = first->z - second->z;
	}
	
	void sub4(const Vector4* first, const Vector4* second, Vector4* result)
	{
		result->x = first->x - second->x;
		result->y = first->y - second->y;
		result->z = first->z - second->z;
		result->w = first->w - second->w;
	}
	
	void mul3(const Vector3* first, const Vector3* second, Vector3* result)
	{
		result->x = first->x * second->x;
		result->y = first->y * second->y;
		result->z = first->z * second->z;
	}

	void div3(const Vector3* first, const Vector3* second, Vector3* result)
	{
		result->x = first->x / second->x;
		result->y = first->y / second->y;
		result->z = first->z / second->z;
	}
		
	float dot3(const Vector3* first, const Vector3* second)
	{
		int i;
		float dot = 0;

		for (i = 0; i < 3; ++i)
		{
			dot += first->data[i] * second->data[i];
		}

		return dot;
	}

	float dot4(const Vector4* first, const Vector4* second)
	{
		int i;
		float dot = 0;

		for (i = 0; i < 4; ++i)
		{
			dot += first->data[i] * second->data[i];
		}

		return dot;
	}

	void cross3(const Vector3* first, const Vector3* second, Vector3* result)
	{
		result->x = (first->y * second->z) - (first->z * second->y);
		result->y = (first->z * second->x) - (first->x * second->z);
		result->z = (first->x * second->y) - (first->y * second->x);
	}

	void cross4(const Vector4* first, const Vector4* second, Vector4* result)
	{
		result->x = (first->y * second->z) - (first->z * second->y);
		result->y = (first->z * second->x) - (first->x * second->z);
		result->z = (first->x * second->y) - (first->y * second->x);
	}

	void normalise3(const Vector3* first, Vector3* result)
	{
		float length = length3(first);
		
		result->x = first->x / length;
		result->y = first->y / length;
		result->z = first->z / length;
	}

	float length3(const Vector3* first)
	{
		// Calculate length with pythagoras
		float lengthSquared = first->x * first->x + first->y * first->y + first->z * first->z;
		float length = sqrt(lengthSquared);

		return length;
	}

	void lerp3(const Vector3* from, const Vector3* to, float t, Vector3* result)
	{
		result->x = from->x + t * (to->x - from->x);
		result->y = from->y + t * (to->y - from->y);
		result->z = from->z + t * (to->z - from->z);
	}

	float linePlaneCoefficient(const Vector3* linePoint, const Vector3* lineDir, const Vector3* planeNormal, const Vector3* planePoint)
	{
		Vector3 diff;
		float dot1;
		float dot2;
		float result;

		sub3(planePoint, linePoint, &diff);
		dot1 = dot3(planeNormal, &diff);
		dot2 = dot3(planeNormal, lineDir);
		result = dot1 / dot2;

		return result;
	}
}