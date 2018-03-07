#include "main.h"

#ifndef ISLAND_H
#define ISLAND_H


class Island {
public:
    Island() {}
    Island(float x, float y, float z, float radius, color_t color);
    glm::vec3 position;
    float radius, rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t getBoundingBox();    
private:
    VAO *object;
    VAO *treasure;
};

#endif // ISLAND_H
