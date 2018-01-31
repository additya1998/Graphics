#include <iostream>
#include "main.h"
#include <string>
#include "timer.h"
#include "ground.h"
#include "ball.h"
#include "pool.h"
#include "player.h"
#include "trampoline.h"
#include "porcupine.h"
#include "magnet.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

const int scores[] = {10, 25, 50, 100};
const color_t colors[] = {COLOR_LBLUE, COLOR_GREEN, COLOR_YELLOW, COLOR_PURPLE};

const int total_balls = 500, total_porcupines = 2, magnet_time = 250;
const int ball_time_gap = 25, porcupine_time_gap = 500, frequency_of_slab = 5, eps = 0.01;
float screen_zoom = 0.5, screen_center_x = 0, screen_center_y = 0;
float screen_top, screen_bottom, screen_left, screen_right;
float screen_xoffset = 0.0, screen_yoffset = 0.0;

Pool pool;
Player player;
Ground ground, ground_useless;
Ball balls[total_balls];
Trampoline trampoline;
Porcupine porcupines[total_porcupines];
Magnet magnet, magnet_useless;
int time_since_last_ball = 0, SCORE = 0, time_since_last_porcupine = 0;

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
	// glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
	// Target - Where is the camera looking at.  Don't change unless you are sure!!
	// glm::vec3 target (0, 0, 0);
	// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
	// glm::vec3 up (0, 1, 0);

	// Compute Camera matrix (view)
	// Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
	// Don't change unless you are sure!!
	Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

	// Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
	// Don't change unless you are sure!!
	glm::mat4 VP = Matrices.projection * Matrices.view;

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	// Don't change unless you are sure!!
	glm::mat4 MVP;  // MVP = Projection * View * Model

	// Scene render
	ground.draw(VP);
	ground_useless.draw(VP);
	pool.draw(VP);
	if(magnet.active) magnet.draw(VP);
	if(magnet_useless.active) magnet_useless.draw(VP);
	trampoline.draw(VP);
	player.draw(VP);
	for(int i=0; i<total_porcupines; ++i){
		if(porcupines[i].active) porcupines[i].draw(VP);
	}
	for(int i=0; i<total_balls; ++i){
		if(balls[i].active) balls[i].draw(VP);
	}
}

void check_screen(){
	if(player.position.x + player.radius > screen_right){
		screen_center_x += (player.position.x + player.radius - screen_right);
		if(magnet.active){
			magnet.position.x += (player.position.x + player.radius - screen_right);
			magnet_useless.position.x += (player.position.x + player.radius - screen_right);
		}
		reset_screen();
	}
	if(player.position.x - player.radius < screen_left){
		screen_center_x += (player.position.x - player.radius - screen_left);
		if(magnet.active){
			magnet.position.x += (player.position.x - player.radius - screen_left);
			magnet_useless.position.x += (player.position.x - player.radius - screen_left);
		}
		reset_screen();
	}	
}

void tick_input(GLFWwindow *window) {
	int left  = glfwGetKey(window, GLFW_KEY_LEFT);
	int right = glfwGetKey(window, GLFW_KEY_RIGHT);
	int up = glfwGetKey(window, GLFW_KEY_UP);
	int down = glfwGetKey(window, GLFW_KEY_DOWN);
	int A = glfwGetKey(window, GLFW_KEY_A);
	int S = glfwGetKey(window, GLFW_KEY_S);
	int D = glfwGetKey(window, GLFW_KEY_D);
	int W = glfwGetKey(window, GLFW_KEY_W);
	int space = glfwGetKey(window, GLFW_KEY_SPACE);

	if(W) handle_zoom(1);
	else if(A){
		if(player.in_water) player.position.x -= 0.015;
		else player.position.x -= 0.05;
		player.rotation += 10;
		if(player.rotation > 360) player.rotation -= 360;
	}
	else if(S) handle_zoom(-1);
	else if(D){
		if(player.in_water) player.position.x += 0.015;
		else player.position.x += 0.05;
		player.rotation -= 10;
		if(player.rotation < -360) player.rotation += 360;
	}
	else if(space){
		if(player.position.y - player.radius <= ground.position.y + (33 / 2.0)) player.y_speed = 10;
	}
	else if(up) screen_center_y -= 0.01;
	else if(down) screen_center_y += 0.01;
	else if(left) screen_center_x -= 0.01;
	else if(right) screen_center_x += 0.01;

	check_screen();
	reset_screen();
	// player.position.x = max(player.position.x, screen_left + player.radius);
	// player.position.x = min(player.position.x, screen_right - player.radius);
}

