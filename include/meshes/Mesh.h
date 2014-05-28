#ifndef __MESH_H__
#define __MESH_H__

#include "maths/Vector.h"

namespace cut
{
	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		void createCube();
		void loadObj(const char* filename);
		void loadObjOld(const char* filename);

		void cut(Mesh* left, Mesh* right, Vector3 planePoint, Vector3 planeNormal);

		Vector3* vertices;
		int* indices;
		Vector3* vertexNormals;
		Vector2* texCoords;
	
		int vertexCount;
		int indexCount;
	};
}

#endif /* __MESH_H__ */