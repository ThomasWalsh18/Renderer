#include "Plane.h"

Plane::Plane(vec3 pos, vec3 normal, vec3 color, float shinyness) : Shape(pos) {
	this->normal = normal;
	this->diffColour = color;
	this->shinyness = shinyness;
}

Plane::~Plane()
{
	delete this;
}

bool Plane::intersection(vec3 origin, vec3 direction, HitInfo& info)
{
	float denominator = dot(direction, this->normal);
	if (denominator == 0) {
		return false;
	}
	float t = dot(this->pos - origin, this->normal) / denominator;
	if (t < 0) {
		return false;
	}
	this->p = origin + t * direction;
	info.hit = true;
	info.t = t;
	this->t = t;
	return true;
}
bool Plane::shadowintersection(vec3 origin, vec3 direction)
{
	float denominator = dot(direction, this->normal);
	if (denominator == 0) {
		return false;
	}
	float t = dot(this->pos - origin, this->normal) / denominator;
	if (t < 0) {
		return false;
	}

	return true;
}

vec3 Plane::normalCalc(vec3 p)
{
	return this->normal;
}