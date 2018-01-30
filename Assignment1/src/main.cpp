#include <iostream>
#include "main.h"
#include "timer.h"
#include "ground.h"
#include "ball.h"
#include "pool.h"
#include "player.h"
#include "trampoline.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

const int total_balls = 100, time_gap = 50, frequency_of_slab = 2, eps = 0.001;
float screen_zoom = 0.75, screen_center_x = 0, screen_center_y = 0;
float screen_top, screen_bottom, screen_left, screen_right;

Pool pool;
Player player;
Ground ground;
Ball balls[total_balls];
Trampoline trampoline;
int time_since_last = 0, SCORE = 0;

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
    for(int i=0; i<total_balls; ++i){
    	if(balls[i].active) balls[i].draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    if(left){
    	player.position.x -= 0.1;
    	player.rotation += 10;
    	if(player.rotation > 360) player.rotation -= 360;
    }
    if(right){
    	player.position.x += 0.1;
    	player.rotation -= 10;
    	if(player.rotation < -360) player.rotation += 360;
    }
    if(up){
    	player.y_speed = 10;
    	// if(abs(player.position.y - player.radius - ground.position.y - (ground.height / 2.0)) < 0.01) player.y_speed += 10;
    }
    player.position.x = max(player.position.x, screen_left + player.radius);
    player.position.x = min(player.position.x, screen_right - player.radius);
}

int distance(float a, float b, float c, float d){
	float ans = ((a - c) * (a - c)) + ((b - d) * (b - d));
	return sqrt(ans);
}

void tick_elements() {

	/*
		new balls appearing
		check collision of player with slab on ball
		check collision of player with ball
		check player and pool
		check player and water (going in, coming out)	
	*/

	// New ball spawn
	++time_since_last;
	if(time_since_last > time_gap){
		time_since_last = 0;
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

	// Player and enemy balls
	for(int i=0; i<total_balls; ++i){
		if(balls[i].active){
			if(distance(balls[i].position.x, balls[i].position.y, player.position.x, player.position.y) < player.radius + balls[i].radius){
				if(player.y_speed < 0){
					balls[i].active = false;
					player.y_speed *= -1;
					player.y_speed += 1;
					++SCORE;
				}
			}
		}
	}

	float d = player.position.y - player.radius - trampoline.position.y;
	if(d > 0 and d < 0.25 and player.position.x + player.radius > trampoline.position.x - trampoline.radius - 0.15 and player.position.x - player.radius < trampoline.position.x + trampoline.radius + 0.15){
		if(player.y_speed < 0){
			player.y_speed = -player.y_speed;
		}
	}

	// Player and water




	// Player and ground
	if(player.position.y - player.radius < ground.position.y + (ground.height / 2)){
		player.position.y = ground.position.y + (ground.height / 2) + player.radius;
		player.y_speed = -0.5 * player.y_speed;	
	}

	
	// if(player.position.x > pool.position.x - pool.radius and player.position.x < pool.position.x + pool.radius and abs(player.position.y - ){
	// 	float y_pos = (pool.radius * pool.radius) - ((pool.position.x - player.position.x) * (pool.position.x - player.position.x));
	// 	y_pos = sqrt(y_pos);
	// 	player.position.y = y_pos;
	// }

	for(int i=0; i<total_balls; ++i){
		balls[i].tick();
		if(balls[i].position.x + balls[i].radius < screen_left) balls[i].active = false;
	}
	player.tick();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

	/* ================================================================================================================================================= */

    // Initialise ground
	float ground_width = screen_right - screen_left, ground_height = (screen_top - screen_bottom) / 5;
	ground = Ground(screen_left + (ground_width / 2.0), screen_bottom + (ground_height / 2.0), ground_width, ground_height, COLOR_GREEN);

	// Initialise balls
	for(int i=0; i<total_balls; ++i){
		balls[i] = Ball();
		balls[i].active = false;
	}

	// Initialise player
	float radius = 0.25;
	player = Player(-4, screen_bottom + ground_height + radius, radius, COLOR_RED);
	printf("Center of player: -1 %f\n", screen_bottom + ground_height);


	pool = Pool(-1, ground.position.y + (ground.height / 2.0), 1.25, COLOR_BLUE);

	float tr_x = 3.0f, tr_y = -1.75f; 
	trampoline = Trampoline(tr_x, tr_y, 1.0f, COLOR_BLACK, tr_y - (screen_bottom + ground_height));

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



void reset_screen() {
	screen_top		= screen_center_y + 4 / screen_zoom;
	screen_bottom 	= screen_center_y - 4 / screen_zoom; 
	screen_left 	= screen_center_x - 4 / screen_zoom;
	screen_right	= screen_center_x + 4 / screen_zoom; 
    Matrices.projection = glm::ortho(screen_left, screen_right, screen_bottom, screen_top, 0.1f, 500.0f);
}
