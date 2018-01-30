#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H


class Player {
public:
    Player() {}
    Player(float x, float y, float radius, color_t color);
    glm::vec3 position;
    float rotation, radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double x_speed, y_speed;
    double x_acc, y_acc;
    bool in_water;
private:
    VAO *object;
};



#endif // PLAYER_H
