#include "main.h"

#ifndef CAMERA_H
#define CAMERA_H


class Camera {
public:
    Camera() {}

    // Eye - Location of camera. Don't change unless you are sure!!
	glm::vec3 eye;
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target;
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up;
private:
    VAO *object;
};

#endif // BALL_H
