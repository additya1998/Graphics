#include "main.h"

#ifndef MAN_H
#define MAN_H


class Man {
public:
    Man() {}
    Man(float x, float y, float z);
    glm::vec3 position;
    int angle, update_time;
    float rotation, length, width, height, y_speed;
    bool on_boat;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t getBoundingBox();
    double speed;
private:
    VAO *face;
    VAO *torso;
    VAO *left_arm;
    VAO *right_arm;
    VAO *left_leg;
    VAO *right_leg;
};

#endif // MAN_H
