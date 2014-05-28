#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace cut
{
	void add3(const Vector3* first, const Vector3* second, Vector3* result);
	void add4(const Vector4* first, const Vector4* second, Vector4* result);
	
	void sub3(const Vector3* first, const Vector3* second, Vector3* result);
	void sub4(const Vector4* first, const Vector4* second, Vector4* result);
	
	void mul3(const Vector3* first, const Vector3* second, Vector3* result);

	void div3(const Vector3* first, const Vector3* second, Vector3* result);

	float dot3(const Vector3* first, const Vector3* second);
	float dot4(const Vector4* first, const Vector4* second);

	void cross3(const Vector3* first, const Vector3* second, Vector3* result);
	void cross4(const Vector4* first, const Vector4* second, Vector4* result);
	
	void normalise3(const Vector3* first, Vector3* result);

	float length3(const Vector3* first);

	void lerp3(const Vector3* from, const Vector3* to, float t, Vector3* result);

	float linePlaneCoefficient(const Vector3* linePoint, const Vector3* lineDir, const Vector3* planeNormal, const Vector3* planePoint);
}
	
#endif
