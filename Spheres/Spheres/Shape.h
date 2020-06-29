#pragma once
#include <iostream>
#include <glm.hpp>
#include <vector>
using namespace std;
using namespace glm;

struct HitInfo
{
	vec3 p;
	float t;
	vec3 colour;
	bool hit = false;
}; 
class Shape
{
public:
	vec3 p;
	float t;
	float shinyness;
	vec3 pos;
	vec3 normal;
	vec3 diffColour;
	vec3 specColour = vec3(0.7f, 0.7f, 0.7f);
	Shape(vec3 pos, vec3 diffColour = vec3(0,0,0), float shinyness = 0.0);
	~Shape();
	bool isReflective = false;
	virtual bool intersection(vec3 origin, vec3 direction, HitInfo& info);
	virtual vec3 normalCalc(vec3 p);
	virtual bool shadowintersection(vec3 origin, vec3 direction);
	virtual float getRadi();
};