float distance(float a, float b, float c, float d){
	float ans = ((a - c) * (a - c)) + ((b - d) * (b - d));
	return sqrt(ans);
}

bool collision(Player player, Ball ball){
	return (distance(ball.position.x, ball.position.y, player.position.x, player.position.y) <= player.radius + ball.radius);
}

void tick_elements() {

	for(int i=0; i<total_balls; ++i){
		balls[i].tick();
		if(balls[i].position.x + balls[i].radius < screen_left) balls[i].active = false;
	}
	player.tick();
	for(int i=0; i<total_porcupines; ++i){
		porcupines[i].tick();
		if(porcupines[i].position.x + 1.5 * porcupines[i].width >= screen_right) porcupines[i].x_speed = -porcupines[i].x_speed;
		else if(porcupines[i].position.x - 1.5 * porcupines[i].width <= screen_left) porcupines[i].x_speed = -porcupines[i].x_speed;
	}

	check_screen();

	// New ball spawn
	++time_since_last_ball;
	if(time_since_last_ball > ball_time_gap){
		time_since_last_ball = 0;
		for(int i=0; i<total_balls; ++i){
			if(balls[i].active) continue;
			int min_radius = 2500, max_radius = 5000; // divide by 10000
			float radius = ((rand() % (max_radius - min_radius)) + min_radius) / 10000.0;
			int min_y = ceil(radius + (ground.height / 2.0) + ground.position.y); int max_y = floor(screen_top - 3 - radius - radius);
			float pos_y = ((rand() % (100 * (max_y - min_y))) + min_y) / 100.0, pos_x = screen_right - radius;
			bool need_slab = false;
			if(rand() % frequency_of_slab == 0) need_slab = true;

			if(need_slab){
				int angle = rand() % 180;
				balls[i] = Ball(pos_x, pos_y, radius, 2, scores[3], colors[3], need_slab, angle);
			}
			else{
				int x = rand() % 3;
				balls[i] = Ball(pos_x, pos_y, radius, 1, scores[x], colors[x], need_slab, 0);
			} 

			balls[i].active = true;
			break;
		}
	}

	++time_since_last_porcupine;
	if(time_since_last_porcupine > porcupine_time_gap){
		time_since_last_porcupine = 0;
		for(int i=0; i<total_porcupines; ++i){
			if(porcupines[i].active) continue;
			Porcupine(screen_center_x + 1.0f, screen_bottom + ground.height, 0.25f, 0.5f, COLOR_RED);
			porcupines[i].active = true;
			break;
		}
	}

	++magnet.timer;
	if(magnet.timer == magnet_time){
		magnet.timer = 0;
		if(magnet.active) magnet.active = magnet_useless.active = false;
		else{
			if(rand() & 1){
				magnet = Magnet(screen_right - 1, 3, 1, 0, COLOR_RED);
				magnet_useless = Magnet(screen_right - 1, 3, 0.75, 0, COLOR_BACKGROUND);
			}
			else{
				magnet = Magnet(screen_left + 1, 3, 1, 1, COLOR_RED);
				magnet_useless = Magnet(screen_left + 1, 3, 0.75, 1, COLOR_BACKGROUND);
			}
			magnet.active = magnet_useless.active = true;
		}
	}

	if(magnet.active){
		if(magnet.left) player.position.x -= 0.025;
		else player.position.x += 0.025;
	}

	// Player and porcupine
	for(int i=0; i<total_porcupines; ++i){
		if(!porcupines[i].active) continue;
		if(player.position.y - player.radius <= porcupines[i].position.y + porcupines[i].height and player.position.y + player.radius >= porcupines[i].position.y){
			bool c = false;
			if(player.position.x + player.radius >= porcupines[i].position.x - 1.5 * porcupines[i].width and player.position.x + player.radius <= porcupines[i].position.x + 1.5 * porcupines[i].width) c = true;
			if(player.position.x - player.radius >= porcupines[i].position.x - 1.5 * porcupines[i].width and player.position.x - player.radius <= porcupines[i].position.x + 1.5 * porcupines[i].width) c = true;
			if(c){
				porcupines[i].active = false;
				SCORE = SCORE - 50;
				if(SCORE < 0) SCORE = 0;
			}
		}
	}

	// Player and enemy balls
	for(int i=0; i<total_balls; ++i){
		if(balls[i].active and balls[i].health == 1){
			if(collision(player, balls[i]) and player.y_speed < 0){
				balls[i].active = false;
				player.y_speed *= -1;
				player.y_speed += 0.5;
				SCORE = SCORE + balls[i].score;
			}
		}
	}

	for(int i=0; i<total_balls; ++i){
		if(balls[i].active and balls[i].health == 2 and balls[i].has_slab == false){
			if(collision(player, balls[i]) and player.y_speed < 0){
				--balls[i].health;
				player.y_speed *= -1;
				player.y_speed += 0.5;
			}
		}
	}

	for(int i=0; i<total_balls; ++i){
		if(balls[i].active and balls[i].has_slab and player.y_speed <= 0){
			if(collision(player, balls[i])){
				float d_theeta = (balls[i].slab.length * M_PI) / (2.0 * 180);
				float low_angle = balls[i].slab.rotation, high_angle = balls[i].slab.rotation;
				low_angle = (low_angle * M_PI) / 180.0;
				high_angle = (high_angle * M_PI) / 180.0;
				low_angle = low_angle - d_theeta;
				high_angle = high_angle + d_theeta;

				float low_x = balls[i].position.x + balls[i].radius * cos(low_angle);
				float high_x = balls[i].position.x + balls[i].radius * cos(high_angle);

				if(high_x < low_x) swap(high_x, low_x);

				bool y_low = false;
				if(balls[i].slab.rotation > 180) y_low = true; 

				float delta = min(fabs(player.position.x - high_x), fabs(player.position.x - low_x));

				if(delta < 1){
					if((y_low == true and player.position.y <= balls[i].position.y) or (y_low == false and player.position.y >= balls[i].position.y)){
						double x_s = player.x_speed, y_s = player.y_speed, slab_angle = balls[i].slab.rotation;
						player.y_speed = -1.0f * (x_s * sin((M_PI - slab_angle - slab_angle) / 180.0) + y_s * cos((slab_angle + slab_angle) / 180.0));
						player.x_speed = -1.0f * (x_s * cos((M_PI - slab_angle - slab_angle) / 180.0) + y_s * sin((slab_angle + slab_angle) / 180.0)); 
						if(slab_angle > 90) player.x_speed = -player.x_speed;
					}
				}				
			}
		}
	}

	// Player and trampoline
	float d = player.position.y - player.radius - trampoline.position.y;
	if(d > 0 and d < 0.25 and player.position.x + player.radius > trampoline.position.x - trampoline.radius - 0.15 and player.position.x - player.radius < trampoline.position.x + trampoline.radius + 0.15){
		if(player.y_speed < 0){
			player.y_speed = -1.5 * player.y_speed;
			double LIM = 12.5;
			if(player.y_speed > LIM) player.y_speed = LIM;
			if(player.y_speed < -LIM) player.y_speed = -LIM;
		}
	}

	// Player and water
	if(player.position.y - player.radius < pool.position.y and player.position.x >= pool.position.x - pool.radius and player.position.x <= pool.position.x + pool.radius){
		if(player.in_water);
		else{
			player.x_speed /= 2;
			player.y_speed /= 2;
			player.x_acc /= 2;
			player.y_acc /= 2;
			player.in_water = true;
		}
	}
	else{
		if(player.in_water){
			player.x_speed *= 2;
			player.y_speed *= 2;
			player.y_speed = min(player.y_speed, (double)10);
			player.x_acc *= 2;
			player.y_acc *= 2;
			player.in_water = false;
		}
		else;
	}

	if(player.in_water){
		if(player.position.x > pool.position.x) player.position.x -= 0.01;
		else if(player.position.x < pool.position.x) player.position.x += 0.01;
		float x_d = pool.position.x - player.position.x, y_d = pool.position.y - player.position.y;
		float d = sqrt(x_d * x_d + y_d * y_d);
		if(d + player.radius > pool.radius and player.position.y < pool.position.y){
			float angle = acos(x_d / d);
			// player.position.x = pool.position.x - (pool.radius - player.radius) * cos(angle);
			player.position.y = pool.position.y - (pool.radius - player.radius) * sin(angle);
			player.y_speed = -0.05 * player.y_speed;
		}
	}
	else{
		if(player.position.y - player.radius < ground.position.y + (ground.height / 2.0)){
			player.position.y = ground.position.y + (ground.height / 2) + player.radius;
			player.y_speed = -0.5 * player.y_speed;
			// if(fabs(player.y_speed) < eps) player.y_speed = 0;
		}
	}

}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
	/* Objects should be created before any other gl function and shaders */
	// Create the models

	/* ================================================================================================================================================= */

	float ground_height = 33, ground_width = 1000;
	ground = Ground(0, -20, ground_width, ground_height, COLOR_GREEN);
	ground_useless = Ground(0, -20.5, ground_width, 1, COLOR_BROWN);

	for(int i=0; i<total_balls; ++i){
		balls[i] = Ball();
		balls[i].active = false;
	}

	float radius = 0.25;
	player = Player(-5, ground.position.y + ground_height / 2.0 + radius, radius, COLOR_RED);

	pool = Pool(-1, ground.position.y + (ground.height / 2.0), 1.25, COLOR_BLUE);

	float tr_x = 3.0f, tr_y = -1.75f; 
	trampoline = Trampoline(tr_x, tr_y, 1.0f, COLOR_BLACK, tr_y - (ground.position.y + ground_height / 2.0));

	porcupines[0] = Porcupine(1.0f, ground.position.y + ground_height / 2.0, 0.25f, 0.5f, COLOR_RED);
	porcupines[1] = Porcupine(-4.0f, ground.position.y + ground_height / 2.0, 0.25f, 0.5f, COLOR_RED);

	magnet = Magnet();
	magnet_useless = Magnet();

	/* ================================================================================================================================================= */

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

