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

	#pragma region Task_1_variables

	// Set up a cubic rigid body
	/*RigidBody rb = RigidBody();
	Mesh m = Mesh::Mesh(Mesh::CUBE);
	rb.setMesh(m);
	Shader rbShader = Shader("resources/shaders/physics.vert ", "resources/shaders/physics.frag ");
	rb.getMesh().setShader(rbShader);
	rb.setMass(2.0f);
	rb.scale(glm::vec3(1.0f, 3.0f, 1.0f));

	// rigid body motion values
	rb.translate(glm::vec3(0.0f, 5.0f, 0.0f));
	rb.setVel(glm::vec3(2.0f, 0.0f, 0.0f));
	rb.setAngVel(glm::vec3(0.0f, 0.0f, 0.0f));

	// impulses
	bool impulsed = false;
	glm::vec3 j = glm::vec3(-4.0f, 0.0f, 0.0f);

	// output inverse inertia matrix
	std::cout << glm::to_string(rb.getInvInertia()) << '\n';*/

	#pragma endregion Task_1_variables

	#pragma region Task_2_and_3_variables

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
	rb.setVel(glm::vec3(0.0f, 0.0f, 0.0f));
	rb.setAngVel(glm::vec3(0.0f, 0.0f, 0.5f));

	// add forces
	rb.addForce(fgravity);

	// impulse elements
	float e = 1.0f;
	bool collision = false;
	int vertexCount = 0;
	glm::vec3 verticesInCollision[4];*/

	#pragma endregion Task_2_and_3_variables

	#pragma region Task_4_variables

	RigidBody rb = RigidBody();
	Mesh m = Mesh::Mesh(Mesh::CUBE);
	rb.setMesh(m);
	Shader rbShader = Shader("resources/shaders/physics.vert ", "resources/shaders/physics.frag ");
	rb.getMesh().setShader(rbShader);
	rb.setMass(1.0f);
	rb.scale(glm::vec3(1.0f, 3.0f, 1.0f));

	// rigid body motion values
	rb.translate(glm::vec3(0.0f, 20.0f, 0.0f));
	rb.setVel(glm::vec3(2.0f, 0.0f, 1.0f));
	rb.setAngVel(glm::vec3(2.0f, 2.0f, 2.0f));

	// this is used to decrease the velocity
	float initialVelocity = glm::length(rb.getVel()) + 0.2f;

	// add forces
	rb.addForce(fgravity);

	// impulse elements
	float e = 0.6f;
	bool collision = false;
	int vertexCount = 0;
	glm::vec3 verticesInCollision[4];
	float frictionCoefficient = 0.25f;

	#pragma endregion Task_4_variables
	
	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		// Set frame time
		GLfloat newTime = (GLfloat)glfwGetTime();
		GLfloat frameTime = newTime - currentTime;
		currentTime = newTime;
		accumulator += frameTime;
		accumulator *= 1.5f;

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
			#pragma region Task_1_simulation
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

			// impulses
			if (currentTime > 2.0f && !impulsed)
			{
				rb.setVel(rb.getVel() + j / rb.getMass());
				rb.setAngVel(rb.getAngVel() + rb.getInvInertia() * glm::cross(rb.getPos() + glm::vec3(1.0f, 2.0f, 0.0f) - rb.getPos(), j));
				impulsed = true;
			}*/

			#pragma endregion Task_1_simulation

			#pragma region Task_2_and_3_simulation

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
			if(collision)
			{
				std::cout << "Colliding Vertices : " << '\n';
				glm::vec3 collisionPoint;
				// Vertex
				if (vertexCount == 1)
				{
					collisionPoint = verticesInCollision[0];
					// output coordinates of colliding vertices
					std::cout << glm::to_string(verticesInCollision[0]) << '\n';
				}
				// Edge
				else if (vertexCount == 2)
				{
					collisionPoint = (verticesInCollision[0] + verticesInCollision[1]) / 2;
					// output coordinates of colliding vertices
					std::cout << glm::to_string(verticesInCollision[0]) << '\n';
					std::cout << glm::to_string(verticesInCollision[1]) << '\n';
				}
				// Face
				else if (vertexCount == 4)
				{
					collisionPoint = (verticesInCollision[0] + verticesInCollision[1] + verticesInCollision[2] + verticesInCollision[3]) / 4;
					// output coordinates of colliding vertices
					std::cout << glm::to_string(verticesInCollision[0]) << '\n';
					std::cout << glm::to_string(verticesInCollision[1]) << '\n';
					std::cout << glm::to_string(verticesInCollision[2]) << '\n';
					std::cout << glm::to_string(verticesInCollision[3]) << '\n';
				}
				// reset count and boolean
				vertexCount = 0;
				collision = false;
				// output average of all cordinates
				std::cout << "Average of all coordinates : " << '\n';
				std::cout << glm::to_string(collisionPoint) << '\n';
				// Translate the lowest vertex a bit so it does not overlap the plane
				rb.translate(glm::vec3(0.0f, glm::abs(plane.getPos().y - lowestCoordinate), 0.0f));
				// r is the vector of the centre of mass and the point of collision
				glm::vec3 r = collisionPoint - rb.getPos();
				// n is the normal of the plane of collision, in this case the normal of the plane
				glm::vec3 n = glm::vec3(0.0f, 1.0f, 0.0f);
				// vr is the relative velocity
				glm::vec3 vr = rb.getVel() + glm::cross(rb.getAngVel(), r);
				// calculate the impulse
				float jr = (-(1.0f + e) * glm::dot(vr, n)) / ((1 / rb.getMass()) + glm::dot(n,(glm::cross(rb.getInvInertia()* glm::cross(r, n), r))));
				// set the new velocities
				rb.setVel(rb.getVel() + (jr / rb.getMass())*n);
				rb.setAngVel(rb.getAngVel() + jr * rb.getInvInertia() * glm::cross(r, n));

				// create skew symmetric matrix for w
				angVelSkew = glm::matrixCross3(rb.getAngVel());
				// create 3x3 rotation matrix from rb rotation matrix
				R = glm::mat3(rb.getRotate());
				// update rotation matrix
				R += deltaTime * angVelSkew *R;
				R = glm::orthonormalize(R);
				rb.setRotate(glm::mat4(R));
			}*/

			#pragma endregion Task_2_and_3_simulation

			#pragma region Task_4_simulation

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
			}

			#pragma endregion Task_4_simulation

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

		app.draw(rb.getMesh());

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

