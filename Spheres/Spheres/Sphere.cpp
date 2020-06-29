#include "Sphere.h"
using namespace glm;

Sphere::Sphere(vec3 pos, float radius, vec3 colour, float shinyness): Shape(pos)
{
	this->radius = radius;
	this->diffColour = colour;
	this->shinyness = shinyness;
}

Sphere::~Sphere()
{
}

bool Sphere::intersection(vec3 origin, vec3 direction, HitInfo& info)
{
	vec3 ray = origin + direction;
	vec3 I = this->pos - origin;
	float tca = dot(I, direction);
	if (tca < 0) {
		return false;
	}
	float s2 = dot(I, I) - dot(tca, tca);
	if (s2 > (this->radius * this->radius)) {
		return false;
	}
	float thc = sqrt((this->radius * this->radius) - s2);

	float t0 = tca - thc;
	float t1 = tca + thc;
	this->p = origin + t0 * direction;

	info.hit = true;
	info.t = tca - thc;
	info.colour = diffColour;
	this->t = tca - thc;
	return true;
}

float Sphere::getRadi()
{
	return this->radius;
}
vec3 Sphere::normalCalc(vec3 p)
{
	this->normal = this->p - this->pos;
	return this->normal;
}
bool Sphere::shadowintersection(vec3 origin, vec3 direction)
{
	vec3 ray = origin + direction;
	vec3 I = this->pos - origin;
	float tca = dot(I, direction);
	if (tca < 0) {
		return false;
	}
	float s2 = dot(I, I) - dot(tca, tca);
	if (s2 > (this->radius * this->radius)) {
		return false;
	}

	return true;
}
