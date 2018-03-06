#include "main.h"

#ifndef TRAIL_H
#define TRAIL_H


class Trail {
public:
    Trail() {}
    Trail(float x, float y, float z, color_t color);
    glm::vec3 position;
    bool active;
    int time;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t getBoundingBox();    
private:
    VAO *object;
};

#endif // TRAIL_H
