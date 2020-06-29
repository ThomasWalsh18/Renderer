#pragma once
#include "Shape.h"
#include "Sphere.h"
#include "AxisAlignedBounding.h"
#include <stdlib.h>    
#include <time.h>

using namespace glm;
class AreaLight
{
public:
	AreaLight(vec3 pos, vec3 intencity, vec3 colour, int radi, int sampleAmmount);
	~AreaLight();
	vec3 lightCalc(vec3 origin,vector<AxisAlignedBounding*> Meshes, vec3 RayDirection, int objectIndex, int MeshHit, int depth);
	int radi;
	vec3 pos;
	vec3 intencity;
	vec3 colour;
	vector<vec3>SamplePoints;
	int sampleAmmount;
	//vec3 lightCalc(Shape* object, vec3 RayDirection);

};
