#include "Sphere.h"

// Default constructor for the sphere
Sphere::Sphere()
{
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