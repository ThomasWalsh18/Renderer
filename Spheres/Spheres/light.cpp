#include "light.h"

light::light(vec3 pos, vec3 intencity, vec3 colour)
{
	this->pos = pos;
	this->intencity = intencity;
	this->colour = colour;
}

light::~light()
{
}

vec3 light::lightCalc(vec3 origin, vector<AxisAlignedBounding*> Meshes, vec3 RayDirection, int objectIndex, int MeshHit, int depth)
{
	vec3 finalColour = vec3(0.0f);
	vec3 Ray = origin + RayDirection;
	vec3 Ambient = this->intencity * vec3(0.1f);

	vec3 lightRay = pos - Meshes[MeshHit]->objects[objectIndex]->p;
	vec3 lightRayNorm = normalize(lightRay);
	vec3 intersection = Meshes[MeshHit]->objects[objectIndex]->normalCalc(Meshes[MeshHit]->objects[objectIndex]->p);
	vec3 intersectionNormal = normalize(intersection);
	vec3 diffuse = Meshes[MeshHit]->objects[objectIndex]->diffColour * (max(0.0f, dot(lightRayNorm, intersectionNormal)));

	vec3 shadOrigin = Meshes[MeshHit]->objects[objectIndex]->p;// +0.0001f;
	vec3 shadDirection = normalize(pos - shadOrigin);
	for (int i = 0; i < Meshes.size(); i++) {
		for (int j = 0; j < Meshes[i]->objects.size(); j++) {
			if (objectIndex != j || MeshHit != i)
			{
				//if (Meshes[i]->objects[j] != Meshes[i]->objects[objectIndex]) {
				if (Meshes[i]->objects[j]->shadowintersection(shadOrigin, shadDirection)) {
					return Ambient;
				}
				//}
			}
		}
	}

	vec3 reflection = normalize(2 * (dot(lightRay, intersection)) * intersection - lightRay);
	vec3 specular = Meshes[MeshHit]->objects[objectIndex]->specColour * this->intencity * pow(max(0.0f, dot(-reflection, normalize(RayDirection))), Meshes[MeshHit]->objects[objectIndex]->shinyness); //128.0f);

	finalColour = Ambient + diffuse + specular;

	if (Meshes[MeshHit]->objects[objectIndex]->isReflective == true && depth < 6) {

		vec3 RefOrigin = Meshes[MeshHit]->objects[objectIndex]->p + Meshes[MeshHit]->objects[objectIndex]->normalCalc(Meshes[MeshHit]->objects[objectIndex]->p) * 0.0001f;
		vec3 RefDir = normalize(reflect(RayDirection, Meshes[MeshHit]->objects[objectIndex]->normalCalc(Meshes[MeshHit]->objects[objectIndex]->p)));
		vec3 RefCol;
		HitInfo info;
		float Closest = INFINITY;
		int RefMeshIndex;
		int RefObjIndex;

		for (int i = 0; i < Meshes.size(); i++) {

			if (Meshes[i]->intersection(RefOrigin, RefDir)) {
				for (int j = 0; j < Meshes[i]->objects.size(); j++) {
					if (objectIndex != j || MeshHit != i) //if its not itself
					{
						if (Meshes[i]->objects[j]->intersection(RefOrigin, RefDir, info)) {
							if (info.hit == true && info.t < Closest)
							{
								Closest = info.t;
								RefMeshIndex = i;
								RefObjIndex = j;
							}

						}
					}
				}
			}
		}
		if (Closest != INFINITY) {
			RefCol = lightCalc(RefOrigin, Meshes, RefDir, RefObjIndex, RefMeshIndex, ++depth);
		}
		finalColour = finalColour + (RefCol * Meshes[MeshHit]->objects[objectIndex]->diffColour);
	}

	return finalColour;

}