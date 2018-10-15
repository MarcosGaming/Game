#pragma once
#include <glm/glm.hpp>
#include "Mesh.h"
#include <iostream>
#include "Force.h"

// Body Class
class Body
{
private:

	Mesh m_mesh;					// Body Mesh
	float m_cor;					// Body Coefficient of Restitution
	float m_mass;					// Body Mass
	glm::vec3 m_pos;				// Body Position
	glm::vec3 m_vel;				// Body Velocity
	glm::vec3 m_acc;				// Body Acceleration
	std::vector<Force*> m_forces;	// Body forces

public:

	// GET METHODS
	// Mesh
	Mesh& getMesh() { return m_mesh; }
	// Transform matrices
	glm::mat3 getTranslate() const { return m_mesh.getTranslate(); }
	glm::mat3 getRotate() const { return m_mesh.getRotate(); }
	glm::mat3 getScale() const { return m_mesh.getScale(); }
	// Dynamic variables
	glm::vec3& getAcc() { return m_acc; }
	glm::vec3& getVel() { return m_vel; }
	glm::vec3& getPos() { return m_pos; }
	// Physical properties
	float getMass() const { return m_mass; }
	float getCor() { return m_cor; }

	//SET METHODS
	// Mesh
	void setMesh(Mesh m) { m_mesh = m; }
	// Dynamic variables
	void setAcc(const glm::vec3 & vect) { m_acc = vect; }
	void setVel(const glm::vec3 & vect) { m_vel = vect; }
	void setVel(int i, float v) { m_vel[i] = v; } // set the ith coordinate of the velocity vector
	void setPos(const glm::vec3 & vect) { m_pos = vect; m_mesh.setPos(vect); }
	void setPos(int i, float p) { m_pos[i] = p; m_mesh.setPos(i, p); } // set the ith coordinate of the position vector
	// Physical properties
	void setCor(float cor) { m_cor = cor; }
	void setMass(float mass) { m_mass = mass; }

	//OTHER METHODS
	// Transformation methods
	void translate(const glm::vec3 & vect);
	void rotate(float angle, const glm::vec3 & vect);
	void scale(const glm::vec3 & vect);

	//FORCE METHODS
	std::vector<Force*> getForces() { return m_forces; }
	void addForce(Force *f) { m_forces.push_back(f); }
	glm::vec3 applyForces(glm::vec3 x, glm::vec3 v, float t, float dt);
	// Constructor
	Body();
	// Destructor
	~Body();
};