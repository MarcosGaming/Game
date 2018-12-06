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

glm::vec3 frictionForce(glm::vec3 vel, float mass, glm::vec3 L)
{
	if (vel == glm::vec3(0.0f))
	{
		return glm::vec3(0.0f);
	}
	glm::vec3 direction = glm::normalize(vel);
	glm::vec3 fFriction = -direction * 0.1 * mass * glm::length(glm::vec3(0.0f, -9.8f, 0.0f));
	return fFriction;
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
	float tableSize = 40.0f;
	float cornerX = table.getPos().x - (tableSize / 2.0f);
	float cornerZ = table.getPos().z - (tableSize / 2.0f);
	table.scale(glm::vec3(tableSize, 0.0f, tableSize));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	table.setShader(lambert);

	// Create grid
	const int gridDimension = 8;
	std::vector<Sphere*> grid[gridDimension][gridDimension];
	float cellsDimension = 5.0f;

	// Spheres positions
	glm::vec3 positions[16];
	positions[0] = glm::vec3(table.getPos().x,1.0f,table.getPos().z + 20.0f);
	positions[1] = glm::vec3(table.getPos().x, 1.0f, table.getPos().z);
	positions[2] = glm::vec3(table.getPos().x + 1.35f, 1.0f, table.getPos().z - 1.5f);
	positions[3] = glm::vec3(table.getPos().x - 1.35f, 1.0f, table.getPos().z - 1.5f);
	positions[4] = glm::vec3(table.getPos().x, 1.0f, table.getPos().z - 3.0f);
	positions[5] = glm::vec3(table.getPos().x + 2.7f, 1.0f, table.getPos().z - 3.0f);
	positions[6] = glm::vec3(table.getPos().x - 2.7f, 1.0f, table.getPos().z - 3.0f);
	positions[7] = glm::vec3(table.getPos().x + 1.35f, 1.0f, table.getPos().z - 4.5f);
	positions[8] = glm::vec3(table.getPos().x + 4.05f, 1.0f, table.getPos().z - 4.5f);
	positions[9] = glm::vec3(table.getPos().x - 1.35f, 1.0f, table.getPos().z - 4.5f);
	positions[10] = glm::vec3(table.getPos().x - 4.05f, 1.0f, table.getPos().z - 4.5f);
	positions[11] = glm::vec3(table.getPos().x, 1.0f, table.getPos().z - 6.0f);
	positions[12] = glm::vec3(table.getPos().x + 2.7f, 1.0f, table.getPos().z - 6.0f);
	positions[13] = glm::vec3(table.getPos().x + 5.4f, 1.0f, table.getPos().z - 6.0f);
	positions[14] = glm::vec3(table.getPos().x - 2.7f, 1.0f, table.getPos().z - 6.0f);
	positions[15] = glm::vec3(table.getPos().x - 5.4f, 1.0f, table.getPos().z - 6.0f);
	

	// Array of spheres
	const int spheresNumber = 1;
	Sphere* spheres[spheresNumber];
	Shader sShader = Shader("resources/shaders/physics.vert ", "resources/shaders/physics.frag ");
	Mesh mesh =  Mesh::Mesh("resources/models/sphere.obj");
	Friction* fFriction = new Friction();
	fFriction->setFrictionCoefficient(0.1f);
	for (int i = 0; i < spheresNumber; i++)
	{
		Sphere* s = new Sphere();
		s->setMesh(mesh);
		s->getMesh().setShader(sShader);
		s->setMass(1.0f);
		// Set Position
		s->setPos(positions[i]);
		s->addForce(fFriction);
		spheres[i] = s;
	}

	// impulse elements
	float e = 0.3f;
	float tableFriction = 0.15f;
	float staticFriction = 0.25f;
	float kineticFriction = 0.001f;
	float maxVelocity = 20.0f;
	// Contact normal with table
	glm::vec3 contactN = glm::vec3(0.0f, 1.0f, 0.0f);
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
		// If r is press restart
		if (glfwGetKey(app.getWindow(), '0') == GLFW_PRESS)
		{
			for (int i = 0; i < spheresNumber; i++)
			{
				// Set Position
				spheres[i]->setVel (glm::vec3(0.0f));
				spheres[i]->setAngAccl(glm::vec3(0.0f));
				spheres[i]->setAngVel(glm::vec3(0.0f));
				spheres[i]->setPos(positions[i]);
			}
		}
		// If 1 is press normal impulse added to ball 0
		if (glfwGetKey(app.getWindow(), '1') == GLFW_PRESS)
		{
			glm::vec3 j = glm::vec3(0.0f, 0.0f, -5.0f);
			spheres[0]->setVel(spheres[0]->getVel() + j / spheres[0]->getMass());
		}
		// Press 2 back spin
		if (glfwGetKey(app.getWindow(), '2') == GLFW_PRESS)
		{
			glm::vec3 j1 = glm::vec3(0.0f, 0.0f, -10.0f);
			spheres[0]->setVel(spheres[0]->getVel() + j1 / spheres[0]->getMass());
			glm::vec3 j2 = glm::vec3(0.0f, 0.0f, -2.0f);
			spheres[0]->setAngVel(spheres[0]->getAngVel() + spheres[0]->getInvInertia() * glm::cross(spheres[0]->getPos() + glm::vec3(0.0f, -1.0f, -1.0f) - spheres[0]->getPos(), j2));
		}


		/*
		**	SIMULATION
		*/
		while (accumulator >= deltaTime)
		{
			// Move all spheres and update cells
			for (Sphere* s : spheres)
			{
				// Integration (translation)
				s->setAcc(s->applyForces(s->getPos(), s->getVel(), t, deltaTime));
				s->setVel(s->getVel() + deltaTime * s->getAcc());
				s->translate(s->getVel() * deltaTime);

				// integration ( rotation )
				//s->setAngVel(s->getAngVel() + deltaTime * s->getAngAcc());
				glm::vec3 L = s->getInertia() * s->getAngVel();
				glm::vec3 torque = glm::cross((s->getPos() - glm::vec3(0.0f, 1.0f, 0.0f)) - s->getPos(), frictionForce(s->getVel(), s->getMass(), L));
				s->setRotationalMomentum(L + torque * deltaTime);
				// If linear velocity is 0 reduce angular velocity
				if (glm::length(s->getVel()) < 0.01f)
				{
					s->setAngVel(s->getInvInertia() * s->getRotationalMomentum()/1.05f);
				}
				else
				{
					s->setAngVel(s->getInvInertia() * s->getRotationalMomentum());
				}
				// create skew symmetric matrix for w
				glm::mat3 angVelSkew = glm::matrixCross3(s->getAngVel());
				// create 3x3 rotation matrix from rb rotation matrix
				glm::mat3 R = glm::mat3(s->getRotate());
				// update rotation matrix
				R += deltaTime * angVelSkew *R;
				R = glm::orthonormalize(R);
				s->setRotate(glm::mat4(R));

				// Get the possible cells in which the sphere s is(max is four cells)
				int i[2];
				int j[2];
				// Possible x cells	
				int i1 = std::floor((s->getPos().x + s->getRadius() - cornerX) / cellsDimension);
				int i2 = std::floor((s->getPos().x - s->getRadius() - cornerX) / cellsDimension);
				// Check that the cells are in the boundaries
				if (i1 < 0) { i1 = 0; }
				else if (i1 > gridDimension - 1) { i1 = gridDimension - 1; }
				if (i2 < 0) { i2 = 0; }
				else if (i2 > gridDimension - 1) { i2 = gridDimension - 1; }
				i[0] = i1;
				i[1] = i2;
				// Possible z cells
				int j1 = std::floor((s->getPos().z + s->getRadius() - cornerZ) / cellsDimension);
				int j2 = std::floor((s->getPos().z - s->getRadius() - cornerZ) / cellsDimension);
				// Check that the cells are in the boundaries
				if (j1 < 0) { j1 = 0; }
				else if (j1 > gridDimension - 1) { j1 = gridDimension - 1; }
				if (j2 < 0) { j2 = 0; }
				else if (j2 > gridDimension - 1) { j2 = gridDimension - 1; }
				j[0] = j1;
				j[1] = j2;

				// Update the cells
				grid[i[0]][j[0]].push_back(s);
				if (j[1] != j[0])
				{
					grid[i[0]][j[1]].push_back(s);
				}
				if (i[1] != i[0])
				{
					grid[i[1]][j[0]].push_back(s);
					if (j[1] != j[0])
					{
						grid[i[1]][j[1]].push_back(s);
					}
				}
			}

			//Calculate collisions for each cell in the grid
			for (int i = 0; i < gridDimension; i++)
			{
				for (int j = 0; j < gridDimension; j++)
				{
					for (Sphere* s : grid[i][j])
					{
						// Friction with the table 
						/*float forceGravity = -9.8f * s->getMass();
						// Calculate vt
						glm::vec3 rTable = glm::vec3(s->getPos().x, s->getPos().y - s->getRadius(), s->getPos().z) - s->getPos();
						glm::vec3 vrTable = s->getVel() + glm::cross(s->getAngVel(), contactN);
						glm::vec3 vtTable = vrTable - (glm::dot(vrTable, contactN)*contactN);
						float vtMagnitude = glm::length(vtTable);
						// Use dynamic friction
						if (vtMagnitude > 5.0f)
						{
							glm::vec3 tangencialImpulse = (forceGravity * -kineticFriction * glm::normalize(vtTable));
							std::cout << glm::to_string(tangencialImpulse) << '\n';
							// Calculate new velocities
							s->setAcc(s->getAcc() - tangencialImpulse / s->getMass());
							s->setAngAccl(s->getAngAcc() - tangencialImpulse / s->getMass());
						}
						// Use static friction
						else if(vtMagnitude != 0.0f)
						{
							vtMagnitude /= 5.0f;
							glm::vec3 tangencialImpulse =  (vtMagnitude * forceGravity * -staticFriction * glm::normalize(vtTable));
							// Calculate new velocities
							s->setAcc(s->getAcc() + tangencialImpulse / s->getMass());
							s->setAngAccl(s->getAngAcc() + tangencialImpulse / s->getMass());
						}*/

						// Collision with table only if i is 0/max or j is 0/max
						if (i == 0 || i == gridDimension - 1 || j == 0 || j == gridDimension - 1)
						{
							bool tableCollision = false;
							glm::vec3 translation = glm::vec3(0.0f);
							glm::vec3 normal = glm::vec3(0.0f);
							glm::vec3 collisionPoint = s->getPos();
							for (int i = 0; i < 3; i++)
							{
								if (i != 1)
								{
									if (s->getPos()[i] + s->getRadius() > table.getPos()[i] + tableSize)
									{
										translation[i] = -((s->getPos()[i] + s->getRadius()) - (table.getPos()[i] + tableSize));
										collisionPoint[i] = table.getPos()[i] + tableSize;
										normal[i] = -1.0f;
										tableCollision = true;
										break;
									}
									else if (s->getPos()[i] - s->getRadius() < table.getPos()[i] - tableSize)
									{
										translation[i] = glm::abs((table.getPos()[i] - tableSize) - (s->getPos()[i] - s->getRadius()));
										collisionPoint[i] = table.getPos()[i] - tableSize;
										normal[i] = 1.0f;
										tableCollision = true;
										break;
									}
								}
							}
							if (tableCollision)
							{
								// Solve overlaping
								s->translate(translation);
								// r is the vector of the centre of mass and the point of collision
								glm::vec3 r = collisionPoint - s->getPos();
								// n is the normal of the plane of collision, in this case the normal of the plane
								glm::vec3 n = normal;
								// vr is the relative velocity
								glm::vec3 vr = s->getVel() + glm::cross(s->getAngVel(), r);
								// vt is the direction of the tangencial impulse
								glm::vec3 vt = vr - (glm::dot(vr, n)*n);
								// Calculate the normal impulse
								float jn = (-(1.0f + e) * glm::dot(vr, n)) / ((1 / s->getMass()) + glm::dot(n, (glm::cross(s->getInvInertia()* glm::cross(r, n), r))));
								// calculate tangencial impulse
								glm::vec3 jt;
								if (vt == glm::vec3(0.0f, 0.0f, 0.0f))
								{
									jt = glm::vec3(0.0f, 0.0f, 0.0f);
								}
								else
								{
									jt = -tableFriction * glm::abs(jn) * glm::normalize(vt);
								}
								// total impulse
								float j = glm::length(jt + (jn * n));
								// Calculate new velocities
								s->setVel(s->getVel() + (j / s->getMass())*n);
								s->setAngVel(s->getAngVel() + j * s->getInvInertia() * glm::cross(r, n));
								//std::cout << "After " + glm::to_string(s->getAngVel()) << '\n';
							}
						}

						//Sphere with sphere collisions if there are more than one in the cell
						/*if (grid[i][j].size() > 1)
						{
							for (Sphere* sColliding : grid[i][j])
							{
								if (s != sColliding)
								{
									// Check if the sphere s is colliding with the sColliding sphere
									if (s->getRadius() + sColliding->getRadius() > glm::distance(s->getPos(), sColliding->getPos()))
									{
										// Solve the overlapping using the velocities of the spheres
										glm::vec3 n = glm::normalize(sColliding->getPos() - s->getPos());
										float overlap = (s->getRadius() + sColliding->getRadius()) - glm::distance(s->getPos(), sColliding->getPos());
										float vSum = glm::length(s->getVel() + sColliding->getVel());
										glm::vec3 sTranslate = -n * (overlap * (glm::length(s->getVel()) / vSum));
										glm::vec3 sCollidingTranslate = n * (overlap *  (glm::length(sColliding->getVel()) / vSum));
										s->translate(sTranslate);
										sColliding->translate(sCollidingTranslate);
										// collision point
										glm::vec3 r1 = (s->getPos() + (s->getRadius()*n)) - s->getPos();
										glm::vec3 r2 = (sColliding->getPos() + (sColliding->getRadius()*(-n))) - sColliding->getPos();
										// Calculate relative velocity
										glm::vec3 vr = sColliding->getVel() + glm::cross(sColliding->getAngVel(),r2) - (s->getVel() + glm::cross(s->getAngVel(), r1));
										// vt is the direction of the tangencial impulse
										glm::vec3 vt = vr - (glm::dot(vr, n)*n);
										// Calculate the normal impulse
										float jn = (-(1.0f + e) * glm::dot(vr, n)) / ((1 / s->getMass()) + (1 / sColliding->getMass()) + glm::dot(n, (glm::cross(s->getInvInertia()* glm::cross(r1, n), r1)) + (glm::cross(sColliding->getInvInertia()* glm::cross(r2, n), r2))));
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
										// Calculate new velocities
										s->setVel(s->getVel() - (j*n / s->getMass()));
										sColliding->setVel(sColliding->getVel() + (j*n / sColliding->getMass()));
										s->setAngVel(s->getAngVel() - j * s->getInvInertia() * glm::cross(r1, n));
										sColliding->setAngVel(sColliding->getAngVel() + j * sColliding->getInvInertia() * glm::cross(r2, n));
									}
								}
							}
						}*/
					}
					// Clean the cell
					grid[i][j].clear();
				}
			}
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
		/*for (Sphere* s : spheres)
		{
			app.draw(s->getMesh());
		}*/
		app.draw(spheres[0]->getMesh());

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}
