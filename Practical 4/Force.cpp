# include <iostream>
# include <cmath>
# include "Force.h"
# include "Body.h"
# include "glm/ext.hpp "

glm::vec3 Force::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel) 
{
	return glm::vec3(0.0f);
}

// GRAVITY
glm::vec3 Gravity::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel)
{
	// Return the force
	return mass * glm::vec3(0.0f, -9.8f, 0.0f);
		
}

// DRAG
glm::vec3 Gravity::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel)
{
	// Return the force (Hardcoded)	float dragCoefficient = 1.05f;	float mediumDensity = 1.0f;	float area = 0.1f;	glm::vec3 e = vel / vel;	return (0.5 * mediumDensity * (vel*vel) * dragCoefficient * area * e);}// HOOKEglm::vec3 Hooke::apply(float mass, const glm::vec3 & pos, const glm::vec3 & vel) 
{
	// Return the force

}