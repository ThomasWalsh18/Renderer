#include "AxisAlignedBounding.h"

AxisAlignedBounding::AxisAlignedBounding(vec3 pmin, vec3 pmax, vector<Shape*> objects)
{
	this->objects = objects;
	this->Pmax = pmax;
	this->Pmin = pmin;
}

AxisAlignedBounding::~AxisAlignedBounding()
{
	//delete this;
}
bool AxisAlignedBounding::intersection(vec3 origin, vec3 direction)
{	
	double t1 = 0.0, t2 = 0.0, t3 = 0.0, t4 = 0.0, t5 = 0.0, t6 = 0.0;

	t1 = (Pmin.x - origin.x) / direction.x;
	t2 = (Pmax.x - origin.x) / direction.x;
	
	double tmin = min(t1,t2);
	double tmax = max(t1,t2);

	t3 = (Pmin.y - origin.y) / direction.y;
	t4 = (Pmax.y - origin.y) / direction.y;

	tmin = max(tmin, min(t3, t4));
	tmax = min(tmax, max(t3, t4));

	t5 = (Pmin.z - origin.z) / direction.z;
	t6 = (Pmax.z - origin.z) / direction.z;

	tmin = max(tmin, min(t5, t6));
	tmax = min(tmax, max(t5, t6));

	if (tmax < max(tmin, 0.0)) {
		return false;
	}

	return true;
}
