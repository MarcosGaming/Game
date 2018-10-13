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

	// TASK 2.2 HOOKE'S LAW IMPLEMENTATION VARIABLES
	/*Particle particle2 = Particle::Particle();
	particle2.translate(glm::vec3(0.0f, 5.0f, 0.0f));
	particle2.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
	Particle particle1 = Particle::Particle();
	particle1.translate(glm::vec3(0.0f, 4.0f, 0.0f));
	particle1.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
	particle1.addForce(&Gravity::Gravity(glm::vec3(0.0f, -9.8f, 0.0f)));
	Hooke fsd = Hooke::Hooke(&particle1, &particle2, 10.0f, 0.1f, 1.0f);
	particle1.addForce(&fsd);*/

	//TASK 2.3 CHAIN OF PARTICLES CONNECTED BY SPRINGS VARIABLES
	Particle particleStatic = Particle::Particle();
	particleStatic.translate(glm::vec3(0.0f, 8.0f, 0.0f));
	particleStatic.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));

	std::vector<Particle> particles;
	particles.push_back(particleStatic);
	int numberParticles = 5;
	for (int i = 1; i < numberParticles; i++)
	{
		Particle particle = Particle::Particle();
		particle.translate(glm::vec3(0.0f, (particles[i-1].getPos().y-1.0f), 0.0f));
		particle.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
		particles.push_back(particle);
	}
	for (int i = 1; i < numberParticles; i++)
	{
		Gravity* fgravity = new Gravity(glm::vec3(0.0f, -9.8f, 0.0f));
		particles[i].addForce(fgravity);
		Hooke* fsd = new Hooke(&particles[i], &particles[i - 1], 20.0f * i * i, 0.01f, 1.0f);
		particles[i].addForce(fsd);
	}

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
			// TASK 2.2 HOOKE'S LAW IMPLEMENTATION
			// Calculate acceleration
			/*particle1.setAcc(particle1.applyForces(particle1.getPos(), particle1.getVel(), t, deltaTime));
			// Integrate to calculate new velocity and position
			particle1.setVel(particle1.getVel() + particle1.getAcc() * deltaTime);
			particle1.translate(particle1.getVel() * deltaTime);*/

			//TASK 2.3 CHAIN OF PARTICLES CONNECTED BY SPRINGS
			for (int i = 0; i < numberParticles; i++)
			{
				// Calculate acceleration
				particles[i].setAcc(particles[i].applyForces(particles[i].getPos(), particles[i].getVel(), t, deltaTime));
				// Integrate to calculate new velocity and position
				particles[i].setVel(particles[i].getVel() + particles[i].getAcc() * deltaTime);
				particles[i].translate(particles[i].getVel() * deltaTime);
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

		// TASK 2.2 HOOKE'S LAW IMPLEMENTATION DRAW
		/*app.draw(particle2.getMesh());
		app.draw(particle1.getMesh());*/

		//TASK 2.3 CHAIN OF PARTICLES CONNECTED BY SPRINGS DRAW
		for (int i = 0; i < numberParticles; i++)
		{
			app.draw(particles[i].getMesh());
		}

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

