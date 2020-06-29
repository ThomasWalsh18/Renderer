#pragma once
#include "Shape.h"
class Triangle : public Shape
{
public:
	Triangle(vec3 point1, vec3 point2, vec3 point3, vec3 n0, vec3 n1, vec3 n2, vec3 color, float shinynes);
	~Triangle();
	bool intersection(vec3 origin, vec3 direction, HitInfo& info);
	bool shadowintersection(vec3 origin, vec3 direction);
	vec3 normalCalc(vec3 p);
	vec3 normal;
	vec3 a, b, c, n0, n1, n2;
};
