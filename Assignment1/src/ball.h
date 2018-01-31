#include "main.h"
#include "slab.h"

#ifndef BALL_H
#define BALL_H

class Ball {
public:
    Ball() {}
    Ball(float x, float y, float radius, int health, float score, color_t color, bool has_slab, int slab_angle);
    glm::vec3 position;
    float rotation, radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double x_speed, y_speed;
    bool active, has_slab;
    int health, score;
    Slab slab;
private:
    VAO *object;
};

#endif // BALL_H
