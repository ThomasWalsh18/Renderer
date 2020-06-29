#include "Shape.h"

Shape::Shape(vec3 pos, vec3 diffColour, float shinyness)
{
	this->pos = pos;
	this->shinyness = shinyness;
}

Shape::~Shape()
{
	//delete this;
}

bool Shape::intersection(vec3 origin, vec3 direction, HitInfo& info)
{
	return false;
}

vec3 Shape::normalCalc(vec3 p)
{
	return vec3(0,0,0);
}

bool Shape::shadowintersection(vec3 origin, vec3 direction)
{
	return false;
}

float Shape::getRadi()
{
	return 0.0f;
}
