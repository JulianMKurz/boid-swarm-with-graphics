// Simplified Renderer application for GP course
// Code is similar to the one in lab 1 but all the graphics sections were refactored into the Graphics Class.
// Extra improvements:
// Reduced OpenGL version from 4.5 to 3.3 to allow it to render in older laptops.
// Added Shapes library for rendering cubes, spheres and vectors.
// Added examples of matrix multiplication on Update.
// Added resize screen and keyboard callbacks.
// 
// Update 2018/01 updated libraries and created project for VS2015.

// Suggestions or extra help please do email me S.Padilla@hw.ac.uk
//
// Note: Do not forget to link the libraries correctly and add the GLEW DLL in your debug/release folder.

#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


#include "graphics.h"
#include "shapes.h"
#include "Swarm.h"

// FUNCTIONS
void render(double currentTime);
void update(double currentTime);
void startup();
void onResizeCallback(GLFWwindow* window, int w, int h);
void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// VARIABLES
bool		running = true;

Graphics	myGraphics;		// Runing all the graphics in this object

Cube		myCube;
Sphere		mySphere;
Arrow		arrowX;
Arrow		arrowY;
Arrow		arrowZ;

float t = 0.001f;			// Global variable for animation
float leftRight = 1.0f;
std::vector<Boid> boids;
std::vector<Cube> visualBoids;
Swarm swarm;
float prevUpdateTime = 0;


int main()
{
	int errorGraphics = myGraphics.Init();		// Launch window and graphics context
	if (errorGraphics) return 0;				//Close if something went wrong...

	// define swarm members

	int zoom = -3;
	
	boids.push_back(Boid(MyVector(-1, -1, -6-zoom), MyVector(1, 0, 0)));
	boids.push_back(Boid(MyVector(-0.5, -1, -6 - zoom), MyVector(1, 1, 0)));
	boids.push_back(Boid(MyVector(0, -1.5, -6.5 - zoom), MyVector(0, 1, 0)));
	boids.push_back(Boid(MyVector(-1.5, 0, -8 - zoom), MyVector(1, 0, 0)));
	boids.push_back(Boid(MyVector(-1, 1, -8 - zoom), MyVector(1, 0, 0)));
	boids.push_back(Boid(MyVector(-1, 0, -7 - zoom), MyVector(1, 1, 0)));
	boids.push_back(Boid(MyVector(-1, 0, -8 - zoom), MyVector(1, 0, 1)));
	boids.push_back(Boid(MyVector(0, -1, -6 - zoom), MyVector(1, 0, 0)));
	boids.push_back(Boid(MyVector(-0.5, -1, -6 - zoom), MyVector(1, 1, 0)));
	boids.push_back(Boid(MyVector(0, -1.5, -6.5 - zoom), MyVector(0, 1, 0)));
	boids.push_back(Boid(MyVector(-1.5, 0, -8 - zoom), MyVector(1, 0, 0)));
	boids.push_back(Boid(MyVector(-1, 1, -8 - zoom), MyVector(1, 0, 0)));
	boids.push_back(Boid(MyVector(-1, 0, -7 - zoom), MyVector(1, 1, 0)));
	boids.push_back(Boid(MyVector(-1, 0, -8 - zoom), MyVector(1, 0, 1)));
	boids.push_back(Boid(MyVector(0, -1, -6 - zoom), MyVector(1, 0, 0)));
	boids.push_back(Boid(MyVector(-0.5, -1, -6 - zoom), MyVector(1, 1, 0)));
	boids.push_back(Boid(MyVector(0, -0.5, -6.5 - zoom), MyVector(0, 1, 0)));
	boids.push_back(Boid(MyVector(-1.5, 0, -8 - zoom), MyVector(1, 0, 0)));
	boids.push_back(Boid(MyVector(-1, 1, -8 - zoom), MyVector(1, 0, 0)));
	boids.push_back(Boid(MyVector(-1, 0, -7 - zoom), MyVector(1, 1, 0)));
	boids.push_back(Boid(MyVector(-1, 0, -8 - zoom), MyVector(1, 0, 1)));

	//create swarm
	swarm = Swarm(&boids);

	startup();									// Setup all necessary information for startup (aka. load texture, shaders, models, etc).

												// Mixed graphics and update functions - declared in main for simplicity.
	glfwSetWindowSizeCallback(myGraphics.window, onResizeCallback);			// Set callback for resize
	glfwSetKeyCallback(myGraphics.window, onKeyCallback);					// Set Callback for keys

																			// MAIN LOOP run until the window is closed
	do {
		double currentTime = glfwGetTime();		// retrieve timelapse
		glfwPollEvents();						// poll callbacks
		update(currentTime);					// update (physics, animation, structures, etc)
		render(currentTime);					// call render function.

		glfwSwapBuffers(myGraphics.window);		// swap buffers (avoid flickering and tearing)

		leftRight = (float)glfwGetKey(myGraphics.window, GLFW_KEY_LEFT);

		running &= (glfwGetKey(myGraphics.window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);	// exit if escape key pressed
		running &= (glfwWindowShouldClose(myGraphics.window) != GL_TRUE);
	} while (running);

	myGraphics.endProgram();			// Close and clean everything up...

	cout << "\nPress any key to continue...\n";
	cin.ignore(); cin.get(); // delay closing console to read debugging errors.

	return 0;
}

void startup() {
 
	// Calculate proj_matrix for the first time.
	myGraphics.aspect = (float)myGraphics.windowWidth / (float)myGraphics.windowHeight;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);

	// Load Geometry

	for (Boid &b : *swarm.boids){
		Cube visualBoid;
		visualBoid.Load();
		visualBoids.push_back(visualBoid);


	}
	
	myCube.Load();

	myGraphics.SetOptimisations();		// Cull and depth testing
}

void update(double currentTime) {
	float deltaSeconds = currentTime - prevUpdateTime;
	swarm.UpdateSwarm(t);

	for (Boid &b : *swarm.boids)
	{
		b.Position = b.Position + b.Velocity * t;
	}

	for (int i = 0; i < visualBoids.size(); i++) {
		glm::mat4 mv_matrix_cube =
			glm::translate(glm::vec3(swarm.boids->at(i).Position.x, swarm.boids->at(i).Position.y, swarm.boids->at(i).Position.z)) *
			glm::scale(glm::vec3(0.1f, 0.1f, 0.1f)) *
			glm::mat4(1.0f);
		visualBoids.at(i).mv_matrix = mv_matrix_cube;
		visualBoids.at(i).proj_matrix = myGraphics.proj_matrix;
	}

	

	prevUpdateTime = currentTime;
	t = 0.01f; // increment movement variable

}

void render(double currentTime) {
	// Clear viewport - start a new frame.
	myGraphics.ClearViewport();

	// Draw
	for (Cube &c : visualBoids){
		c.Draw();
	}
	

}

void onResizeCallback(GLFWwindow* window, int w, int h) {	// call everytime the window is resized
	myGraphics.windowWidth = w;
	myGraphics.windowHeight = h;

	myGraphics.aspect = (float)w / (float)h;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) { // called everytime a key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	//if (key == GLFW_KEY_LEFT) angleY += 0.05f;
}