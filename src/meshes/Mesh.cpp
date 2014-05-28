#include "meshes/Mesh.h"

#include <stdio.h>
#include <string>
#include <fstream>

namespace cut
{
	Mesh::Mesh()
		: vertexCount(0), indexCount(0), vertices(nullptr), indices(nullptr), vertexNormals(nullptr), texCoords(nullptr)
	{

	}

	Mesh::~Mesh()
	{
		delete[] vertices;
		delete[] indices;
		delete[] vertexNormals;
		delete[] texCoords;
	}

	void Mesh::createCube()
	{
		const int VERTEX_COUNT = 8;
		const int INDEX_COUNT = 36;

		static Vector3 VERTICES[VERTEX_COUNT] =
		{
			{  1.0f, -1.0f, -1.0f },
			{  1.0f, -1.0f,  1.0f },
			{ -1.0f, -1.0f,  1.0f },
			{ -1.0f, -1.0f, -1.0f },
			{  1.0f,  1.0f, -1.0f },
			{  1.0f,  1.0f,  1.0f },
			{ -1.0f,  1.0f,  1.0f },
			{ -1.0f,  1.0f, -1.0f }
		};

		static Vector3 NORMALS[VERTEX_COUNT] =
		{
			{  0.666667f, -0.666667f, -0.333333f },
			{  0.408248f, -0.408248f,  0.816497f },
			{ -0.666667f, -0.666667f,  0.333333f },
			{ -0.408248f, -0.408248f, -0.816497f },
			{  0.333333f,  0.666667f, -0.666667f },
			{  0.816497f,  0.408248f,  0.408248f },
			{ -0.333333f,  0.666667f,  0.666667f },
			{ -0.816497f,  0.408248f, -0.408248f }
		};

		static int INDICES[INDEX_COUNT] =
		{
			1, 0, 2, 2, 0, 3,
			7, 4, 6, 6, 4, 5,
			4, 0, 5, 5, 0, 1,
			5, 1, 6, 6, 1, 2,
			6, 2, 7, 7, 2, 3,
			0, 4, 3, 3, 4, 7
		};

		static Vector2 UVS[VERTEX_COUNT] =
		{
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f }
		};
		
		vertices = new Vector3[VERTEX_COUNT];
		memcpy(vertices, VERTICES, VERTEX_COUNT * sizeof(Vector3));

		vertexNormals = new Vector3[VERTEX_COUNT];
		memcpy(vertexNormals, NORMALS, VERTEX_COUNT * sizeof(Vector3));

		indices = new int[INDEX_COUNT];
		memcpy(indices, INDICES, INDEX_COUNT * sizeof(int));

		texCoords = new Vector2[VERTEX_COUNT];
		memcpy(UVS, texCoords, VERTEX_COUNT * sizeof(Vector2));

