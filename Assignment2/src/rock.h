#include "main.h"

#ifndef ROCK_H
#define ROCK_H


class Rock {
public:
    Rock() {}
    Rock(float x, float y, float z, color_t color);
    glm::vec3 position;
    bool active;
    float rotation, angle, update_time, length, width, height;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t getBoundingBox();    
private:
    VAO *object;
};

#endif // ROCK_H
