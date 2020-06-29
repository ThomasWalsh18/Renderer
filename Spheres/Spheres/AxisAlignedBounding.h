#pragma once
#include <glm.hpp>
#include "Shape.h"
#include <vector>
using namespace std;
using namespace glm;

class AxisAlignedBounding
{
public:
	AxisAlignedBounding(vec3 pmin, vec3 pmax, vector<Shape*> objects);
	~AxisAlignedBounding();
	bool PlaneIntersection(vec3 p);
	bool intersection(vec3 origin, vec3 direction);
	vector<Shape*> objects;
	vec3 Pmin;
	vec3 Pmax;
};

