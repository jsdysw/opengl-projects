#pragma once

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

using namespace std;

class Cube {
public:
	vector<glm::vec4> verts;
	vector<glm::vec4> normals;
	vector<glm::vec2> texCoords;

	Cube() { 
		makeUV();
		computeNormals();
	};
	~Cube() {
		verts.clear();
		vector<glm::vec4>().swap(verts);
		normals.clear();
		vector<glm::vec4>().swap(normals);
	}
private:
	void makeUV();
	void computeNormals();
};


