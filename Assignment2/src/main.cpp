#include "main.h"
#include "timer.h"
#include "camera.h"
#include "boat.h"
#include "water.h"
#include "ball.h"
#include "rock.h"
#include "power.h"
#include "helper.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

const int ROCKS = 20;
const int POWERUPS = 20;

Water water;
Boat boat, boat_test;
Camera camera[5];
Rock rocks[ROCKS];
Power powers[POWERUPS];

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float window_width = 1024, window_height = 1024;
bool left_pressed = true;
int cur_camera = 0;

// Functions
void set_camera(int idx);


Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
	// clear the color and depth in the frame buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the loaded shader program
	// Don't change unless you know what you are doing
	glUseProgram (programID);

	// Eye - Location of camera. Don't change unless you are sure!!
	// glm::vec3 eye (5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f));
	// Target - Where is the camera looking at.  Don't change unless you are sure!!
	// glm::vec3 target (0, 0, 0);
	// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
	// glm::vec3 up (0, 1, 0);

	// Compute Camera matrix (view)
	Matrices.view = glm::lookAt(camera[cur_camera].eye, camera[cur_camera].target, camera[cur_camera].up); // Rotating Camera for 3D
	// Don't change unless you are sure!!
	// Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

	// Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
	// Don't change unless you are sure!!
	glm::mat4 VP = Matrices.projection * Matrices.view;

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	// Don't change unless you are sure!!
	glm::mat4 MVP;  // MVP = Projection * View * Model

	// Scene render
	water.draw(VP);
	boat.draw(VP);
	for(int i=0; i<ROCKS; ++i) if(rocks[i].active) rocks[i].draw(VP);
	for(int i=0; i<POWERUPS; ++i) if(powers[i].active) powers[i].draw(VP);
	// boat_test.draw(VP);
	// ball2.draw(VP);
}

void move_up(){
	boat.position.z = boat.position.z -  boat.speed * cos(boat.rotation * M_PI / 180);
	boat.position.x = boat.position.x - boat.speed * sin(boat.rotation * M_PI / 180);	
}

void move_down(){
	boat.position.z = boat.position.z + boat.speed * cos(boat.rotation * M_PI / 180);
	boat.position.x = boat.position.x + boat.speed * sin(boat.rotation * M_PI / 180);
}

void drag_view(float val){
	if(val > 0) ++boat.rotation;
	else --boat.rotation;
	if(boat.rotation > 360) boat.rotation -= 360;
	if(boat.rotation < -360) boat.rotation += 360;
}

void tick_input(GLFWwindow *window) {
	int up  = glfwGetKey(window, GLFW_KEY_UP);
	int down  = glfwGetKey(window, GLFW_KEY_DOWN);
	int left  = glfwGetKey(window, GLFW_KEY_LEFT);
	int right = glfwGetKey(window, GLFW_KEY_RIGHT);
	int view = glfwGetKey(window, GLFW_KEY_V);
	if(up) move_up();
	if(down) move_down();
	if(left) drag_view(1);
	if(right) drag_view(-1);
}

