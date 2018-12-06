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
glm::vec3 Drag::apply(float mass, const glm ::vec3 &pos, const glm::vec3 &vel)
{
	// Velocity of triangle
	glm::vec3 v = (getParticle1()->getVel() + getParticle2()->getVel() + getParticle3()->getVel()) / 3;
	v = v - getWind();
	// Normal to the surface
	glm::vec3 p1p2 = getParticle2()->getPos() - getParticle1()->getPos();
	glm::vec3 p1p3 = getParticle3()->getPos() - getParticle1()->getPos();
	glm::vec3 n = glm::normalize(glm::cross(p1p2, p1p3));
	// Area affected by the aerodynamic drag
	float ao = 0.5 * glm::length(glm::cross(p1p2, p1p3));
	float a = ao * (glm::dot(v, n)) / glm::length(v);
	// Return aerodynamic drag force
	return (0.5 * getMediumDensity() * glm::length(v*v) * getDragCoefficient() * a * n) / 3;
}

// HOOKE
glm::vec3 Hooke::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel)
{
	// Transform distances and velocities from 3D to 1D
	float length = glm::length(getParticle2()->getPos() - getParticle1()->getPos());
	glm::vec3 e = (getParticle2()->getPos() - getParticle1()->getPos()) / length;
	float v1 = glm::dot(e, getParticle1()->getVel());
	float v2 = glm::dot(e, getParticle2()->getVel());
	// Compute 1D force
	float fsd = -getStiffnes()*(getRestLength() - length) - getDamperCoefficient() * (v1 - v2);
	// Return the force in 3D
	return fsd * e;
}