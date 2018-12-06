#pragma once
#include "RigidBody.h"

class Sphere : public RigidBody
{
public:
	// Constructor
	Sphere();
	// Destructor
	~Sphere();
	// Set methods
	void setRadius(float radius) { m_radius = radius; }
	void scale(const glm::vec3 & vect) override;
	// Get methods
	float getRadius() { return m_radius; }

private:
	float m_radius;		// Radius of the sphere
};