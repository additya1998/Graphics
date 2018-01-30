#include <iostream>
#include "main.h"
#include "timer.h"
#include "ground.h"
#include "ball.h"
#include "pool.h"
#include "player.h"
#include "trampoline.h"
#include "porcupine.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

const int total_balls = 100, total_porcupines = 2;
const int ball_time_gap = 50, porcupine_time_gap = 500, frequency_of_slab = 2, eps = 0.01;
float screen_zoom = 0.75, screen_center_x = 0, screen_center_y = 0;
float screen_top, screen_bottom, screen_left, screen_right;

Pool pool;
Player player;
Ground ground;
Ball balls[total_balls];
Trampoline trampoline;
Porcupine porcupines[total_porcupines];
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
    pool.draw(VP);
    trampoline.draw(VP);
    player.draw(VP);
    for(int i=0; i<total_porcupines; ++i){
    	if(porcupines[i].active) porcupines[i].draw(VP);
    }
    for(int i=0; i<total_balls; ++i){
    	if(balls[i].active) balls[i].draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    if(left){
    	if(player.in_water) player.position.x -= 0.015;
    	else player.position.x -= 0.05;
    	player.rotation += 10;
    	if(player.rotation > 360) player.rotation -= 360;
    }
    if(right){
    	if(player.in_water) player.position.x += 0.015;
    	else player.position.x += 0.05;
    	player.rotation -= 10;
    	if(player.rotation < -360) player.rotation += 360;
    }
    if(up){
    	player.y_speed = 5;
    	// if(abs(player.position.y - player.radius - ground.position.y - (ground.height / 2.0)) < 0.01) player.y_speed += 10;
    }
    player.position.x = max(player.position.x, screen_left + player.radius);
    player.position.x = min(player.position.x, screen_right - player.radius);
}

float distance(float a, float b, float c, float d){
	float ans = ((a - c) * (a - c)) + ((b - d) * (b - d));
	return sqrt(ans);
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

	// New ball spawn
	++time_since_last_ball;
	if(time_since_last_ball > ball_time_gap){
		time_since_last_ball = 0;
		for(int i=0; i<total_balls; ++i){
			if(balls[i].active) continue;
			int min_radius = 2500, max_radius = 5000; // divide by 10000
			float radius = ((rand() % (max_radius - min_radius)) + min_radius) / 10000.0;
			int min_y = ceil(radius + (ground.height / 2.0) + ground.position.y); int max_y = floor(screen_top - 3 - radius - radius);
			float pos_y = ((rand() % (100 * (max_y - min_y))) + min_y) / 100.0;
			bool need_slab = false;
			if(rand() % frequency_of_slab == 0) need_slab = true;
			balls[i] = Ball(screen_right - radius, pos_y, radius, COLOR_GREEN, need_slab, rand() % 360);
			balls[i].active = true;
			break;
		}
	}

	++time_since_last_porcupine;
	if(time_since_last_porcupine > porcupine_time_gap){
		time_since_last_porcupine = 0;
		for(int i=0; i<total_porcupines; ++i){
			if(porcupines[i].active) continue;
			Porcupine(1.0f, screen_bottom + ground.height, 0.25f, 0.5f, COLOR_RED);
			porcupines[i].active = true;
			break;
		}
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
				--SCORE;
			}
		}
	}

	// Player and enemy balls
	for(int i=0; i<total_balls; ++i){
		if(balls[i].active){
			bool hit_slab = false;
			// if(balls[i].has_slab){
			// 	if(distance(balls[i].position.x, balls[i].position.y, player.position.x, player.position.y) < player.radius + balls[i].radius + balls[i].slab.width){
			// 		float d = distance(balls[i].position.x, balls[i].position.y, player.position.x, player.position.y);
			// 		float x_d = player.position.x - balls[i].position.x, y_d = player.position.y - balls[i].position.y;
			// 		float angle = asin(y_d / d), slab_angle = (balls[i].slab.rotation * M_PI) / 180;
			// 		angle = angle * 180 / M_PI;
			// 		cout << fabs(angle - slab_angle) << endl;
			// 		if(fabs(angle - slab_angle) < 15){
			// 			double x_s = player.x_speed, y_s = player.y_speed;
			// 			cout << x_s << ' ' << y_s << endl;
			// 			player.y_speed = -1.0f * (x_s * sin(M_PI - slab_angle - slab_angle) + y_s * cos(slab_angle + slab_angle));
			// 			player.x_speed = -1.0f * (x_s * cos(M_PI - slab_angle - slab_angle) + y_s * sin(slab_angle + slab_angle)); 
			// 			cout << x_s << ' ' << y_s << endl;
			// 			hit_slab = true;
			// 		}
			// 	}
			// }

			if(!hit_slab and distance(balls[i].position.x, balls[i].position.y, player.position.x, player.position.y) < player.radius + balls[i].radius){
				if(player.y_speed < 0){
					balls[i].active = false;
					player.y_speed *= -1;
					player.y_speed += 1;
					++SCORE;
				}
			}
		}
	}

	// Player and trampoline
	float d = player.position.y - player.radius - trampoline.position.y;
	if(d > 0 and d < 0.25 and player.position.x + player.radius > trampoline.position.x - trampoline.radius - 0.15 and player.position.x - player.radius < trampoline.position.x + trampoline.radius + 0.15){
		if(player.y_speed < 0){
			player.y_speed = -1.5 * player.y_speed;
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
			player.x_acc *= 2;
			player.y_acc *= 2;
			player.in_water = false;
		}
		else;
	}

	if(player.in_water){
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
			if(fabs(player.y_speed) < eps) player.y_speed = 0;
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

	for(int i=0; i<total_balls; ++i){
		balls[i] = Ball();
		balls[i].active = false;
	}

	float radius = 0.25;
	player = Player(-4, ground.position.y + ground_height / 2.0 + radius, radius, COLOR_RED);

	pool = Pool(-1, ground.position.y + (ground.height / 2.0), 1.25, COLOR_BLUE);

	float tr_x = 3.0f, tr_y = -1.75f; 
	trampoline = Trampoline(tr_x, tr_y, 1.0f, COLOR_BLACK, tr_y - (ground.position.y + ground_height / 2.0));

	porcupines[0] = Porcupine(1.0f, ground.position.y + ground_height / 2.0, 0.25f, 0.5f, COLOR_RED);
	porcupines[1] = Porcupine(-4.0f, ground.position.y + ground_height / 2.0, 0.25f, 0.5f, COLOR_RED);

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


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;
    reset_screen();

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

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

    quit(window);
}

void handle_zoom(double yoffset){
	if(yoffset > 0) screen_zoom += 0.1f;
	else screen_zoom -= 0.1f;
	if(screen_zoom < 0.15) screen_zoom = 0.15;
	reset_screen();
	// cout << screen_zoom << endl;
}

void reset_screen() {
	screen_top		= screen_center_y + 4 / screen_zoom;
	screen_bottom 	= screen_center_y - 4 / screen_zoom; 
	screen_left 	= screen_center_x - 4 / screen_zoom;
	screen_right	= screen_center_x + 4 / screen_zoom; 
    Matrices.projection = glm::ortho(screen_left, screen_right, screen_bottom, screen_top, 0.1f, 500.0f);
}
