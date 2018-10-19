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

// Gravity constant
glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);
Gravity* fgravity = new Gravity(g);

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


	// TASK 2.1 CHAIN OF PARTICLES CONNECTED BY SPRINGS VARIABLES
	/*Particle particleStatic = Particle::Particle();
	particleStatic.translate(glm::vec3(0.0f, 8.0f, 0.0f));
	particleStatic.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
	// Particles and forces
	std::vector<Particle> particles;
	particles.push_back(particleStatic);
	int numberParticles = 5;
	for (int i = 1; i < numberParticles; i++)
	{
		Particle particle = Particle::Particle();
		particle.translate(glm::vec3(1.0f * i, 8.0f, 0.0f));
		particle.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
		particles.push_back(particle);
	}
	for (int i = 1; i < numberParticles; i++)
	{
		
		particles[i].addForce(fgravity);
		Hooke* fsd = new Hooke(&particles[i], &particles[i - 1], 10.0f * i * i, 5.0f, 1.0f);
		particles[i].addForce(fsd);
		if (i+1 < numberParticles)
		{
			Hooke* fsd2 = new Hooke(&particles[i], &particles[i + 1], 10.0f * i * i, 5.0f, 1.0f);
			particles[i].addForce(fsd2);
		}
	}*/

	// TASK 2.2 AND 2.3 CHAIN OF PARTICLES CONNECTED BY DAMPED SPRINGS VARIABLES
	/*Particle particleStatic1 = Particle::Particle();
	particleStatic1.translate(glm::vec3(-5.0f, 5.0f, 0.0f));
	particleStatic1.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
	Particle particleStatic2 = Particle::Particle();
	particleStatic2.translate(glm::vec3(5.0f, 5.0f, 0.0f));
	particleStatic2.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
	// Particles and forces
	std::vector<Particle> particles;
	int numberParticles = 10;
	particles.push_back(particleStatic1);
	for (int i = 1; i < numberParticles-1; i++)
	{
		Particle particle = Particle::Particle();
		particle.translate(glm::vec3(particles[i-1].getPos().x + 1.0f, particleStatic1.getPos().y + 2.0f, 0.0f));
		particle.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
		particles.push_back(particle);
	}
	particles.push_back(particleStatic2);
	for (int i = 1; i < numberParticles; i++)
	{
		particles[i].addForce(fgravity);
		Hooke* fsd = new Hooke(&particles[i], &particles[i - 1], 20.0f, 2.0f, 1.0f);
		particles[i].addForce(fsd);
		if (i + 1 < numberParticles)
		{
			Hooke* fsd2 = new Hooke(&particles[i], &particles[i + 1], 20.0f, 2.0f, 1.0f);
			particles[i].addForce(fsd2);
		}
	}*/

	// TASK 2.4 CLOTH SIMULATION 1 VARIABLES
	// Create Particles
	/*Particle particles[10][10];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Particle particle = Particle::Particle();
			particle.translate(glm::vec3(-5.0f + i, 8.0f, -5.0f + j));
			particle.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
			particles[i][j] = particle;
		}
	}
	// Add forces to particles
	int numberOfParticlesWithForces = 10;
	float ks = 20.0f;
	float kd = 2.0f;
	float rest = 1.0f;
	for (int i = 1; i < numberOfParticlesWithForces; i++)
	{
		for (int j = 1; j < numberOfParticlesWithForces; j++)
		{
			particles[i][j].addForce(fgravity);
			// 4 connections for all particles
			Hooke* fsd1 = new Hooke(&particles[i][j], &particles[i - 1][j], ks, kd, rest);
			particles[i][j].addForce(fsd1);
			Hooke* fsd2 = new Hooke(&particles[i][j], &particles[i + 1][j], ks, kd, rest);
			particles[i][j].addForce(fsd2);
			Hooke* fsd3 = new Hooke(&particles[i][j], &particles[i][j - 1], ks, kd, rest);
			particles[i][j].addForce(fsd3);
			Hooke* fsd4 = new Hooke(&particles[i][j], &particles[i][j + 1], ks, kd, rest);
			particles[i][j].addForce(fsd4);
		}
	}*/

	// TASK 2.5 CLOTH SIMULATION 2 VARIABLES
	// Create particles
	Particle particles[10][10];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Particle particle = Particle::Particle();
			particle.translate(glm::vec3(-4.5f + i, 10.0f - (j/2.0f), 0.0f));
			particle.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
			particles[i][j] = particle;
		}
	}
	// Add forces to particles
	int numberOfParticlesWithForces = 10;
	float ks = 15.0f;
	float kd = 5.0f;
	float rest = 0.5f;
	for (int i = 0; i < numberOfParticlesWithForces; i++)
	{
		for (int j = 1; j < numberOfParticlesWithForces; j++)
		{
			particles[i][j].addForce(fgravity);
			// 4 connections for all particles
			Hooke* fsd1 = new Hooke(&particles[i][j], &particles[i][j-1], ks, kd, rest);
			particles[i][j].addForce(fsd1);
			// Checking for the last row
			if (j + 1 < numberOfParticlesWithForces)
			{
				Hooke* fsd2 = new Hooke(&particles[i][j], &particles[i][j + 1], ks, kd, rest);
				particles[i][j].addForce(fsd2);
			}
			// Checking for the first column
			if (i - 1 >= 0)
			{
				Hooke* fsd3 = new Hooke(&particles[i][j], &particles[i - 1][j], ks, kd, rest);
				particles[i][j].addForce(fsd3);
			} 
			// Checking for the last column
			if (i + 1 < numberOfParticlesWithForces)
			{
				Hooke* fsd4 = new Hooke(&particles[i][j], &particles[i + 1][j], ks, kd, rest);
				particles[i][j].addForce(fsd4);
			}
		}
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
			//TASK 2.1 CHAIN OF PARTICLES CONNECTED BY SPRINGS
			/*for (int i = 1; i < numberParticles; i++)
			{
				// Calculate acceleration
				particles[i].setAcc(particles[i].applyForces(particles[i].getPos(), particles[i].getVel(), t, deltaTime));
				// Integrate to calculate new velocity and position
				particles[i].setVel(particles[i].getVel() + particles[i].getAcc() * deltaTime);
				particles[i].translate(particles[i].getVel() * deltaTime);
				if (particles[i].getPos().y < plane.getPos().y)
				{
					particles[i].setPos(1, plane.getPos().y);
					particles[i].setVel(1, -particles[i].getVel().y);
				}
			}*/

			// TASK 2.2 AND 2.3 CHAIN OF PARTICLES CONNECTED BY DAMPED SPRINGS 
			/*for (int i = 1; i < numberParticles-1; i++)
			{
				// Calculate acceleration
				particles[i].setAcc(particles[i].applyForces(particles[i].getPos(), particles[i].getVel(), t, deltaTime));
				// Integrate to calculate new velocity and position
				particles[i].setVel(particles[i].getVel() + particles[i].getAcc() * deltaTime);
				particles[i].translate(particles[i].getVel() * deltaTime);
				// Plane collision
				if (particles[i].getPos().y <= plane.getPos().y)
				{
					particles[i].setPos(1, plane.getPos().y);
					particles[i].setVel(1, -particles[i].getVel().y/2.0f);
				}
			}*/

			// TASK 2.4 CLOTH SIMULATION 1 
			/*for (int i = 1; i < numberOfParticlesWithForces - 1; i++)
			{
				for (int j = 1; j < numberOfParticlesWithForces - 1; j++)
				{
					// Calculate acceleration
					particles[i][j].setAcc(particles[i][j].applyForces(particles[i][j].getPos(), particles[i][j].getVel(), t, deltaTime));
					// Integrate to calculate new velocity and position
					particles[i][j].setVel(particles[i][j].getVel() + particles[i][j].getAcc() * deltaTime);
					particles[i][j].translate(particles[i][j].getVel() * deltaTime);
					// Plane collision
					if (particles[i][j].getPos().y <= plane.getPos().y)
					{
						particles[i][j].setPos(1, plane.getPos().y);
						particles[i][j].setVel(1, -particles[i][j].getVel().y);
					}
				}
			}*/

			// TASK 2.5 CLOTH SIMULATION 2
			for (int i = 0; i < numberOfParticlesWithForces; i++)
			{
				for (int j = 0; j < numberOfParticlesWithForces; j++)
				{
					// Calculate acceleration
					particles[i][j].setAcc(particles[i][j].applyForces(particles[i][j].getPos(), particles[i][j].getVel(), t, deltaTime));
					// Integrate to calculate new velocity and position
					particles[i][j].setVel(particles[i][j].getVel() + particles[i][j].getAcc() * deltaTime);
					particles[i][j].translate(particles[i][j].getVel() * deltaTime);
					// Plane collision
					if (particles[i][j].getPos().y <= plane.getPos().y)
					{
						particles[i][j].setPos(1, plane.getPos().y);
						particles[i][j].setVel(1, -particles[i][j].getVel().y);
					}
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

		//TASK 2.1 CHAIN OF PARTICLES CONNECTED BY SPRINGS DRAW
		/*for(int i = 0; i < numberParticles; i++)
		{
			app.draw(particles[i].getMesh());
		}
		app.draw(plane);*/

		// TASK 2.2 AND 2.3 CHAIN OF PARTICLES CONNECTED BY DAMPED SPRINGS DRAW
		/*for (int i = 0; i < numberParticles; i++)
		{
			app.draw(particles[i].getMesh());
		}
		app.draw(plane);*/

		// TASK 2.4 CLOTH SIMULATION 1 DRAW
		/*app.draw(plane);
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				app.draw(particles[i][j].getMesh());
			}
		}*/

		// TASK 2.5 CLOTH SIMULATION 2 DRAW
		app.draw(plane);
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				app.draw(particles[i][j].getMesh());
			}
		}

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

