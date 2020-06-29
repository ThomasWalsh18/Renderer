#pragma once
#include "Shape.h"
#include "Sphere.h"
#include "AxisAlignedBounding.h"
using namespace glm;
class light
{
public:
	light(vec3 pos, vec3 intencity, vec3 colour);
	~light();
	vec3 lightCalc(vec3 origin, vector<AxisAlignedBounding*> Meshes, vec3 RayDirection, int objectIndex, int MeshHit, int depth);
	vec3 pos;
	vec3 intencity;
	vec3 colour;

};
