#include <iostream>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "main.h"
#include "timer.h"
#include "camera.h"
#include "boat.h"
#include "water.h"
#include "ball.h"
#include "rock.h"
#include "power.h"
#include "monster.h"
#include "cannon.h"
#include "trail.h"
#include "man.h"
#include "island.h"
#include "helper.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

const int ROCKS = 15;
const int POWERUPS = 25;
const int MONSTERS = 5;
const int CANNONS = 100;
const int TRAIL = 10000;
const int BOSS_FIRE = 120;
int fire_time = 0, PID;

Water water;
Boat boat, boat_test;
Camera camera[5];
Rock rocks[ROCKS];
Power powers[POWERUPS];
Monster monsters[MONSTERS];
Monster boss;
Cannon cannons[CANNONS];
Trail trails[TRAIL];
Man man;
Island island;

float screen_zoom = 0, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float window_width = 1366, window_height = 768;
float y_offset = 0;
bool left_pressed = false;
int cur_camera = 0;

// Functions
void set_camera(int idx);
bool check_collissions();

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
	for(int i=0; i<TRAIL; ++i) if(trails[i].active) trails[i].draw(VP);
	island.draw(VP);
	boat.draw(VP);
	for(int i=0; i<ROCKS; ++i) if(rocks[i].active) rocks[i].draw(VP);
	for(int i=0; i<POWERUPS; ++i) if(powers[i].active) powers[i].draw(VP);
	for(int i=0; i<MONSTERS; ++i) if(monsters[i].active) monsters[i].draw(VP);
	for(int i=0; i<CANNONS; ++i) if(cannons[i].active) cannons[i].draw(VP);
	if(boss.active) boss.draw(VP);
	man.draw(VP);
	water.draw(VP);
}

void get_off(){
	if(man.on_boat){
		man.on_boat = false;
		man.position.z = man.position.z - 2 * man.speed * cos(man.rotation * M_PI / 180);
		man.position.x = man.position.x - 2 * man.speed * sin(man.rotation * M_PI / 180);
		man.position.y = island.position.y + 1.2;
	}
	else man.on_boat = true;
}

void move_up(){
	if(man.on_boat){
		if(check_collissions());
		else{			
			boat.position.z = boat.position.z - boat.speed * cos(boat.rotation * M_PI / 180);
			boat.position.x = boat.position.x - boat.speed * sin(boat.rotation * M_PI / 180);				
		}
	}
	else{
		if(man.angle == 0) man.angle = 15;
		else{
			++man.update_time;
			if(man.update_time == 10) man.angle = -man.angle, man.update_time = 0;
			man.tick();
		}
		man.position.z = man.position.z - man.speed * cos(man.rotation * M_PI / 180);
		man.position.x = man.position.x - man.speed * sin(man.rotation * M_PI / 180);						
	}
}

void move_down(){
	if(man.on_boat){
		if(check_collissions());
		else{
			boat.position.z = boat.position.z + boat.speed * cos(boat.rotation * M_PI / 180);
			boat.position.x = boat.position.x + boat.speed * sin(boat.rotation * M_PI / 180);				
		}
	}
	else{
		if(man.angle == 0) man.angle = -15;
		else{
			++man.update_time;
			if(man.update_time == 10) man.angle = -man.angle, man.update_time = 0;
			man.tick();
		}
		man.position.z = man.position.z + man.speed * cos(man.rotation * M_PI / 180);
		man.position.x = man.position.x + man.speed * sin(man.rotation * M_PI / 180);					
	}
}

void drag_view(float val){
	if(man.on_boat){
		if(val > 0) ++boat.rotation;
		else --boat.rotation;
		if(boat.rotation > 360) boat.rotation -= 360;
		if(boat.rotation < -360) boat.rotation += 360;		
	}
	else{
		if(val > 0) ++man.rotation;
		else --man.rotation;
		if(man.rotation > 360) man.rotation -= 360;
		if(man.rotation < -360) man.rotation += 360;		
	}
}

void boat_jump(){
	if(man.on_boat)	boat.y_speed = 7.5;
	else man.y_speed = 7.5;
}