void tick_elements() {
	boat.tick();
	for(int i=0; i<POWERUPS; ++i){
		if(powers[i].active) powers[i].tick();		
	}
	for(int i=0; i<ROCKS; ++i){
		if(rocks[i].active and detect_collision(boat.getBoundingBox(), rocks[i].getBoundingBox())){
			boat.health = max(0, boat.health - 10);
			rocks[i].active = false;
		}
	}
	for(int i=0; i<POWERUPS; ++i){
		if(powers[i].active and detect_collision(boat.getBoundingBox(), powers[i].getBoundingBox())){
			// 0: Health, 1: Booster, 2: Points
			if(powers[i].type == 0) boat.health = boat.health + 10;
			else if(powers[i].type == 1){
				boat.speed_time = 0;
				boat.speed = boat.speed + 0.05;
			}
			else boat.score = boat.score + 10;
			powers[i].active = false;
		} 		
	}
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
	/* Objects should be created before any other gl function and shaders */
	// Create the models

	boat = Boat(0, 0, 0, COLOR_RED);
	++boat.rotation;
	water = Water(0, -3, 0, COLOR_LBLUE);

	for(int i=0; i<ROCKS; ++i) rocks[i] = Rock(rand() % 50 + 1, 0, rand() % 50 + 1, COLOR_BLACK);
	for(int i=0; i<POWERUPS; ++i){
		// 0: Health, 1: Booster, 2: Points
		if(i % 3 == 0) powers[i] = Power(rand() % 50 + 1, 0, rand() % 50 + 1, COLOR_RED, 0);
		else if(i % 3 == 1) powers[i] = Power(rand() % 50 + 1, 0, rand() % 50 + 1, COLOR_GREEN, 1);
		else powers[i] = Power(rand() % 50 + 1, 0, rand() % 50 + 1, COLOR_BROWN, 2);
	}

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

	// Background color of the scene
	glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
	cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
	cout << "VERSION: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
	srand(time(0));
	int width  = 600;
	int height = 600;

	window = initGLFW(width, height);

	initGL (window, width, height);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	set_camera(cur_camera);

	/* Draw in loop */
	while (!glfwWindowShouldClose(window)) {
		// Process timers

		if (t60.processTick()) {
			// Update camera
			set_camera(cur_camera);

			// 60 fps
			// OpenGL Draw commands
			draw();
			// Swap Frame Buffer in double buffering
			glfwSwapBuffers(window);

			tick_elements();
			tick_input(window);
			if(boat.health < 0) quit(window);
		}

		// Poll for Keyboard and mouse events
		glfwPollEvents();
	}

	quit(window);
}

// float distance(Point a, Point b){
// 	return sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z)));
// }

bool detect_collision(bounding_box_t a, bounding_box_t b) {
	float d = sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z)));
	// printf("%f %f %f\n", d, a.radius, b.radius);
	// if(d <= a.radius + b.radius) printf("True\n");;
	if(d <= a.radius + b.radius) return true;
	return false;
}

void reset_screen() {

	float near = 0.0f;
	float far = 1000.0;
	GLfloat fov = M_PI / 1.2;
	Matrices.projection = glm::perspective(fov, (GLfloat) window_width / (GLfloat) window_height, near, far);

	// float top    = screen_center_y + 4 / screen_zoom;
	// float bottom = screen_center_y - 4 / screen_zoom;
	// float left   = screen_center_x - 4 / screen_zoom;
	// float right  = screen_center_x + 4 / screen_zoom;
	// Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

void set_camera(int idx){
	
	// follow boat
	if(idx == 0){
		camera[0].eye = glm::vec3 (boat.position.x + 2 * sin(boat.rotation * M_PI / 180.0f), 2.5, boat.position.z + 2 * cos(boat.rotation * M_PI / 180.0f));
		camera[0].target = glm::vec3 (boat.position.x, boat.position.y, boat.position.z);
		camera[0].up = glm::vec3 (0, 1, 0);
	}

	// top view
	else if(idx == 1){
		camera[1].eye = glm::vec3(boat.position.x, 5, boat.position.z);
		camera[1].target = glm::vec3(boat.position.x, boat.position.y, boat.position.z + 0.05);
		camera[1].up = glm::vec3(0, 1, 0);
	}

	// boat view
	else if(idx == 2){
		// camera[2].eye = glm::vec3 (boat.position.x, 1, boat.position.z);
		// camera[2].target = glm::vec3 (boat.position.x - 2 * sin(boat.rotation * M_PI / 180.0f), 1, boat.position.z - 2 * cos(boat.rotation * M_PI / 180.0f));
		// camera[2].up = glm::vec3 (0, 1, 0);

		// camera[2].eye = glm::vec3(boat.position.x, 1, boat.position.z + 1);
		// camera[2].target = glm::vec3(boat.position.x, 1.0f, boat.position.z + 2);
		// camera[2].up = glm::vec3(0, 1, 0);	
	}

}

void change_view(){
	cur_camera = (cur_camera + 1) % 2;
	printf("switch %d\n", cur_camera);
}