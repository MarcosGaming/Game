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
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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
	
	// time
	GLfloat firstFrame = (GLfloat) glfwGetTime();

	// Gravity constant
	glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);

	// PARTICLE BOUNCING INSIDE CUBE VARIABLES
	/*Particle particleInCube = Particle::Particle();
	particleInCube.translate(glm::vec3(0.0f, 2.5f, 0.0f));
	particleInCube.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
	particleInCube.setMass(2.0f);
	particleInCube.setAcc(glm::vec3(0.0f));
	particleInCube.setVel(glm::vec3(10.0f, 8.0f, 5.0f));
	glm::vec3 cubeCorner = glm::vec3(2.5f, 5.0f, 2.5f);
	glm::vec3 cubeDimensions = glm::vec3(5.0f);
	float energyDrain = 1.05f;*/
	
	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		// Set frame time
		GLfloat currentFrame = (GLfloat)  glfwGetTime() - firstFrame;
		// the animation can be sped up or slowed down by multiplying currentFrame by a factor.
		currentFrame *= 1.5f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*
		**	INTERACTION
		*/
		// Manage interaction
		app.doMovement(deltaTime);


		/*
		**	SIMULATION
		*/
		
		// PARTICLE BOUNCING INSIDE CUBE
		// Add forces
		/*glm::vec3 forceg = particleInCube.getMass() * g;
		glm::vec3 totalForce = forceg;
		// Compute acceleration
		particleInCube.setAcc(totalForce / particleInCube.getMass());
		// Integrate to calculate new velocity and position
		particleInCube.setVel( particleInCube.getVel() + particleInCube.getAcc() * deltaTime);
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


		/*
		**	RENDER 
		*/		
		// clear buffer
		app.clear();
		// draw groud plane
		app.draw(plane);

		// PARTICLE BOUNCING INSIDE CUBE
		//app.draw(particleInCube.getMesh());

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