int scores_needed[] = {100, 500, 1000};
int time_allowed[] = {60, 120, 180};

string get_string(int x){
	string t = "";
	while(x){
		t = (char)(x % 10 + '0') + t;
		x /= 10;
	}
	if(t == "") t = "0";
	return t;
}

int main(int argc, char **argv) {

	for(int cur_level=0; cur_level<3; ++cur_level){
		srand(time(0));
		int width  = 600;
		int height = 600;
		reset_screen();
		SCORE = 0;
		bool win = false;

		window = initGLFW(width, height);

		initGL (window, width, height);

		time_t start_time = time(NULL);

		int pre_score = SCORE, pre_time = time(NULL) - start_time;

		/* Draw in loop */
		while (!glfwWindowShouldClose(window)) {
			// Process timers

			time_t cur_time = time(NULL);

			if(SCORE >= scores_needed[cur_level]){
				win = true;
				break;
			}

			long long time_taken = cur_time - start_time; 
			if(time_taken > time_allowed[cur_level]){
				win = false;
				break;
			}

			if(time_taken > time_allowed[cur_level]) exit(0);

			if(SCORE != pre_score or time_taken != pre_time){
				pre_time = time_taken;
				pre_score = SCORE;
				string title = "Level: " + get_string(cur_level + 1) + ", Score: " + get_string(SCORE) + ", Time Left: " + get_string(time_allowed[cur_level] - time(NULL) + start_time); 
				glfwSetWindowTitle(window, title.c_str());
			}

			if (t60.processTick()) {
				// 60 fps
				// OpenGL Draw commands
				tick_elements();
				draw();
				// Swap Frame Buffer in double buffering
				glfwSwapBuffers(window);

				tick_input(window);
			}

			// Poll for Keyboard and mouse events
			glfwPollEvents();

		}

		if(win == false) quit(window);
	}

	quit(window);
}

void handle_zoom(double yoffset){
	if(yoffset > 0) screen_zoom += 0.1f;
	else screen_zoom -= 0.1f;
	if(screen_zoom < 0.15) screen_zoom = 0.15;
	if(screen_zoom > 0.75) screen_zoom = 0.75;
	reset_screen();
}

bool on_screen(float l, float r, float ql, float qr){
	if(ql > r or qr < l) return false;
	return true;
}

void shift_elements(){
	if(magnet.active){
		if(magnet.left) magnet.position.x = magnet_useless.position.x = screen_left + 3;
		else magnet.position.x = magnet_useless.position.x = screen_right - 3;
	}
}

void reset_screen() {
	screen_top		= screen_center_y + 4 / screen_zoom;
	screen_bottom 	= screen_center_y - 4 / screen_zoom; 
	screen_left 	= screen_center_x - 4 / screen_zoom;
	screen_right	= screen_center_x + 4 / screen_zoom; 

	// shift elements on screen
	shift_elements();

	Matrices.projection = glm::ortho(screen_left, screen_right, screen_bottom, screen_top, 0.1f, 500.0f);
}
