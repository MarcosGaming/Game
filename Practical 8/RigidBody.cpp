#include "RigidBody.h"
#include "glm/ext.hpp"

RigidBody::RigidBody() 
{
	// Inital h,d and w of a cube is 2 and therefore Io is going to be the same in the three axis and the mass is 1 as default
	float Ixyz = (1.0f / 12.0f) * 1.0f * ( 4.0f + 4.0f);
	glm::mat3 tensor = glm::mat3(Ixyz);
	m_invTensor = glm::inverse(tensor);
};
RigidBody::~RigidBody() {};

// The inertia tensor is going to change when the body scale is changed
void RigidBody::scale(const glm::vec3 &vect) 
{
	__super::scale(vect);
	updateTensor();
}
// The inertia tensor is going to change when the body mass is changed
void RigidBody::setMass(float mass)
{
	__super::setMass(mass);
	updateTensor();
}

void RigidBody::updateTensor()
{
	// Get the width, height and depth of the body
	float w = 2.0f * getScale()[0][0];
	float h = 2.0f * getScale()[1][1];
	float d = 2.0f * getScale()[2][2];
	// Set Ix, Iy, Iz for the inertia tensor matrix diagonal
	glm::mat3 tensor = glm::mat3(1.0f);
	tensor[0][0] = (1.0f / 12.0f) * getMass() * (glm::pow(h, 2) + glm::pow(d, 2));
	tensor[1][1] = (1.0f / 12.0f) * getMass() * (glm::pow(w, 2) + glm::pow(d, 2));
	tensor[2][2] = (1.0f / 12.0f) * getMass() * (glm::pow(w, 2) + glm::pow(h, 2));
	// Set the inverse inertia tensor
	m_invTensor = glm::inverse(tensor);
}