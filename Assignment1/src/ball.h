#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, float radius, color_t color);
    glm::vec3 position;
    float rotation, radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double x_speed, y_speed;
    bool active;
private:
    VAO *object;
};

#endif // BALL_H
