#pragma once
# include <glm/glm.hpp>
# include <iostream>

class Body; // forward declaration to avoid circular dependencies

class Force
{
public:
	Force() {}
	~Force() {}

	virtual glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel);
};

// GRAVITY CLASS
class Gravity : public Force 
{
	
public:
	// constructors
	Gravity() {}
	Gravity(const glm::vec3 &gravity) { m_gravity = gravity; }
	
	// get and set methods
	glm::vec3 getGravity() const { return m_gravity; }
	void setGravity(glm::vec3 gravity) { m_gravity = gravity; }
	
	// physics
	glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel);
	
private:
	glm::vec3 m_gravity = glm::vec3(0.0f, -9.8f, 0.0f);
};

// DRAG CLASS
class Drag : public Force
{
public:
	Drag() {}
	Drag(Body* b1, Body* b2, Body *b3,  const glm::vec3 wind, float cd, float a, glm::vec3 n)
	{
		m_wind = wind; m_cd = cd; m_a = a; m_n = n;
	}
	// Physics
	glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel);private:
	glm::vec3 m_wind;	// Velocity of the wind
	float m_cd;			// Drag coefficient
	float m_a;			// Area of the surface
	glm::vec3 m_n;		// Normal of the surface
};

// HOOKE CLASS
class Hooke : public Force 
{
public:
	Hooke() {}
	Hooke(Body * b1, Body * b2, float ks, float kd, float rest) 
	{
		m_ks = ks; m_kd = kd; m_rest = rest; m_b1 = b1; m_b2 = b2;
	}
	// get and set methods you can write these yourself as necessary
	Body* getParticle1() { return m_b1; }
	void setParticle1(Body *b1) { m_b1 = b1; }

	Body* getParticle2() { return m_b2; }
	void setParticle2(Body *b2) { m_b1 = b2; }

	float getRestLength() { return m_rest; }
	void setRestLength(float restLength) { m_rest = restLength; }

	float getStiffnes() { return m_ks; }
	void setStiffnes(float ks) { m_ks = ks; }

	float getDamperCoefficient() { return m_kd; }
	void setDamperCoefficient(float kd) { m_kd = kd; }

	// physics
	glm::vec3 apply(float mass, const glm::vec3 & pos, const glm::vec3 & vel);
	
	
private:
	float m_ks;		// spring stiffness
	float m_kd;		// damping coefficient
	float m_rest;	// spring rest length
	
	Body* m_b1; // pointer to the body connected to one extremity of the spring
	Body* m_b2; // pointer to the body connected to the other extremity
};