void drag_cannon(int val){
	if(cur_camera != 4){
		if(man.on_boat){		
			if(val > 0) --boat.cannon_dir;
			else ++boat.cannon_dir;
			if(boat.cannon_dir > 360) boat.cannon_dir -= 360;
			if(boat.cannon_dir < -360) boat.cannon_dir += 360;
		}
	}
	else{
		float v_val = val;
		if(abs(val - 0) == 1) camera_rotation_angle += v_val;
		else y_offset += (v_val / 10);
		y_offset = min(y_offset, 3.0f);
		y_offset = max(y_offset, -3.0f);
	}
}

void fire_cannon(color_t color){
	if(!man.on_boat) return;
	for(int i=0; i<CANNONS; ++i){
		if(cannons[i].active) continue;
		cannons[i] = Cannon(boat.position.x - 2.5 * sin((boat.rotation + boat.cannon_dir) * M_PI / 180), boat.position.y + 1, boat.position.z - 2.5 * cos((boat.rotation + boat.cannon_dir) * M_PI / 180), boat.rotation + boat.cannon_dir, 0, COLOR_BLACK);
		cannons[i].active = true;
		break;
	}
}

void tick_input(GLFWwindow *window) {
	int up  = glfwGetKey(window, GLFW_KEY_UP);
	int down  = glfwGetKey(window, GLFW_KEY_DOWN);
	int left  = glfwGetKey(window, GLFW_KEY_LEFT);
	int right = glfwGetKey(window, GLFW_KEY_RIGHT);
	int view = glfwGetKey(window, GLFW_KEY_V);
	int cannon_left = glfwGetKey(window, GLFW_KEY_A);
	int cannon_right = glfwGetKey(window, GLFW_KEY_D);
	int space = glfwGetKey(window, GLFW_KEY_SPACE);
	if(up) move_up();
	if(down) move_down();
	if(left) drag_view(1);
	if(right) drag_view(-1);
	if(cannon_left) drag_cannon(-1);
	if(cannon_right) drag_cannon(1);
}

bool check_collissions(){
	bool hitting = false;
	for(int i=0; i<ROCKS; ++i){
		if(rocks[i].active and detect_collision(boat.getBoundingBox(), rocks[i].getBoundingBox())){
			if(boat.position.y > 1) continue;
			boat.health = max(0, boat.health - 10);
			rocks[i].active = false;
			hitting = true;
		}
	}
	for(int i=0; i<POWERUPS; ++i){
		if(powers[i].active and detect_collision(boat.getBoundingBox(), powers[i].getBoundingBox())){
			// 0: Health, 1: Booster, 2: Points
			if(boat.position.y > 0.75){
				if(powers[i].type == 0) boat.health = boat.health + 10;
				else if(powers[i].type == 1){
					boat.speed_time = 0;
					boat.speed = boat.speed + 0.05;
				}
				else boat.score = boat.score + 10;				
				powers[i].active = false;
				hitting = true;
			}
		} 		
	}
	for(int i=0; i<MONSTERS; ++i){
		if(monsters[i].active and detect_collision(boat.getBoundingBox(), monsters[i].getBoundingBox())){
			boat.health = max(boat.health - 50, 0);
			monsters[i].active = false;
			hitting = true;

		}
	}
	float distance = ((boat.position.x - island.position.x) * (boat.position.x - island.position.x)) + ((boat.position.z - island.position.z) * (boat.position.z - island.position.z));
	distance = sqrt(distance);
	if(distance < island.radius) hitting = true;

	return hitting;
}

