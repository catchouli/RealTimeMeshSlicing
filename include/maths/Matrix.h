#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Vector3.h"
#include "Vector4.h"

#include "Matrix3.h"
#include "Matrix4.h"

namespace cut
{
	void addMatrix3(Matrix3* first, Matrix3* second, Matrix3* result);
	void multVector3(Matrix3* first, Vector3* second, Vector3* result);
	void multMatrix3(Matrix3* first, Matrix3* second, Matrix3* result);

	void addMatrix4(Matrix4* first, Matrix4* second, Matrix4* result);
	void multVector4(Matrix4* first, Vector4* second, Vector4* result);
	void multMatrix4(Matrix4* first, Matrix4* second, Matrix4* result);

	void multMatrix(float* first, int w1, int h1,
					float* second, int w2, int h2,
					float* result);
					
	void rotationX(Matrix4* result, double rot);	
	void rotationY(Matrix4* result, double rot);
	void rotationZ(Matrix4* result, double rot);
}

#endif /* __MATRIX_H__ */
