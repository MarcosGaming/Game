#include "Body.h"

// Constructor
Body::Body() {}
// Destructor
Body::~Body() {}

// TRANSFORMATION METHODS
void Body::translate(const glm::vec3 & vect)
{
	m_pos = m_pos + vect;
	m_mesh.translate(vect);
}
void Body::rotate(float angle, const glm::vec3 & vect)
{
	m_mesh.rotate(angle, vect);
}
void Body::scale(const glm::vec3 & vect) {
	m_mesh.scale(vect);
}
// FORCE MEHODS
glm::vec3 Body::applyForces(glm::vec3 x, glm::vec3 v, float t, float dt)
{
	glm::vec3 fAccumulator = glm::vec3(0.0f);

	for (auto &f : m_forces)
	{
		fAccumulator += f->apply(getMass(), x, v);
	}
	return fAccumulator / getMass();
}