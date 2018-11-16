#include "Sphere.h"

// Default constructor for the sphere
Sphere::Sphere()
{
	// The initial scale has a radius of 1.0f
	setMesh(Mesh::Mesh("resources/models/sphere.obj"));
	// Set the radius to the default which is 1.0f
	setRadius(1.0f);
}
// The radius needs to be updated if the sphere is scaled (as its a sphere it is assumed that x,y and z are scaled by the same);
void Sphere::scale(const glm::vec3 & vect)
{
	__super::scale(vect);
	setRadius(1.0f * getScale()[0][0]);
}

// Default destructor
Sphere::~Sphere(){}