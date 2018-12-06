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
	void setRotationalMomentum(const glm::vec3 & beta) { m_L = beta; }

	glm::vec3 getAngVel() { return m_angVel; }
	glm::vec3 getAngAcc() { return m_angAcc; }
	glm::vec3 getRotationalMomentum() { return m_L; }
	glm::mat3 getInvInertia() { return getRotate() * m_invTensor * glm::transpose(getRotate()); }
	glm::mat3 getInertia(){ return getRotate() * m_tensor * glm::transpose(getRotate()); }
	void scale(const glm::vec3 & vect) override;
	void setMass(float mass) override;

private:
	float m_density;		// Rigidbody density
	glm::mat3 m_invTensor;	// Inverse inertia tensor
	glm::mat3 m_tensor;		// Inertia tensor
	glm::vec3 m_angVel;		// Angular velocity
	glm::vec3 m_angAcc;		// Angular acceleration
	glm::vec3 m_L;			// Rotational momentum

	// Method to update the inertia tensor
	void updateTensor();
};
