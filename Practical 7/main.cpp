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
#include "RigidBody.h"


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

	#pragma region Task_2_variables

	// Set up a cubic rigid body
	/*RigidBody rb = RigidBody();
	Mesh m = Mesh::Mesh(Mesh::CUBE);
	rb.setMesh(m);
	Shader rbShader = Shader("resources/shaders/physics.vert ", "resources/shaders/physics.frag ");
	rb.getMesh().setShader(rbShader);
	rb.setMass(1.0f);
	rb.scale(glm::vec3(1.0f, 3.0f, 1.0f));

	// rigid body motion values
	rb.translate(glm::vec3(0.0f, 5.0f, 0.0f));
	rb.setVel(glm::vec3(3.0f, 0.0f, 0.0f));
	rb.setAngVel(glm::vec3(0.0f, 0.0f, 1.0f));

	// impulses
	bool impulsed = false;
	glm::vec3 j = glm::vec3(-3.0f, 0.0f, 0.0f);*/

	#pragma endregion Task_2_variables

	#pragma region Task_3_variables

	// Set up a cubic rigid body
	RigidBody rb = RigidBody();
	Mesh m = Mesh::Mesh(Mesh::CUBE);
	rb.setMesh(m);
	Shader rbShader = Shader("resources/shaders/physics.vert ", "resources/shaders/physics.frag ");
	rb.getMesh().setShader(rbShader);
	rb.setMass(1.0f);
	rb.scale(glm::vec3(1.0f, 3.0f, 1.0f));

	// rigid body motion values
	rb.translate(glm::vec3(0.0f, 5.0f, 0.0f));
	rb.setVel(glm::vec3(0.0f, 0.0f, 0.0f));
	rb.setAngVel(glm::vec3(0.0f, 0.0f, 0.0f));

	// add forces
	rb.addForce(fgravity);

	// impulse elements
	float e = 1.0f;
	bool collision = false;
	int vertexCount = 0;
	glm::vec3 *verticesInCollision[4];

	#pragma endregion Task_3_variables
	
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
			#pragma region Task_2_simulation
			// integration (translation)
			/*rb.setAcc(rb.applyForces(rb.getPos(), rb.getVel(), t, deltaTime));
			rb.setVel(rb.getVel() + deltaTime * rb.getAcc());
			rb.translate(rb.getVel() * deltaTime);

			// integration ( rotation )
			rb.setAngVel(rb.getAngVel() + deltaTime * rb.getAngAcc());
			// create skew symmetric matrix for w
			glm::mat3 angVelSkew = glm::matrixCross3(rb.getAngVel());
			// create 3x3 rotation matrix from rb rotation matrix
			glm::mat3 R = glm::mat3(rb.getRotate());
			// update rotation matrix
			R += deltaTime * angVelSkew *R;
			R = glm::orthonormalize(R);
			rb.setRotate(glm::mat4(R));

			// Plane collision
			for (auto vertex : rb.getMesh().getVertices())
			{
				glm::vec3 coordinates = rb.getMesh().getModel() * glm::vec4(vertex.getCoord(), 1.0f);
				if (coordinates.y <= plane.getPos().y)
				{
					rb.setVel(1, -rb.getVel().y);
					rb.translate(rb.getVel() * deltaTime);
				}
			}

			// impulses
			if (currentTime > 2.0f && !impulsed)
			{
				rb.setVel(rb.getVel() + j / rb.getMass());
				rb.setAngVel(rb.getInvInertia() * glm::cross(rb.getPos() + glm::vec3(1.0f, 0.0f, 0.0f) - rb.getPos(), j));
				impulsed = true;
			}*/

			#pragma endregion Task_2_simulation

			#pragma region Task_3_simulation

			// integration (translation)
			rb.setAcc(rb.applyForces(rb.getPos(), rb.getVel(), t, deltaTime));
			rb.setVel(rb.getVel() + deltaTime * rb.getAcc());
			rb.translate(rb.getVel() * deltaTime);

			// integration ( rotation )
			rb.setAngVel(rb.getAngVel() + deltaTime * rb.getAngAcc());
			// create skew symmetric matrix for w
			glm::mat3 angVelSkew = glm::matrixCross3(rb.getAngVel());
			// create 3x3 rotation matrix from rb rotation matrix
			glm::mat3 R = glm::mat3(rb.getRotate());
			// update rotation matrix
			R += deltaTime * angVelSkew *R;
			R = glm::orthonormalize(R);
			rb.setRotate(glm::mat4(R));

			// Check how many vertex are in collision with the plane 
			for (auto vertex : rb.getMesh().getVertices())
			{
				glm::vec3 coordinates = rb.getMesh().getModel() * glm::vec4(vertex.getCoord(), 1.0f);
				if (coordinates.y <= plane.getPos().y)
				{
					verticesInCollision[vertexCount] = &coordinates;
					vertexCount++;
					collision = true;
				}
			}
			if(collision)
			{
				glm::vec3 collisionPoint;
				// Vertex
				if (vertexCount == 1)
				{
					collisionPoint = *verticesInCollision[0];
				}
				// Edge
				else if (vertexCount == 2)
				{
					collisionPoint = (*verticesInCollision[0] + *verticesInCollision[1]) / 2;
				}
				// Face
				else if (vertexCount == 4)
				{
					collisionPoint = glm::vec3(rb.getPos().x, rb.getPos().y - 3.0f, rb.getPos().z);
				}
				vertexCount = 0;
				collision = false;
				// Translate the rigid body up a bit so it does not overlap the plane
				rb.translate(glm::vec3(0.0f, 0.5f, 0.0f));
				// r is the vector of the centre of mass and the point of collision
				glm::vec3 r = collisionPoint - rb.getPos();
				// n is the normal of the plane of collision, in this case the normal of the plane
				glm::vec3 n = glm::vec3(0.0f, 1.0f, 0.0f);
				// vr is the relative velocity
				glm::vec3 vr = rb.getVel() + glm::cross(rb.getAngVel(), r);
				// calculate the impulse
				glm::vec3 jr = (-(1.0f + e) * vr * n) / ((1 / rb.getMass()) + n * (rb.getInvInertia()*glm::cross(glm::cross(r, n), r)));
				// set the new velocities
				rb.setVel(rb.getVel() + (jr / rb.getMass())*n);
				rb.setAngVel(rb.getAngVel() + jr * rb.getInvInertia() * glm::cross(r, n));
			}

			#pragma endregion Task_3_simulation

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

		#pragma region Task_2_draw

		//app.draw(rb.getMesh());

		#pragma endregion Task_2_draw

		#pragma region Task_3_draw

		app.draw(rb.getMesh());

		#pragma endregion Task_3_draw


		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

