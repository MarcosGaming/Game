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

// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Body.h"
#include "Particle.h"


// time
GLfloat t = 0.0f;
const GLfloat deltaTime = 0.01f;
GLfloat currentTime = (GLfloat)glfwGetTime();
GLfloat accumulator = 0.0f;

// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 20.0f));

	// create ground plane
	Mesh plane = Mesh::Mesh(Mesh::QUAD);
	// scale it up x5
	plane.scale(glm::vec3(5.0f, 5.0f, 5.0f));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	plane.setShader(lambert);

	// Gravity constant
	glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);

	// TASK 2 PARTICLE BOUNCING INSIDE CUBE VARIABLES
	// Cube
	/*Mesh cube = Mesh::Mesh(Mesh::CUBE);
	cube.scale(glm::vec3(2.5f, 2.5f, 2.5f));
	cube.translate(glm::vec3(0.0f, 2.5f, 0.0f));
	cube.setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_transparent.frag"));
	glm::vec3 cubeCorner = glm::vec3(2.5f, 5.0f, 2.5f);
	glm::vec3 cubeDimensions = glm::vec3(5.0f);
	// Particle
	Particle particleInCube = Particle::Particle();
	particleInCube.translate(glm::vec3(0.0f, 2.5f, 0.0f));
	particleInCube.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
	particleInCube.setMass(2.0f);
	particleInCube.setAcc(glm::vec3(0.0f));
	particleInCube.setVel(glm::vec3(10.0f, 8.0f, 5.0f));
	float energyDrain = 1.05f;*/

	// TASK 3 INTEGRATION METHODS VARIABLES
	// Static particle
	/*Particle staticParticle = Particle::Particle();
	staticParticle.translate(glm::vec3(-2.0f, 2.5f, 0.0f));
	staticParticle.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
	//Moving particles
	std::vector<Particle> movingParticles;
	for (int i = 0; i < 2; i++)
	{
		Particle movingParticle = Particle::Particle();
		movingParticle.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
		movingParticle.setAcc(glm::vec3(0.0f));
		movingParticle.setMass(2.0f);
		movingParticle.setVel(glm::vec3(0.0f, 5.0f, 0.0f));
		movingParticles.push_back(movingParticle);
	}
	movingParticles[0].translate(glm::vec3(0.0f, 2.5f, 0.0f));
	movingParticles[1].translate(glm::vec3(2.0f, 2.5f, 0.0f));*/

	// TASK 4 BLOW DRYER VARIABLES
	// Cube
	Mesh cube = Mesh::Mesh(Mesh::CUBE);
	cube.scale(glm::vec3(2.5f, 2.5f, 2.5f));
	cube.translate(glm::vec3(0.0f, 2.5f, 0.0f));
	cube.setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_transparent.frag"));
	glm::vec3 cubeCorner = glm::vec3(2.5f, 5.0f, 2.5f);
	glm::vec3 cubeDimensions = glm::vec3(5.0f);
	// Particles 
	Particle particleInCube = Particle::Particle();
	particleInCube.translate(glm::vec3(0.0f, 2.5f, 0.0f));
	particleInCube.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
	particleInCube.setMass(2.0f);
	particleInCube.setAcc(glm::vec3(0.0f));
	particleInCube.setVel(glm::vec3(10.0f, 8.0f, 5.0f));
	float energyDrain = 1.05f;
	// Blow Dryer
	glm::vec3 dryerPeak = glm::vec3(0.0f, -2.0f, 0.0f);
	float dryerHeight = 4.0f;
	float dryerRadius = 1.5f;
	float faero = 0.5f * 0.125f * (200.0f * 200.0f) * 1.05f * 0.1f;

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
			// TASK 2 PARTICLE BOUNCING INSIDE CUBE
			// Add forces
			/*glm::vec3 forceg = particleInCube.getMass() * g;
			glm::vec3 totalForce = forceg;
			// Compute acceleration
			particleInCube.setAcc(totalForce / particleInCube.getMass());
			// Integrate to calculate new velocity and position
			particleInCube.setVel(particleInCube.getVel() + particleInCube.getAcc() * deltaTime);
			particleInCube.translate(particleInCube.getVel() * deltaTime);
			for (int i = 0; i < 3; i++)
			{
				if (particleInCube.getPos()[i] >= cubeCorner[i])
				{
					glm::vec3 particlePosition = glm::vec3(particleInCube.getPos().x, particleInCube.getPos().y, particleInCube.getPos().z);
					particlePosition[i] = cubeCorner[i];
					particleInCube.setPos(particlePosition);
					particleInCube.setVel(i, -particleInCube.getVel()[i] / energyDrain);
				}
				else if (particleInCube.getPos()[i] <= (cubeCorner[i] - cubeDimensions[i]))
				{
					glm::vec3 particlePosition = glm::vec3(particleInCube.getPos().x, particleInCube.getPos().y, particleInCube.getPos().z);
					particlePosition[i] = cubeCorner[i] - cubeDimensions[i];
					particleInCube.setPos(particlePosition);
					particleInCube.setVel(i, -particleInCube.getVel()[i] / energyDrain);
				}
			}*/

			// TASK 3 INTEGRATION METHODS
			/*for (int i = 0; i < 2; i++)
			{
				// Add forces
				glm::vec3 forceg = movingParticles[i].getMass() * g;
				// Compute acceleration
				movingParticles[i].setAcc(forceg / movingParticles[i].getMass());
				// Calculate new velocity and position (Semi-Implicit Euler Integration)
				if (i == 0)
				{
					movingParticles[i].setVel(movingParticles[i].getVel() + deltaTime * movingParticles[i].getAcc());
					movingParticles[i].translate(deltaTime * movingParticles[i].getVel());
				}
				// Calculate new velocity and position (Forward Euler Integration)
				else
				{
					glm::vec3 previousVel = movingParticles[i].getVel();
					movingParticles[i].setVel(movingParticles[i].getVel() + deltaTime * movingParticles[i].getAcc());
					movingParticles[i].translate(deltaTime * previousVel);
				}
				// Collision
				if (movingParticles[i].getPos().y < plane.getPos().y)
				{
					movingParticles[i].setPos(1, plane.getPos().y);
					movingParticles[i].setVel(1, -movingParticles[i].getVel().y);
				}
			}*/

			// TASK 4 BLOW DRYER
			// Add forces
			// Calculate blow dryer force
			glm::vec3 fdryer = glm::vec3(0.0f);
			// Check if the particle its in the y range of the cone
			if (particleInCube.getPos().y < (dryerHeight + dryerPeak.y))
			{
				// Check if the particle its in the x and z ranges
				float newHeight = particleInCube.getPos().y + glm::abs(dryerPeak.y);
				float radiusRange = (newHeight * dryerRadius) / dryerHeight;
				if ((particleInCube.getPos().x < (dryerPeak.x + radiusRange) && particleInCube.getPos().x >(dryerPeak.x - radiusRange)) && (particleInCube.getPos().z < (dryerPeak.z + radiusRange) && particleInCube.getPos().z >(dryerPeak.z - radiusRange)))
				{
					// Use the normalize direction of the particle to apply the force
					glm::vec3 direction = glm::normalize(particleInCube.getPos() - dryerPeak);
					fdryer = direction * faero;
					// Reduce the force base on how far of the cone the particle is
					if (particleInCube.getPos().x != 0.0f && particleInCube.getPos().y != 0.0f && particleInCube.getPos().z != 0.0f)
					{
						fdryer.x /= glm::abs(particleInCube.getPos().x * 4.0f);
						fdryer.y /= glm::abs(particleInCube.getPos().y * 2.0f);
						fdryer.z /= glm::abs(particleInCube.getPos().z * 4.0f);
					}
				}
			}
			glm::vec3 fgravity = particleInCube.getMass() * g;
			glm::vec3 totalForce = fgravity + fdryer;
			// Compute acceleration
			particleInCube.setAcc(totalForce / particleInCube.getMass());
			// Integrate to calculate new velocity and position
			particleInCube.setVel(particleInCube.getVel() + particleInCube.getAcc() * deltaTime);
			particleInCube.translate(particleInCube.getVel() * deltaTime);
			// Cube collisions
			for (int i = 0; i < 3; i++)
			{
				if (particleInCube.getPos()[i] >= cubeCorner[i])
				{
					glm::vec3 particlePosition = glm::vec3(particleInCube.getPos().x, particleInCube.getPos().y, particleInCube.getPos().z);
					particlePosition[i] = cubeCorner[i];
					particleInCube.setPos(particlePosition);
					particleInCube.setVel(i, -particleInCube.getVel()[i] / energyDrain);
				}
				else if (particleInCube.getPos()[i] <= (cubeCorner[i] - cubeDimensions[i]))
				{
					glm::vec3 particlePosition = glm::vec3(particleInCube.getPos().x, particleInCube.getPos().y, particleInCube.getPos().z);
					particlePosition[i] = cubeCorner[i] - cubeDimensions[i];
					particleInCube.setPos(particlePosition);
					particleInCube.setVel(i, -particleInCube.getVel()[i] / energyDrain);
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
		app.draw(plane);

		// TASK 2 PARTICLE BOUNCING INSIDE CUBE
		/*app.draw(particleInCube.getMesh());
		app.draw(cube);*/

		// TASK 3 INTEGRATION METHODS
		/*app.draw(staticParticle.getMesh());
		for (int i = 0; i < 2; i++)
		{
			app.draw(movingParticles[i].getMesh());
		}*/

		// TASK 4 BLOW DRYER
		app.draw(particleInCube.getMesh());
		app.draw(cube);

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

