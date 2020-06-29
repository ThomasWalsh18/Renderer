#pragma once
#include "Shape.h"
class Plane : public Shape
{
public:
	Plane(vec3 pos, vec3 normal, vec3 color, float shinyness);
	~Plane();
	bool intersection(vec3 origin, vec3 direction, HitInfo& info);
	bool shadowintersection(vec3 origin, vec3 direction);
	vec3 normalCalc(vec3 p);
	vec3 normal;
};

