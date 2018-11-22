#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/ext.hpp"
#include "glm/ext.hpp"

// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Body.h"
#include "Particle.h"
#include "RigidBody.h"
#include "Sphere.h"


// time
GLfloat t = 0.0f;
const GLfloat deltaTime = 1.0f/60.0f;
GLfloat currentTime = (GLfloat)glfwGetTime();
GLfloat accumulator = 0.0f;

// Gravity constant
glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);
Gravity* fgravity = new Gravity(g);

// Return random float
float randomFloat(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}

// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 20.0f));

	// Create table
	Mesh table = Mesh::Mesh(Mesh::QUAD);
	// Table size is 30x30
	float tableSize = 30.0f;
	table.scale(glm::vec3(tableSize, 0.0f, tableSize));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	table.setShader(lambert);

	// Array of spheres
	Sphere spheres[10];
	int spheresNumber = 10;
	for (int i = 0; i < spheresNumber; i++)
	{
		Sphere s = Sphere::Sphere();
		Shader sShader = Shader("resources/shaders/physics.vert ", "resources/shaders/physics.frag ");
		s.getMesh().setShader(sShader);
		s.setMass(1.0f);
		// Set random initial linear velocity
		s.setVel(glm::vec3(randomFloat(-20, 20), 0.0f, randomFloat(-20, 20)));
		// Set random initial position
		s.setPos(glm::vec3(randomFloat(-tableSize, tableSize), 1.0f, randomFloat(-tableSize, tableSize)));
		// Check that the sphere does not overlap with any of the other spheres
		/*for (Sphere sp : spheres)
		{
			float centreDistances = glm::sqrt(glm::pow(sp.getPos().x - s.getPos().x,2)+glm::pow(sp.getPos().z - s.getPos().z,2));
			float radiusDistances = s.getRadius() + sp.getRadius();
			// If the distance between the spheres is less or equal than the sum of the radius there is overlap
			if (centreDistances <= radiusDistances)
			{
				int randomX = rand() % 3000 - 1500;
				int randomZ = rand() % 3000 - 1500;
				float rX = (float)randomX / 100.0f;
				float rZ = (float)randomZ / 100.0f;
				s.setPos(glm::vec3(rX, 1.0f, rZ));
			}
		}*/
		// Add sphere to array
		spheres[i] = s;
	}

	// impulse elements
	float e = 1.0f;
	float frictionCoefficient = 0.25f;
	
	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		// Set frame time
		GLfloat newTime = (GLfloat)glfwGetTime();
		GLfloat frameTime = newTime - currentTime;
		currentTime = newTime;
		accumulator += frameTime;

		/*
		**	INTERACTION
		*/
		// Manage interaction
		app.doMovement(deltaTime);


		/*
		**	SIMULATION
		*/
		while (accumulator >= deltaTime)
		{
			// integration (translation)
			for (Sphere& s : spheres)
			{
				s.setAcc(s.applyForces(s.getPos(), s.getVel(), t, deltaTime));
				s.setVel(s.getVel() + deltaTime * s.getAcc());
				s.translate(s.getVel() * deltaTime);

				// Collision with table
				bool tableCollision = false;
				glm::vec3 translation;
				glm::vec3 normal;
				glm::vec3 collisionPoint;
				//Right side of the table
				if ((s.getPos().x + s.getRadius()) >= (table.getPos().x + tableSize))
				{
					// The translation vector is set based on the distance between the outside of the sphere and the side of the table
					translation = glm::vec3(-((s.getPos().x + s.getRadius()) - (table.getPos().x + tableSize)),0.0f,0.0f);
					collisionPoint = glm::vec3(table.getPos().x + tableSize,s.getPos().y,s.getPos().z);
					normal = glm::vec3(-1.0f, 0.0f, 0.0f);
					tableCollision = true;
				}
				// Left side of the table
				else if ((s.getPos().x - s.getRadius()) <= (table.getPos().x - tableSize))
				{
					// The translation vector is set based on that distance
					translation = glm::vec3(glm::abs((table.getPos().x - tableSize) - (s.getPos().x - s.getRadius())), 0.0f, 0.0f);
					collisionPoint = glm::vec3(table.getPos().x - tableSize, s.getPos().y, s.getPos().z);
					normal = glm::vec3(1.0f, 0.0f, 0.0f);
					tableCollision = true;
				}
				// Up side of the table
				else if ((s.getPos().z + s.getRadius()) >= (table.getPos().z + tableSize))
				{
					// The translation vector is set based on that distance
					translation = glm::vec3(0.0f, 0.0f, -((s.getPos().z + s.getRadius()) - (table.getPos().z + tableSize)));
					collisionPoint = glm::vec3(s.getPos().x, s.getPos().y, table.getPos().z + tableSize);
					normal = glm::vec3(0.0f, 0.0f, -1.0f);
					tableCollision = true;
				}
				// Down side of the table
				else if ((s.getPos().z - s.getRadius()) <= (table.getPos().z - tableSize))
				{
					// The translation vector is set based on that distance
					translation = glm::vec3(0.0f, 0.0f, glm::abs((table.getPos().z - tableSize) - (s.getPos().z - s.getRadius())));
					collisionPoint = glm::vec3(s.getPos().x, s.getPos().y, table.getPos().z - tableSize);
					normal = glm::vec3(0.0f, 0.0f, 1.0f);
					tableCollision = true;
				}
				if (tableCollision)
				{
					// Solve overlaping
					s.translate(translation);
					// r is the vector of the centre of mass and the point of collision
					glm::vec3 r = collisionPoint - s.getPos();
					// n is the normal of the plane of collision, in this case the normal of the plane
					glm::vec3 n = normal;
					// vr is the relative velocity
					glm::vec3 vr = s.getVel() + glm::cross(s.getAngVel(), r);
					// Calculate the normal impulse
					float jn = (-(1.0f + e) * glm::dot(vr, n)) / ((1 / s.getMass()) + glm::dot(n, (glm::cross(s.getInvInertia()* glm::cross(r, n), r))));
					// Calculate new velocities
					s.setVel(s.getVel() + (jn / s.getMass())*n);
				}

				//Sphere with sphere collision
				for (Sphere& sColliding : spheres)
				{
					if (&s == &sColliding)
					{
						continue;
					}
					// Check if the sphere s is colliding with the sColliding sphere
					if (s.getRadius() + sColliding.getRadius() > glm::distance(s.getPos(), sColliding.getPos()))
					{
						// Solve the overlapping using the velocities of the spheres
						glm::vec3 direction = glm::normalize(sColliding.getPos() - s.getPos());
						float overlap = (s.getRadius() + sColliding.getRadius()) - glm::distance(s.getPos(), sColliding.getPos());
						glm::vec3 sTranslate = -direction * (overlap * glm::length(s.getVel() / (s.getVel() + sColliding.getVel())));
						glm::vec3 sCollidingTranslate = direction * (overlap *  glm::length(sColliding.getVel() / (s.getVel() + sColliding.getVel())));
						s.translate(sTranslate);
						sColliding.translate(sCollidingTranslate);
						// r is the vector of the centre of mass and the point of collision
						/*glm::vec3 r = (s.getPos()-sTranslate) - s.getPos();
						// Calculate relative velocity
						glm::vec3 vr = sColliding.getVel() - s.getVel();
						// Calculate the normal impulse
						float jn = (-(1.0f + e) * glm::dot(vr, r)) / ((1 / s.getMass()) + (1 / sColliding.getMass()));
						// Calculate new velocities
						s.setVel(s.getVel() + (jn / s.getMass()));
						sColliding.setVel(sColliding.getVel() + (-jn / sColliding.getMass()));*/
					}
				}

			}

			// integration ( rotation )
			/*rb.setAngVel(rb.getAngVel() + deltaTime * rb.getAngAcc());
			// create skew symmetric matrix for w
			glm::mat3 angVelSkew = glm::matrixCross3(rb.getAngVel());
			// create 3x3 rotation matrix from rb rotation matrix
			glm::mat3 R = glm::mat3(rb.getRotate());
			// update rotation matrix
			R += deltaTime * angVelSkew *R;
			R = glm::orthonormalize(R);
			rb.setRotate(glm::mat4(R));

			// Check how many vertex are in collision with the plane 
			float lowestCoordinate = plane.getPos().y;
			for (auto vertex : rb.getMesh().getVertices())
			{
				glm::vec3 coordinates = rb.getMesh().getModel() * glm::vec4(vertex.getCoord(), 1.0f);
				if (coordinates.y <= plane.getPos().y)
				{
					verticesInCollision[vertexCount] = coordinates;
					vertexCount++;
					collision = true;
					// Set which coordinate is the lowest
					if (coordinates.y < lowestCoordinate)
					{
						lowestCoordinate = coordinates.y;
					}
				}
			}
			// Apply the collision
			if (collision)
			{
				//std::cout << "Colliding Vertices : " << '\n';
				glm::vec3 collisionPoint;
				// Vertex
				if (vertexCount == 1)
				{
					collisionPoint = verticesInCollision[0];
				}
				// Edge
				else if (vertexCount == 2)
				{
					collisionPoint = (verticesInCollision[0] + verticesInCollision[1]) / 2;
				}
				// Face
				else if (vertexCount == 4)
				{
					collisionPoint = (verticesInCollision[0] + verticesInCollision[1] + verticesInCollision[2] + verticesInCollision[3]) / 4;
				}
				// reset count and boolean
				vertexCount = 0;
				collision = false;
				// Translate the lowest vertex a bit so it does not overlap the plane
				rb.translate(glm::vec3(0.0f, glm::abs(plane.getPos().y - lowestCoordinate), 0.0f));
				// r is the vector of the centre of mass and the point of collision
				glm::vec3 r = collisionPoint - rb.getPos();
				// n is the normal of the plane of collision, in this case the normal of the plane
				glm::vec3 n = glm::vec3(0.0f, 1.0f, 0.0f);
				// vr is the relative velocity
				glm::vec3 vr = rb.getVel() + glm::cross(rb.getAngVel(), r);
				// vt is the direction of the tangencial impulse
				glm::vec3 vt = vr - (glm::dot(vr, n)*n);
				// calculate the normal impulse
				float jn = (-(1.0f + e) * glm::dot(vr, n)) / ((1 / rb.getMass()) + glm::dot(n, (glm::cross(rb.getInvInertia()* glm::cross(r, n), r))));
				// calculate tangencial impulse
				glm::vec3 jt;
				if (vt == glm::vec3(0.0f, 0.0f, 0.0f))
				{
					jt = glm::vec3(0.0f, 0.0f, 0.0f);
				}
				else
				{
					jt = -frictionCoefficient * glm::abs(jn) * glm::normalize(vt);
				}
				// total impulse
				float j = glm::length(jt + (jn * n));
				// set the new velocities
				rb.setVel(rb.getVel() + (j / rb.getMass())*n);
				rb.setAngVel(rb.getAngVel() + j * rb.getInvInertia() * glm::cross(r, n));
				// dicrease the velocities to make the rigid body stop
				if (glm::length(rb.getVel()) < initialVelocity && glm::length(rb.getVel()) > 0.2f)
				{
					rb.setVel(rb.getVel() / 1.05f);
				}
				else if (glm::length(rb.getVel()) < 0.2f)
				{
					rb.setVel(glm::vec3(0.0f));
					rb.setAngVel(rb.getAngVel()/1.05f);
				}
				if (glm::length(rb.getAngVel()) < 0.08f)
				{
					rb.setAngVel(glm::vec3(0.0f));
				}

				// Update rotational matrix
				angVelSkew = glm::matrixCross3(rb.getAngVel());
				R = glm::mat3(rb.getRotate());
				R += deltaTime * angVelSkew *R;
				R = glm::orthonormalize(R);
				rb.setRotate(glm::mat4(R));
			}*/
			accumulator -= deltaTime;
			t += deltaTime;
		}

		/*
		**	RENDER
		*/
		// clear buffer
		app.clear();
		// draw groud plane
		app.draw(table);
		//draw the spheres
		for (Sphere s : spheres)
		{
			app.draw(s.getMesh());
		}

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}
