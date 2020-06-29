#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <vector>
using namespace std;
using namespace glm;

bool loadOBJ(
	const char * path,
	vector<vec3> & out_vertices,
	vector<vec3> & out_normals
);

#endif
