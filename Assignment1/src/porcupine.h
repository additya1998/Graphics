#include "main.h"

#ifndef PORCUPINE_H
#define PORCUPINE_H

class Porcupine {
public:
    Porcupine() {}
    Porcupine(float x, float y, float width, float height, color_t color);
    glm::vec3 position;
    float rotation, width, height;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double x_speed;
    bool active;
private:
    VAO *object;
};

#endif // PORCUPINE_H
