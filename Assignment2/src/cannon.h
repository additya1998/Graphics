#include "main.h"

#ifndef CANNON_H
#define CANNON_H


class Cannon {
public:
    Cannon() {}
    Cannon(float x, float y, float z, float angle, bool fire_ball, color_t color);
    glm::vec3 position;
    bool active, fire_ball;
    float rotation, angle, update_time, radius, x_speed, y_speed;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t getBoundingBox();    
private:
    VAO *object;
};

#endif // CANNON_H
