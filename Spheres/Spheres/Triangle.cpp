#include "Triangle.h"

Triangle::Triangle(vec3 point1, vec3 point2, vec3 point3, vec3 n0, vec3 n1, vec3 n2, vec3 color, float shinyness) : Shape(pos)
{
	this->a = point1;
	this->b = point2;
	this->c = point3;	
	this->n0 = n0;
	this->n1 = n1;
	this->n2 = n2;
	this->diffColour = color;
	this->shinyness = shinyness;
}

Triangle::~Triangle()
{
	delete this;
}

bool Triangle::intersection(vec3 origin, vec3 direction, HitInfo& info)
{ 
	vec3 e1 = this->b - this->a;
	vec3 e2 = this->c - this->a;

	vec3 e22 = cross(direction, e2);
	vec3 o2 = origin - this->a;
	float dot22 = dot(e1, e22);
	float invdot22 = 1 / dot(e1, e22);

	float u = dot(o2, e22) * invdot22;
	float v = dot(direction, (cross(o2, e1) * invdot22));

	if (u < 0 || u > 1) {
		return false;
	}
	else if (v < 0 || (u + v) > 1) {
		return false;
	}
	float w = 1 - u - v;
	float t = dot(e2, cross(o2, e1)) * invdot22;
	
	this->normal = w * this->n0 + u * this->n1 + v * this->n2;

	info.hit = true; 
	info.t = t;
	this->t = t;
	return true;
}
bool Triangle::shadowintersection(vec3 origin, vec3 direction)
{ 
	vec3 e1 = this->b - this->a;
	vec3 e2 = this->c - this->a;

	vec3 e22 = cross(direction, e2);
	vec3 o2 = origin - this->a;
	float dot22 = dot(e1, e22);
	float invdot22 = 1 / dot(e1, e22);

	float u = dot(o2, e22) * invdot22;
	float v = dot(direction, (cross(o2, e1) * invdot22));

	if (u < 0 || u > 1) {
		return false;
	}
	else if (v < 0 || (u + v) > 1) {
		return false;
	}
	return true;
}
vec3 Triangle::normalCalc(vec3 p)
{
	return this->normal;
}