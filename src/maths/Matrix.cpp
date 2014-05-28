#include "maths/Matrix.h"

#include <math.h>

namespace cut
{
	void addMatrix3(Matrix3* first, Matrix3* second, Matrix3* result)
	{
		int i;
		for (i = 0; i < 3*3; ++i)
		{
			result->data[i] = first->data[i] + second->data[i];
		}
	}

	void multVector3(Matrix3* first, Vector3* second, Vector3* result)
	{
		multMatrix(first->data, 3, 3, second->data, 1, 3, result->data);
	}

	void multMatrix3(Matrix3* first, Matrix3* second, Matrix3* result)
	{
		multMatrix(first->data, 3, 3, second->data, 3, 3, result->data);
	}

	void addMatrix4(Matrix4* first, Matrix4* second, Matrix4* result)
	{
		int i;
		for (i = 0; i < 4*4; ++i)
		{
			result->data[i] = first->data[i] + second->data[i];
		}
	}

	void multVector4(Matrix4* first, Vector4* second, Vector4* result)
	{
		multMatrix(first->data, 4, 4, second->data, 1, 4, result->data);
	}

	void multMatrix4(Matrix4* first, Matrix4* second, Matrix4* result)
	{
		multMatrix(first->data, 4, 4, second->data, 4, 4, result->data);
	}

	void multMatrix(float* first, int w1, int h1,
					float* second, int w2, int h2,
					float* result)
	{
		int x;
		int y;
		int i;

		// If the matrices aren't compatible, return
		if (w1 != h2)
			return;

		for (y = 0; y < h1; ++y)
		{
			for (x = 0; x < w2; ++x)
			{
				result[x * h1 + y] = 0;

				for (i = 0; i < w1; ++i)
				{
					result[x * h1 + y] += first[y + i*h1] * second[i + x*h2];
				}
			}
		}
	}
	
	void rotationX(Matrix4* result, double rot)
	{
		float sinRot = (float)sin(rot);
		float cosRot = (float)cos(rot);
		
		// Construct rotation matrix
		result->data[0] = 1;
		result->data[1] = 0;
		result->data[2] = 0;
		result->data[3] = 0;
		
		result->data[4] = 0;
		result->data[5] = cosRot;
		result->data[6] = -sinRot;
		result->data[7] = 0;
		
		result->data[8] = 0;
		result->data[9] = sinRot;
		result->data[10] = cosRot;
		result->data[11] = 0;
		
		result->data[12] = 0;
		result->data[13] = 0;
		result->data[14] = 0;
		result->data[15] = 1;
	};
	
	void rotationY(Matrix4* result, double rot)
	{
		float sinRot = (float)sin(rot);
		float cosRot = (float)cos(rot);
		
		// Construct rotation matrix
		result->data[0] = cosRot;
		result->data[1] = 0;
		result->data[2] = sinRot;
		result->data[3] = 0;
		
		result->data[4] = 0;
		result->data[5] = 1;
		result->data[6] = 0;
		result->data[7] = 0;
		
		result->data[8] = -sinRot;
		result->data[9] = 0;
		result->data[10] = cosRot;
		result->data[11] = 0;
		
		result->data[12] = 0;
		result->data[13] = 0;
		result->data[14] = 0;
		result->data[15] = 1;
	};
	
	void rotationZ(Matrix4* result, double rot)
	{
		float sinRot = (float)sin(rot);
		float cosRot = (float)cos(rot);
		
		// Construct rotation matrix
		result->data[0] = cosRot;
		result->data[1] = -sinRot;
		result->data[2] = 0;
		result->data[3] = 0;
		
		result->data[4] = sinRot;
		result->data[5] = cosRot;
		result->data[6] = 0;
		result->data[7] = 0;
		
		result->data[8] = 0;
		result->data[9] = 0;
		result->data[10] = 1;
		result->data[11] = 0;
		
		result->data[12] = 0;
		result->data[13] = 0;
		result->data[14] = 0;
		result->data[15] = 1;
	};
}