		vertexCount = VERTEX_COUNT;
		indexCount = INDEX_COUNT;
	}

	void Mesh::loadObj(const char* inputFile)
	{
		return loadObjOld(inputFile);

		// Obj specific datatypes
		union ObjVertex
		{
			struct
			{
				float x, y, z, w;
			};
			float vertices[4];
		};

		union ObjUv
		{
			struct
			{
				float u, v, w;
			};
			float uvs[3];
		};

		struct ObjFace
		{
			int vertexCount;
			bool hasTextureCoords;
			union
			{
				struct
				{
					int v1, v2, v3, v4;
					int t1, t2, t3, t4;
				};
				struct
				{
					int v[4];
					int t[4];
				};
			};
		};

		int objVertexCount = 0;
		int objUvCount = 0;
		int objFaceCount = 0;

		ObjVertex* objVertices;
		ObjUv* objUvs;
		ObjFace* objFaces;

		const int bufferSize = 1024;
		char buffer[bufferSize];

		// Open file
		FILE* fd = fopen(inputFile, "r");
		
		// If opening file failed, return immediately
		if (fd == NULL)
			return;
		
		// Initial pass - determine memory requirements for loading data
		while (fgets(buffer, bufferSize, fd) != NULL)
		{
			switch(buffer[0])
			{
			case 'v':
				if (buffer[1] == ' ')
					objVertexCount++;
				else if (buffer[1] == 't')
					objUvCount++;
				break;
			case 'f':
				objFaceCount++;
				break;
			}
		}

		// Allocate memory
		objVertices = new ObjVertex[objVertexCount];
		objFaces = new ObjFace[objFaceCount];
		objUvs = new ObjUv[objUvCount];

		// Second pass - load data from file
		int verticesLoaded = 0;
		int uvsLoaded = 0;
		int facesLoaded = 0;

		rewind(fd);
		while (fgets(buffer, bufferSize, fd) != NULL)
		{
			switch(buffer[0])
			{
			case 'v':
				if (buffer[1] == ' ')
				{
					// Load vertex
					int coordinatesLoaded = sscanf(buffer, "v %f %f %f %f", &objVertices[verticesLoaded].x, &objVertices[verticesLoaded].y, &objVertices[verticesLoaded].z, &objVertices[verticesLoaded].w);

					if (coordinatesLoaded >= 3)
					{
						// If no w provided, it defaults to 1.0
						if (coordinatesLoaded == 3)
							objVertices[verticesLoaded].w = 1.0f;

						verticesLoaded++;
					}
				}
				else if (buffer[1] == 't')
				{
					// Load uv
					int coordinatesLoaded = sscanf(buffer, "vt %f %f %f", &objUvs[uvsLoaded].u, &objUvs[uvsLoaded].v, &objUvs[uvsLoaded].w);

					if (coordinatesLoaded >= 2)
					{
						// If no w provided, it defaults to 1.0
						if (coordinatesLoaded == 2)
							objUvs[uvsLoaded].w = 1.0f;

						uvsLoaded++;
					}
				}
				break;
			case 'f':
				{
					// Load face
					int matched;

					if ((matched = sscanf(buffer, "f %d/%d %d/%d %d/%d %d/%d",
									&objFaces[facesLoaded].v1, &objFaces[facesLoaded].t1,
									&objFaces[facesLoaded].v2, &objFaces[facesLoaded].t2,
									&objFaces[facesLoaded].v3, &objFaces[facesLoaded].t3,
									&objFaces[facesLoaded].v4, &objFaces[facesLoaded].t4)) == 6 || matched == 8)
					{
						objFaces[facesLoaded].hasTextureCoords = true;
						objFaces[facesLoaded].vertexCount = matched/2;
						facesLoaded++;
					}
					else if ((matched = sscanf(buffer, "f %d/%d/%*d %d/%d/%*d %d/%d/%*d %d/%d/%*d",
											&objFaces[facesLoaded].v1, &objFaces[facesLoaded].t1,
											&objFaces[facesLoaded].v2, &objFaces[facesLoaded].t2,
											&objFaces[facesLoaded].v3, &objFaces[facesLoaded].t3,
											&objFaces[facesLoaded].v4, &objFaces[facesLoaded].t4)) == 6 || matched == 8)
					{
						objFaces[facesLoaded].hasTextureCoords = true;
						objFaces[facesLoaded].vertexCount = matched/2;
						facesLoaded++;
					}
					else if ((matched = sscanf(buffer, "f %d//%*d %d//%*d %d//%*d %d//%*d",
											&objFaces[facesLoaded].v1,
											&objFaces[facesLoaded].v2,
											&objFaces[facesLoaded].v3,
											&objFaces[facesLoaded].v4)) == 3 || matched == 4)
					{
						objFaces[facesLoaded].hasTextureCoords = false;
						objFaces[facesLoaded].vertexCount = matched;
						facesLoaded++;
					}
					else if ((matched = sscanf(buffer, "f %d %d %d %d",
											&objFaces[facesLoaded].v1,
											&objFaces[facesLoaded].v2,
											&objFaces[facesLoaded].v3,
											&objFaces[facesLoaded].v4)) == 3 || matched == 4)
					{
						objFaces[facesLoaded].hasTextureCoords = false;
						objFaces[facesLoaded].vertexCount = matched;
						facesLoaded++;
					}
				}
				break;
			}
		}
		
		// Close file descriptor
		fclose(fd);

		// Convert obj model to Mesh
		// Max number of vertices = face count * 4
		vertices = new Vector3[objFaceCount * 4];
		texCoords = new Vector2[objFaceCount * 4];
		indices = new int[objFaceCount*4];

		for (int i = 0; i < objFaceCount; ++i)
		{
			ObjFace* face = &objFaces[i];

			int newIndexCount = 0;
			int newIndices[4];
			
			// Copy new vertices
			for (int j = 0; j < face->vertexCount; ++j)
			{
				if (face->hasTextureCoords)
					memcpy(&texCoords[vertexCount], &objUvs[face->t[j]], sizeof(Vector2));

				memcpy(&vertices[vertexCount++], &objVertices[face->v[j]], sizeof(Vector3));

				newIndices[newIndexCount++] = face->v[j];
			}

			// Generate triangles
			indices[indexCount++] = newIndices[0];
			indices[indexCount++] = newIndices[1];
			indices[indexCount++] = newIndices[2];

			if (face->vertexCount > 3)
			{
				indices[indexCount++] = newIndices[3];
				indices[indexCount++] = newIndices[2];
				indices[indexCount++] = newIndices[1];
			}
		}

		// Clean up memory
		delete objVertices;
		delete objFaces;
		delete objUvs;
	}

	void Mesh::loadObjOld(const char* inputFile)
	{
		std::ifstream model(inputFile);

		if (model)
		{
			std::string line;

			int a, b, c, d;

			// Initial pass
			while (std::getline(model, line))
			{
				if (line.length() > 1)
				{
					switch (line[0])
					{
					case 'v':
						if (line[1] == ' ')
							vertexCount++;
						break;
					case 'f':
						if (sscanf_s(line.c_str(), "f %d %d %d %d", &a, &b, &c, &d) == 4
							|| sscanf_s(line.c_str(), "f %d/%*d %d/%*d %d/%*d %d/%*d", &a, &b, &c, &d) == 4
							|| sscanf_s(line.c_str(), "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &a, &b, &c, &d) == 4)
							indexCount += 3;
						indexCount += 3;
						break;
					default:
						break;
					}
				}
			}

			// Allocate memory
			vertices = new cut::Vector3[vertexCount];
			indices = new int[indexCount * 3];

			model.clear();
			model.seekg(std::ios::beg);

			int verticesRead = 0;
			int indicesRead = 0;

			int matches = 0;

			FILE* file = fopen("indices.txt", "w");
			// Second pass - read data
			while (std::getline(model, line))
			{ 
				matches = 0;
				if (line.length() > 1)
				{
					switch (line[0])
					{
					case 'v':
						if (sscanf_s(line.c_str(), "v %f %f %f", &vertices[verticesRead].x, &vertices[verticesRead].y, &vertices[verticesRead].z) == 3)
						{
							verticesRead++;
						}
						break;
					case 'f':
						if ((matches = sscanf_s(line.c_str(), "f %d/%*s %d/%*s %d/%*s %d/%*s", &a, &b, &c, &d)) >= 3)
						{
							indices[indicesRead] = a - 1;
							indices[indicesRead+1] = b - 1;
							indices[indicesRead+2] = c - 1;
							indicesRead += 3;

							if (matches > 3)
							{
								indices[indicesRead] = d - 1;
								indices[indicesRead+1] = c - 1;
								indices[indicesRead+2] = a - 1;
								indicesRead += 3;
							}
						}
						else if ((matches = sscanf_s(line.c_str(), "f %d/%*d %d/%*d %d/%*d %d/%*d", &a, &b, &c, &d)) >= 3)
						{
							indices[indicesRead] = a - 1;
							indices[indicesRead+1] = b - 1;
							indices[indicesRead+2] = c - 1;
							indicesRead += 3;

							if (matches > 3)
							{
								indices[indicesRead] = d - 1;
								indices[indicesRead+1] = c - 1;
								indices[indicesRead+2] = a - 1;
								indicesRead += 3;
							}
						}
						else if ((matches = sscanf_s(line.c_str(), "f %d %d %d %d", &a, &b, &c, &d)) >= 3)
						{
							indices[indicesRead] = b - 1;
							indices[indicesRead+1] = a - 1;
							indices[indicesRead+2] = c - 1;
							indicesRead += 3;

							if (matches > 3)
							{
								indices[indicesRead] = c - 1;
								indices[indicesRead+1] = a - 1;
								indices[indicesRead+2] = d - 1;
								indicesRead += 3;

								fprintf(file, "{ %d, %d, %d, %d, %d, %d },\n", b-1,a-1,c-1,c-1,a-1,d-1);
							}
						}
						break;
					default:
						printf("%c", line[0]);
					}
				}
			}
			fclose(file);

			// Calculate normals for each face
			Vector3* faceNormals = new Vector3[indexCount/3];
			vertexNormals = new Vector3[vertexCount];
			int* surroundingTriangles = new int[vertexCount];

			memset(faceNormals, 0, (indexCount/3) * sizeof(Vector3));
			memset(vertexNormals, 0, vertexCount * sizeof(Vector3));
			memset(surroundingTriangles, 0, vertexCount * sizeof(int));

			int faceCount = indexCount/3;

			for (int i = 0; i < faceCount; ++i)
			{
				Vector3 edge1;
				Vector3 edge2;
				
				int i1 = indices[i*3 +0];
				int i2 = indices[i*3 +1];
				int i3 = indices[i*3 +2];
				
				Vector3* v1 = &vertices[i1];
				Vector3* v2 = &vertices[i2];
				Vector3* v3 = &vertices[i3];
				
				// Calculate edges
				sub3(v3, v1, &edge1);
				sub3(v2, v1, &edge2);

				// Calculate face normal
				cross3(&edge1, &edge2, &faceNormals[i]);
				normalise3(&faceNormals[i], &faceNormals[i]);

				// Increment vertex normals
				add3(&vertexNormals[i1], &faceNormals[i], &vertexNormals[i1]);
				add3(&vertexNormals[i2], &faceNormals[i], &vertexNormals[i2]);
				add3(&vertexNormals[i3], &faceNormals[i], &vertexNormals[i3]);

				// Increment triangle count for each vertex so we can average the normals
				surroundingTriangles[i1]++;
				surroundingTriangles[i2]++;
				surroundingTriangles[i3]++;
			}

			// Calculate vertex normals
			for (int i = 0; i < vertexCount; ++i)
			{
				// Average normals
				vertexNormals[i].x /= surroundingTriangles[i];
				vertexNormals[i].y /= surroundingTriangles[i];
				vertexNormals[i].z /= surroundingTriangles[i];

				normalise3(&vertexNormals[i], &vertexNormals[i]);
			}

			delete faceNormals;
			delete surroundingTriangles;
		}
	}

	void Mesh::cut(Mesh* left, Mesh* right, Vector3 planePoint, Vector3 planeNormal)
	{
		int faceCount = indexCount / 3;

		// The maximum number of vertices for the new meshes is vertexCount + faceCount * 2
		// (two points of intersection for each face)
		int newVertexCount = vertexCount;
		int newVertexMax = vertexCount + faceCount * 2;

		Vector3* newVertices = new Vector3[newVertexMax];
		Vector3* newNormals = new Vector3[newVertexMax];
		
		memcpy(newVertices, vertices, vertexCount * sizeof(Vector3));
		memcpy(newNormals, vertexNormals, vertexCount * sizeof(Vector3));

		// The maximum number of indices for the new meshes is indexCount + faceCount * 9
		// (each face could be split up into three faces)
		int newIndexCount = indexCount;
		int newIndexMax = indexCount + faceCount * 9;
		
		int leftIndexCount = 0;
		int* leftIndices = new int[newIndexMax];

		int rightIndexCount = 0;
		int* rightIndices = new int[newIndexMax];

		// Iterate through each face and decide which list to put it in and whether to divide it
		for (int i = 0; i < faceCount; ++i)
		{
			int i1 = indices[i*3 +0];
			int i2 = indices[i*3 +1];
			int i3 = indices[i*3 +2];
			
			Vector3* v1 = &vertices[i1];
			Vector3* v2 = &vertices[i2];
			Vector3* v3 = &vertices[i3];

			// Check if each vertex is to the left of the plane
			Vector3 v1ToPlane;
			Vector3 v2ToPlane;
			Vector3 v3ToPlane;
			
			sub3(&planePoint, v1, &v1ToPlane);
			sub3(&planePoint, v2, &v2ToPlane);
			sub3(&planePoint, v3, &v3ToPlane);
			
			bool v1Left = dot3(&v1ToPlane, &planeNormal) < 0;
			bool v2Left = dot3(&v2ToPlane, &planeNormal) < 0;
			bool v3Left = dot3(&v3ToPlane, &planeNormal) < 0;

			int pointsToLeft = (v1Left ? 1 : 0) + (v2Left ? 1 : 0) + (v3Left ? 1 : 0);

			// All vertices to left
			if (pointsToLeft == 3)
			{
				// Add all vertices to left
				leftIndices[leftIndexCount++] = i1;
				leftIndices[leftIndexCount++] = i2;
				leftIndices[leftIndexCount++] = i3;
			}
			// All vertices to right
			else if (pointsToLeft == 0)
			{
				// Add all vertices to right
				rightIndices[rightIndexCount++] = i1;
				rightIndices[rightIndexCount++] = i2;
				rightIndices[rightIndexCount++] = i3;
			}
			// One vertex to left
			else if (pointsToLeft == 1)
			{
				if (v1Left)
				{
					Vector3 line1, line2;
					Vector3 intersect1, intersect2;

					int intersect1index, intersect2index;
					float intersect1coeff, intersect2coeff;

					int ia = i1;
					int ib = i2;
					int ic = i3;
					
					const Vector3* a = &newVertices[ia];
					const Vector3* b = &newVertices[ib];
					const Vector3* c = &newVertices[ic];

					sub3(b, a, &line1);
					sub3(c, a, &line2);
					
					// Calculate lerp coefficient for intersections
					intersect1coeff = linePlaneCoefficient(a, &line1, &planeNormal, &planePoint);
					intersect2coeff = linePlaneCoefficient(a, &line2, &planeNormal, &planePoint);
					
					// Calculate intersections
					lerp3(a, b, intersect1coeff, &intersect1);
					lerp3(a, c, intersect2coeff, &intersect2);
					
					intersect1index = newVertexCount++;
					intersect2index = newVertexCount++;
					
					// Save intersections as new vertices
					newVertices[intersect1index] = intersect1;
					newVertices[intersect2index] = intersect2;

					lerp3(&vertexNormals[ia], &vertexNormals[ib], intersect1coeff, &newNormals[intersect1index]);
					lerp3(&vertexNormals[ia], &vertexNormals[ic], intersect2coeff, &newNormals[intersect2index]);

					// Add triangles
					// Left
					leftIndices[leftIndexCount++] = i1;
					leftIndices[leftIndexCount++] = intersect1index;
					leftIndices[leftIndexCount++] = intersect2index;

					// Right
					rightIndices[rightIndexCount++] = intersect1index;
					rightIndices[rightIndexCount++] = i2;
					rightIndices[rightIndexCount++] = i3;

					rightIndices[rightIndexCount++] = intersect1index;
					rightIndices[rightIndexCount++] = i3;
					rightIndices[rightIndexCount++] = intersect2index;
				}
				else if (v2Left)
				{
					Vector3 line1, line2;
					Vector3 intersect1, intersect2;
					int intersect1index, intersect2index;
					float intersect1coeff, intersect2coeff;

					int ia = i2;
					int ib = i1;
					int ic = i3;
					
					const Vector3* a = &newVertices[ia];
					const Vector3* b = &newVertices[ib];
					const Vector3* c = &newVertices[ic];

					sub3(b, a, &line1);
					sub3(c, a, &line2);
					
					// Calculate lerp coefficient for intersections
					intersect1coeff = linePlaneCoefficient(a, &line1, &planeNormal, &planePoint);
					intersect2coeff = linePlaneCoefficient(a, &line2, &planeNormal, &planePoint);
					
					// Calculate intersections
					lerp3(a, b, intersect1coeff, &intersect1);
					lerp3(a, c, intersect2coeff, &intersect2);
					
					intersect1index = newVertexCount++;
					intersect2index = newVertexCount++;
					
					// Save intersections as new vertices
					newVertices[intersect1index] = intersect1;
					newVertices[intersect2index] = intersect2;

					lerp3(&vertexNormals[ia], &vertexNormals[ib], intersect1coeff, &newNormals[intersect1index]);
					lerp3(&vertexNormals[ia], &vertexNormals[ic], intersect2coeff, &newNormals[intersect2index]);

					// Add triangles
					// Left
					leftIndices[leftIndexCount++] = i2;
					leftIndices[leftIndexCount++] = intersect2index;
					leftIndices[leftIndexCount++] = intersect1index;

					// Right
					rightIndices[rightIndexCount++] = intersect1index;
					rightIndices[rightIndexCount++] = intersect2index;
					rightIndices[rightIndexCount++] = i3;

					rightIndices[rightIndexCount++] = intersect1index;
					rightIndices[rightIndexCount++] = i3;
					rightIndices[rightIndexCount++] = i1;
				}
				else
				{
					Vector3 line1, line2;
					Vector3 intersect1, intersect2;
					int intersect1index, intersect2index;
					float intersect1coeff, intersect2coeff;

					int ia = i3;
					int ib = i1;
					int ic = i2;
					
					const Vector3* a = &newVertices[ia];
					const Vector3* b = &newVertices[ib];
					const Vector3* c = &newVertices[ic];

					sub3(b, a, &line1);
					sub3(c, a, &line2);
					
					// Calculate lerp coefficient for intersections
					intersect1coeff = linePlaneCoefficient(a, &line1, &planeNormal, &planePoint);
					intersect2coeff = linePlaneCoefficient(a, &line2, &planeNormal, &planePoint);
					
					// Calculate intersections
					lerp3(a, b, intersect1coeff, &intersect1);
					lerp3(a, c, intersect2coeff, &intersect2);
					
					intersect1index = newVertexCount++;
					intersect2index = newVertexCount++;
					
					// Save intersections as new vertices
					newVertices[intersect1index] = intersect1;
					newVertices[intersect2index] = intersect2;

					lerp3(&vertexNormals[ia], &vertexNormals[ib], intersect1coeff, &newNormals[intersect1index]);
					lerp3(&vertexNormals[ia], &vertexNormals[ic], intersect2coeff, &newNormals[intersect2index]);

					// Add triangles
					// Left
					leftIndices[leftIndexCount++] = intersect1index;
					leftIndices[leftIndexCount++] = intersect2index;
					leftIndices[leftIndexCount++] = i3;

					// Right
					rightIndices[rightIndexCount++] = i2;
					rightIndices[rightIndexCount++] = intersect2index;
					rightIndices[rightIndexCount++] = intersect1index;

					rightIndices[rightIndexCount++] = i2;
					rightIndices[rightIndexCount++] = intersect1index;
					rightIndices[rightIndexCount++] = i1;
				}
			}
			// Two vertices to left
			else if (pointsToLeft == 2)
			{
				if (!v1Left)
				{
					Vector3 line1, line2;
					Vector3 intersect1, intersect2;
					int intersect1index, intersect2index;
					float intersect1coeff, intersect2coeff;

					int ia = i1;
					int ib = i2;
					int ic = i3;
					
					const Vector3* a = &newVertices[ia];
					const Vector3* b = &newVertices[ib];
					const Vector3* c = &newVertices[ic];

					sub3(b, a, &line1);
					sub3(c, a, &line2);
					
					// Calculate lerp coefficient for intersections
					intersect1coeff = linePlaneCoefficient(a, &line1, &planeNormal, &planePoint);
					intersect2coeff = linePlaneCoefficient(a, &line2, &planeNormal, &planePoint);
					
					// Calculate intersections
					lerp3(a, b, intersect1coeff, &intersect1);
					lerp3(a, c, intersect2coeff, &intersect2);
					
					intersect1index = newVertexCount++;
					intersect2index = newVertexCount++;
					
					// Save intersections as new vertices
					newVertices[intersect1index] = intersect1;
					newVertices[intersect2index] = intersect2;

					lerp3(&vertexNormals[ia], &vertexNormals[ib], intersect1coeff, &newNormals[intersect1index]);
					lerp3(&vertexNormals[ia], &vertexNormals[ic], intersect2coeff, &newNormals[intersect2index]);

					// Add triangles
					// Right
					rightIndices[rightIndexCount++] = i1;
					rightIndices[rightIndexCount++] = intersect1index;
					rightIndices[rightIndexCount++] = intersect2index;

					// Left
					leftIndices[leftIndexCount++] = intersect1index;
					leftIndices[leftIndexCount++] = i2;
					leftIndices[leftIndexCount++] = i3;

					leftIndices[leftIndexCount++] = intersect1index;
					leftIndices[leftIndexCount++] = i3;
					leftIndices[leftIndexCount++] = intersect2index;
				}
				else if (!v2Left)
				{
					Vector3 line1, line2;
					Vector3 intersect1, intersect2;
					int intersect1index, intersect2index;
					float intersect1coeff, intersect2coeff;

					int ia = i2;
					int ib = i1;
					int ic = i3;
					
					const Vector3* a = &newVertices[ia];
					const Vector3* b = &newVertices[ib];
					const Vector3* c = &newVertices[ic];

					sub3(b, a, &line1);
					sub3(c, a, &line2);
					
					// Calculate lerp coefficient for intersections
					intersect1coeff = linePlaneCoefficient(a, &line1, &planeNormal, &planePoint);
					intersect2coeff = linePlaneCoefficient(a, &line2, &planeNormal, &planePoint);
					
					// Calculate intersections
					lerp3(a, b, intersect1coeff, &intersect1);
					lerp3(a, c, intersect2coeff, &intersect2);
					
					intersect1index = newVertexCount++;
					intersect2index = newVertexCount++;
					
					// Save intersections as new vertices
					newVertices[intersect1index] = intersect1;
					newVertices[intersect2index] = intersect2;

					lerp3(&vertexNormals[ia], &vertexNormals[ib], intersect1coeff, &newNormals[intersect1index]);
					lerp3(&vertexNormals[ia], &vertexNormals[ic], intersect2coeff, &newNormals[intersect2index]);

					// Add triangles
					// Right
					rightIndices[rightIndexCount++] = i2;
					rightIndices[rightIndexCount++] = intersect2index;
					rightIndices[rightIndexCount++] = intersect1index;

					// Left
					leftIndices[leftIndexCount++] = intersect1index;
					leftIndices[leftIndexCount++] = intersect2index;
					leftIndices[leftIndexCount++] = i3;

					leftIndices[leftIndexCount++] = intersect1index;
					leftIndices[leftIndexCount++] = i3;
					leftIndices[leftIndexCount++] = i1;
				}
				else if (!v3Left)
				{
					Vector3 line1, line2;
					Vector3 intersect1, intersect2;
					int intersect1index, intersect2index;
					float intersect1coeff, intersect2coeff;

					int ia = i3;
					int ib = i1;
					int ic = i2;
					
					const Vector3* a = &newVertices[ia];
					const Vector3* b = &newVertices[ib];
					const Vector3* c = &newVertices[ic];

					sub3(b, a, &line1);
					sub3(c, a, &line2);
					
					// Calculate lerp coefficient for intersections
					intersect1coeff = linePlaneCoefficient(a, &line1, &planeNormal, &planePoint);
					intersect2coeff = linePlaneCoefficient(a, &line2, &planeNormal, &planePoint);
					
					// Calculate intersections
					lerp3(a, b, intersect1coeff, &intersect1);
					lerp3(a, c, intersect2coeff, &intersect2);
					
					intersect1index = newVertexCount++;
					intersect2index = newVertexCount++;
					
					// Save intersections as new vertices
					newVertices[intersect1index] = intersect1;
					newVertices[intersect2index] = intersect2;

					lerp3(&vertexNormals[ia], &vertexNormals[ib], intersect1coeff, &newNormals[intersect1index]);
					lerp3(&vertexNormals[ia], &vertexNormals[ic], intersect2coeff, &newNormals[intersect2index]);

					// Add triangles
					// Right
					rightIndices[rightIndexCount++] = intersect1index;
					rightIndices[rightIndexCount++] = intersect2index;
					rightIndices[rightIndexCount++] = i3;

					// Left
					leftIndices[leftIndexCount++] = i2;
					leftIndices[leftIndexCount++] = intersect2index;
					leftIndices[leftIndexCount++] = intersect1index;

					leftIndices[leftIndexCount++] = i2;
					leftIndices[leftIndexCount++] = intersect1index;
					leftIndices[leftIndexCount++] = i1;
				}
			}
			else
			{
				int j = 0;
			}
		}

		delete left->vertices;
		delete left->vertexNormals;
		delete left->indices;

		delete right->vertices;
		delete right->vertexNormals;
		delete right->indices;

		left->vertices = new Vector3[newVertexCount];
		left->vertexNormals = new Vector3[newVertexCount];
		left->indices = new int[leftIndexCount];

		left->vertexCount = newVertexCount;
		left->indexCount = leftIndexCount;
		
		memcpy(left->vertices, newVertices, left->vertexCount * sizeof(Vector3));
		memcpy(left->vertexNormals, newNormals, left->vertexCount * sizeof(Vector3));
		memcpy(left->indices, leftIndices, left->indexCount * sizeof(int));

		right->vertices = new Vector3[newVertexCount];
		right->vertexNormals = new Vector3[newVertexCount];
		right->indices = new int[rightIndexCount];

		right->vertexCount = newVertexCount;
		right->indexCount = rightIndexCount;
		
		memcpy(right->vertices, newVertices, right->vertexCount * sizeof(Vector3));
		memcpy(right->vertexNormals, newNormals, right->vertexCount * sizeof(Vector3));
		memcpy(right->indices, rightIndices, right->indexCount * sizeof(int));

		delete newVertices;
		delete newNormals;
		delete leftIndices;
		delete rightIndices;
	}
}
