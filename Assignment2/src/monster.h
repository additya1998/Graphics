#include "main.h"

#ifndef MONSTER_H
#define MONSTER_H


class Monster {
public:
    Monster() {}
    Monster(float x, float y, float z, bool active);
    glm::vec3 position;
    bool active, boss;
    float rotation, angle, update_time, length, width, height;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t getBoundingBox();    
private:
    VAO *body;
    VAO *face;
};

#endif // MONSTER_H
