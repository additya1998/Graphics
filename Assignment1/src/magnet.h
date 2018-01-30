#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H

class Magnet {
public:
    Magnet() { this->timer = 0; }
    Magnet(float x, float y, float radius, bool left, color_t color_main, color_t color_edge);
    glm::vec3 position;
    float rotation, radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bool active, left;
    int timer;
private:
    VAO *object_1;
    VAO *object_2;
};

#endif // MAGNET_H
