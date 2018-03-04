#include "main.h"

#ifndef POWER_H
#define POWER_H


class Power {
public:
    Power() {}
    Power(float x, float y, float z, color_t color, int type);
    glm::vec3 position;
    bool active;
    float rotation, length, width, height;
    int type, update_time;
    void draw(glm::mat4 VP);
    void tick();
    bounding_box_t getBoundingBox();
private:
    VAO *object;
};

#endif // POWER_H