void tick_elements() {

	// tick
	boat.tick();
	if(boat.position.y < 0.5){
		for(int i=0; i<TRAIL; ++i){
			if(trails[i].active) trails[i].tick();
			else{
				trails[i] = Trail(boat.position.x, -1, boat.position.z, COLOR_WHITE);
				break;
			}
		}
	}

	if(man.on_boat){
		man.position.x = boat.position.x;
		man.position.y = boat.position.y + 1.2;
		man.position.z = boat.position.z;
		man.rotation = boat.rotation;
		man.y_speed = boat.y_speed;
	}
	else man.tick();

	if(boss.active) boss.tick();
	for(int i=0; i<POWERUPS; ++i){
		if(powers[i].active) powers[i].tick();		
	}
	for(int i=0; i<MONSTERS; ++i){
		if(monsters[i].active){
			if(monsters[i].position.x < boat.position.x) monsters[i].position.x += 0.05;
			else monsters[i].position.x -= 0.05;
			if(monsters[i].position.z < boat.position.z) monsters[i].position.z += 0.05;
			else monsters[i].position.z -= 0.05;
		}		
	}
	for(int i=0; i<CANNONS; ++i){
		if(cannons[i].active){
			cannons[i].tick();
			if(cannons[i].fire_ball){
				if(cannons[i].position.x < boat.position.x) cannons[i].position.x += 0.15;
				else cannons[i].position.x -= 0.15;
				if(cannons[i].position.z < boat.position.z) cannons[i].position.z += 0.15;
				else cannons[i].position.z -= 0.15;
			}

		}
	}
	if(boss.active){
		if(boss.position.x < boat.position.x) boss.position.x += 0.05;
		else boss.position.x -= 0.05;
		if(boss.position.z < boat.position.z) boss.position.z += 0.05;
		else boss.position.z -= 0.05;

		if(fire_time == BOSS_FIRE){
			for(int i=0; i<CANNONS; ++i){
				if(cannons[i].active) continue;
				cannons[i] = Cannon(boss.position.x, boss.position.y + 2.8, boss.position.z, 0, 1, COLOR_RED);
				cannons[i].active = true;
				break;
			}
			fire_time = 0;
		}
		++fire_time;
	}	

	// boat collissions 
	check_collissions();
	
	for(int i=0; i<CANNONS; ++i){
		if(cannons[i].active and !cannons[i].fire_ball){
			for(int j=0; j<ROCKS; ++j){
				if(rocks[j].active and detect_collision(rocks[j].getBoundingBox(), cannons[i].getBoundingBox())){
					rocks[j].active = false;
					cannons[i].active = false;
				}
			}
			for(int j=0; j<MONSTERS; ++j){
				if(monsters[j].active and detect_collision(monsters[j].getBoundingBox(), cannons[i].getBoundingBox())){
					monsters[j].active = false;
					cannons[i].active = false;
				}
			}
			if(boss.active and detect_collision(cannons[i].getBoundingBox(), boss.getBoundingBox())){		
				cannons[i].active = false;
				boss.active = false;
			}
		}
		else if(cannons[i].active and cannons[i].fire_ball){
			if(detect_collision(boat.getBoundingBox(), cannons[i].getBoundingBox())){
				boat.health = 0;
			}
		}
	}
	if(boss.active and detect_collision(boat.getBoundingBox(), boss.getBoundingBox())){
		boat.health = max(boat.health - 50, 0);
		boss.active = false;
	}
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
	/* Objects should be created before any other gl function and shaders */
	// Create the models

	srand(time(NULL));

	man = Man(0, 0, 0);
	boat = Boat(0, -0.5, 0, COLOR_RED);
	++boat.rotation;
	water = Water(0, -3, 0, COLOR_BLUE);
	boss = Monster(rand() % 100 + 30, 0, -(rand() % 100 + 30), 1);
	island = Island(150, 0, 150, 45, COLOR_SAND);

	for(int i=0; i<ROCKS; ++i){
		int x_pos = rand() % 100 + 1, z_pos = rand() % 100 + 1;
		if(i & 1) x_pos = -x_pos;
		if((i >> 1) & 1) z_pos = -z_pos; 
		rocks[i] = Rock(x_pos, 0, z_pos, COLOR_BLACK);
	}
	for(int i=0; i<POWERUPS; ++i){
		// 0: Health, 1: Booster, 2: Points
		int x_pos = rand() % 100 + 1, z_pos = rand() % 100 + 1;
		if(i & 1) x_pos = -x_pos;
		if((i >> 1) & 1) z_pos = -z_pos; 
		if(i % 3 == 0) powers[i] = Power(x_pos, 0, z_pos, COLOR_RED, 0);
		else if(i % 3 == 1) powers[i] = Power(x_pos, 0, z_pos, COLOR_GREEN, 1);
		else powers[i] = Power(x_pos, 0, z_pos, COLOR_YELLOW, 2);
	}
	for(int i=0; i<MONSTERS; ++i){
		int x_pos = rand() % 100 + 3, z_pos = rand() % 100 + 3;
		if(i & 1) x_pos = -x_pos;
		if((i >> 1) & 1) z_pos = -z_pos; 
		monsters[i] = Monster(x_pos, 0, z_pos, 0);
	}
	for(int i=0; i<CANNONS; ++i){
		cannons[i] = Cannon(0, 0, 0, 0, 0, COLOR_BLACK);
		cannons[i].active = false;
	}

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
	Matrices.Shader = glGetUniformLocation(programID, "SHD");


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

	PID = fork();

	audio_init();
	if(PID > 0){
		srand(time(0));
		int width  = 600;
		int height = 600;

		window = initGLFW(width, height);

		initGL (window, width, height);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		set_camera(cur_camera);

		int ticks = 0;

		/* Draw in loop */
		while (true) {
			// Process timers

			if (t60.processTick()) {
				++ticks;
				if(ticks % 120 == 0){
					printf("Points: %d\n", boat.score);
					printf("Health: %d\n", boat.health);
				}
				// Update camera
				set_camera(cur_camera);

				// 60 fps
				// OpenGL Draw commands
				draw();
				// Swap Frame Buffer in double buffering
				glfwSwapBuffers(window);

				tick_elements();
				tick_input(window);
				if(boat.health <= 0) break;
			}

			// Poll for Keyboard and mouse events
			glfwPollEvents();
		}
		kill(PID, SIGKILL);		
		quit(window);
	}

	else{
		while(true) audio_play();		
	}

}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
	float d = sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z)));
	if(abs(d - (a.radius + b.radius)) < 0.5) return true;
	return false;
}

