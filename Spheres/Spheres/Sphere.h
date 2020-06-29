#pragma once
#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere(vec3 pos, float radius, vec3 colour, float shinyness);
	~Sphere();
	float radius;
	bool intersection(vec3 origin, vec3 direction, HitInfo& info);
	float getRadi();
	vec3 normalCalc(vec3 p);
	bool shadowintersection(vec3 origin, vec3 direction);
	
};

