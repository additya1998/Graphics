#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"

bool   cannon_keyboard_input = true;
bool   drag_pan = false, old_cki;
double drag_oldx = -1, drag_oldy = -1;
double cursor_xpos, cursor_ypos;

using namespace std;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
	// Function is called first on GLFW_PRESS.

	if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_V:
			change_view();
			break;
		case GLFW_KEY_F:
			fire_cannon(COLOR_BLACK);
			break;
		case GLFW_KEY_G:
			get_off();
			break;
		case GLFW_KEY_SPACE:
			boat_jump();
			break;
		default:
			break;
		}
	} else if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			quit(window);
			break;
		default:
			break;
		}
	}
}

/* Executed for character input (like in text boxes) */
void keyboardChar(GLFWwindow *window, unsigned int key) {
	switch (key) {
	case 'Q':
	case 'q':
		quit(window);
		break;
	default:
		break;
	}
}

/* Executed when a mouse button is pressed/released */
void mouseButton(GLFWwindow *window, int button, int action, int mods) {
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_PRESS) {
			left_pressed = true;
			glfwGetCursorPos(window, &cursor_xpos, &cursor_ypos);
		} else if (action == GLFW_RELEASE) left_pressed = false;
		break;
	// case GLFW_MOUSE_BUTTON_RIGHT:
	// if (action == GLFW_RELEASE) {
	// rectangle_rot_dir *= -1;
	// }
	// break;
	default:
		break;
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
	if(left_pressed){
		if(xpos > cursor_xpos) drag_view(0.05);
		else if(xpos < cursor_xpos) drag_view(-0.05);
		cursor_xpos = xpos;
	}
	else{
		if(abs(xpos - cursor_xpos) > 1){
			if(xpos > cursor_xpos) drag_cannon(1);
			else if(xpos < cursor_xpos) drag_cannon(-1);
			cursor_xpos = xpos;
		}
		else if(abs(ypos - cursor_ypos) > 1){
			if(ypos > cursor_ypos) drag_cannon(2);
			else if(ypos < cursor_ypos) drag_cannon(-2);
			cursor_ypos = ypos;
		}		
	}
 }

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
	handle_zoom(yoffset / 20);
}