void handle_zoom(double offset){
	screen_zoom += 3*offset;
	reset_screen();
}

void reset_screen() {

	float near = 0.5f;
	float far = 800.0;
	GLfloat fov = M_PI / 1.7;
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
		camera[0].eye = glm::vec3(man.position.x + (5 - screen_zoom) * sin(man.rotation * M_PI / 180.0f), man.position.y + 3, man.position.z + (5 - screen_zoom) * cos(man.rotation * M_PI / 180.0f));
		camera[0].target = glm::vec3(man.position.x, man.position.y, man.position.z);
		camera[0].up = glm::vec3(0, 1, 0);
	}

	// top view
	else if(idx == 1){
		camera[1].eye = glm::vec3(man.position.x, 10 - screen_zoom, man.position.z);
		camera[1].target = glm::vec3(man.position.x, man.position.y, man.position.z + 0.05);
		camera[1].up = glm::vec3(0, -1, 0);
	}

	// boat view
	else if(idx == 2){
		float d = 3.2;
		camera[2].eye = glm::vec3(man.position.x - d * sin(man.rotation * M_PI / 180.0f), man.position.y + 1, man.position.z - d * cos(man.rotation * M_PI / 180.0f));
		camera[2].target = glm::vec3(man.position.x - 2 * d * sin(man.rotation * M_PI / 180.0f), man.position.y, man.position.z - 2 * d * cos(man.rotation * M_PI / 180.0f));
		camera[2].up = glm::vec3(0, 1, 0);
	}

	// tower view
	else if(idx == 3){
		camera[3].eye = glm::vec3(-5, 10, -5);
		camera[3].target = glm::vec3(man.position.x , man.position.y, man.position.z);
		// camera[3].target = glm::vec3(0, 0, 0);
		camera[3].up = glm::vec3(0, 1, 0);
	}

	else{
		camera[4].eye = glm::vec3(man.position.x + (5 - screen_zoom) * sin(camera_rotation_angle * M_PI / 180.0f), man.position.y + 3 + y_offset, man.position.z + (5 - screen_zoom) * cos(camera_rotation_angle * M_PI / 180.0f));
		camera[4].target = glm::vec3(man.position.x, man.position.y, man.position.z + 0.05);
		camera[4].up = glm::vec3(0, 1, 0);		
	}
}

void change_view(){
	cur_camera = (cur_camera + 1) % 5;
	// printf("%d\n", cur_camera);
}