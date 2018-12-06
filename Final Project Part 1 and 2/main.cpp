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
	float tableSize = 1000.0f;
	float cornerX = table.getPos().x - (tableSize / 2.0f);
	float cornerZ = table.getPos().z - (tableSize / 2.0f);
	table.scale(glm::vec3(tableSize, 0.0f, tableSize));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	table.setShader(lambert);

	// Create grid
	const int gridDimension = 100;
	std::vector<Sphere*> grid[gridDimension][gridDimension];
	float cellsDimension = 10.0f;
	

	// Array of spheres
	const int spheresNumber = 10000;
	Sphere* spheres[spheresNumber];
	Shader sShader = Shader("resources/shaders/physics.vert ", "resources/shaders/physics.frag ");
	Mesh mesh =  Mesh::Mesh("resources/models/sphere.obj");
	for (int i = 0; i < spheresNumber; i++)
	{
		Sphere* s = new Sphere();
		s->setMesh(mesh);
		s->getMesh().setShader(sShader);
		s->setMass(1.0f);
		// Set random initial linear velocity
		s->setVel(glm::vec3(randomFloat(-20, 20), 0.0f, randomFloat(-20, 20)));
		// Set random initial position
		s->setPos(glm::vec3(randomFloat(-tableSize/2.0f, tableSize/2.0f), s->getRadius(), randomFloat(-tableSize/2.0f, tableSize/2.0f)));
		spheres[i] = s;
	}

	// impulse elements
	float e = 1.0f;
	float frictionCoefficient = 0.25f;
	
	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		app.showFPS();
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
			// Move all spheres and update cells
			for (Sphere* s : spheres)
			{
				// Integration (translation)
				s->setAcc(s->applyForces(s->getPos(), s->getVel(), t, deltaTime));
				s->setVel(s->getVel() + deltaTime * s->getAcc());
				s->translate(s->getVel() * deltaTime);

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
									if (s->getPos()[i] + s->getRadius() >= table.getPos()[i] + tableSize)
									{
										translation[i] = -((s->getPos()[i] + s->getRadius()) - (table.getPos()[i] + tableSize));
										collisionPoint[i] = table.getPos()[i] + tableSize;
										normal[i] = -1.0f;
										tableCollision = true;
										break;
									}
									else if (s->getPos()[i] - s->getRadius() <= table.getPos()[i] - tableSize)
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
								// Calculate the normal impulse
								float jn = (-(1.0f + e) * glm::dot(vr, n)) / ((1 / s->getMass()) + glm::dot(n, (glm::cross(s->getInvInertia()* glm::cross(r, n), r))));
								// Calculate new velocities
								s->setVel(s->getVel() + (jn / s->getMass())*n);
							}
						}

						//Sphere with sphere collisions if there are more than one in the cell
						if (grid[i][j].size() > 1)
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
										// Calculate relative velocity
										glm::vec3 vr = sColliding->getVel() - s->getVel();
										// Calculate the normal impulse
										float jn = (-(1.0f + e) * glm::dot(vr, n)) / ((1 / s->getMass()) + (1 / sColliding->getMass()));
										// Calculate new velocities
										s->setVel(s->getVel() - (jn*n / s->getMass()));
										sColliding->setVel(sColliding->getVel() + (jn*n / sColliding->getMass()));
									}
								}
							}
						}
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
		for (Sphere* s : spheres)
		{
			app.draw(s->getMesh());
		}

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}
