#pragma once
#include "Body.h"

class RigidBody : public Body
{
public :
	RigidBody();
	~RigidBody();

	// set and get methods
	void setAngVel(const glm::vec3 & omega) { m_angVel = omega; }
	void setAngAccl(const glm::vec3 & alpha) { m_angAcc = alpha; }

	glm::vec3 getAngVel() { return m_angVel; }
	glm::vec3 getAngAcc() { return m_angAcc; }
	glm::mat3 getInvInertia() { return getRotate() * m_invTensor * glm::transpose(getRotate()); }
	void scale(const glm::vec3 & vect) override;
	void setMass(float mass) override;

private:
	float m_density;		// Rigidbody density
	glm::mat3 m_invTensor;	// Inverse inertia tensor
	glm::vec3 m_angVel;		// Angular velocity
	glm::vec3 m_angAcc;		// Angular acceleration

	// Method to update the inertia tensor
	void updateTensor();
};
