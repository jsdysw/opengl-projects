#include "cube.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "GL/glew.h"


void Cube::makeUV()
{
	vector<glm::vec4> vertList;

	vertList.push_back(glm::vec4(-0.5, -0.5, 0.5, 1.0));
	vertList.push_back(glm::vec4(-0.5, 0.5, 0.5, 1.0));
	vertList.push_back(glm::vec4(0.5, 0.5, 0.5, 1.0));
	vertList.push_back(glm::vec4(0.5, -0.5, 0.5, 1.0));
	vertList.push_back(glm::vec4(-0.5, -0.5, -0.5, 1.0));
	vertList.push_back(glm::vec4(-0.5, 0.5, -0.5, 1.0));
	vertList.push_back(glm::vec4(0.5, 0.5, -0.5, 1.0));
	vertList.push_back(glm::vec4(0.5, -0.5, -0.5, 1.0));

	// make triangles
	verts.push_back(vertList[1]);  
	verts.push_back(vertList[0]);  
	verts.push_back(vertList[3]);  

	verts.push_back(vertList[3]);  
	verts.push_back(vertList[2]);  
	verts.push_back(vertList[1]);  

	verts.push_back(vertList[1]);
	verts.push_back(vertList[0]);
	verts.push_back(vertList[4]);

	verts.push_back(vertList[4]);
	verts.push_back(vertList[5]);
	verts.push_back(vertList[1]);

	verts.push_back(vertList[2]);
	verts.push_back(vertList[3]);
	verts.push_back(vertList[7]);

	verts.push_back(vertList[7]);
	verts.push_back(vertList[6]);
	verts.push_back(vertList[2]);

	verts.push_back(vertList[5]);
	verts.push_back(vertList[4]);
	verts.push_back(vertList[7]);

	verts.push_back(vertList[7]);
	verts.push_back(vertList[6]);
	verts.push_back(vertList[5]);

	verts.push_back(vertList[5]);
	verts.push_back(vertList[1]);
	verts.push_back(vertList[2]);

	verts.push_back(vertList[2]);
	verts.push_back(vertList[6]);
	verts.push_back(vertList[5]);

	verts.push_back(vertList[4]);
	verts.push_back(vertList[0]);
	verts.push_back(vertList[3]);

	verts.push_back(vertList[3]);
	verts.push_back(vertList[7]);
	verts.push_back(vertList[4]);

	for (int i = 0; i < 6; i++) {
		glm::vec2 texcoord[4];
		const int U = 0, V = 1;

		texcoord[0][U] = 1;
		texcoord[1][U] = 1;
		texcoord[2][U] = 0;
		texcoord[3][U] = 0;

		texcoord[0][V] = 0;
		texcoord[1][V] = 1;
		texcoord[2][V] = 1;
		texcoord[3][V] = 0;

		texCoords.push_back(texcoord[0]);
		texCoords.push_back(texcoord[1]);
		texCoords.push_back(texcoord[2]);

		texCoords.push_back(texcoord[2]);
		texCoords.push_back(texcoord[3]);
		texCoords.push_back(texcoord[0]);
	}
}


void Cube::computeNormals()
{
	for (int i = 0; i < verts.size(); i++)
	{
		glm::vec4 n;
		for (int k = 0; k < 3; k++)
		{
			n[k] = verts[i][k];
		}
		n[3] = 0.0;
		glm::normalize(n);
		normals.push_back(n);
	}
}