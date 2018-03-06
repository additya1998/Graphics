#include "main.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation, angle, length, width, height, y_speed;
    int score, update_time, speed_time, wind_time, health, wind_dir, cannon_dir;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t getBoundingBox();
    double speed;
private:
    VAO *object;
    VAO *object_1;
    VAO *mast_rod;
    VAO *flag;
    VAO *cannon;
};

#endif // BOAT_H
