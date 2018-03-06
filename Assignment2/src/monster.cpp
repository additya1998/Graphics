#include "monster.h"
#include "main.h"
#include "helper.h"

Monster::Monster(float x, float y, float z, bool isBoss) {

    this->position = glm::vec3(x, y, z);
    this->active = true;
    this->boss = isBoss;

    int pos_body = 0, pos_face = 0;
    GLfloat vertex_buffer_data_body[108 * 3], vertex_buffer_data_face[100];
    vector<float> body, face;

    float monster_height = 2.0f, monster_width = 4.0f, monster_depth = 3.0f;
    float leg_height = 3.0f, leg_width = 1.0f, leg_depth = 1.0f;

    if(isBoss);
    else monster_height /= 2, monster_width /= 2, monster_depth /= 2, leg_height /= 2, leg_width /= 2, leg_depth /= 2;

    Point leg_1(0 - (monster_width / 2.0) + (leg_width / 2.0), leg_height / 2.0, 0);
    get_box(leg_1, leg_height, leg_width, leg_depth, body);
    
    Point leg_2(0 + (monster_width / 2.0) - (leg_width / 2.0), leg_height / 2.0, 0);
    get_box(leg_2, leg_height, leg_width, leg_depth, body);

    Point torso_center(0, leg_height, 0);
    get_box(torso_center, monster_height, monster_width, monster_depth, body);

    for(int i=0; i<body.size(); ++i) vertex_buffer_data_body[pos_body++] = body[i];
    
    // float eye = 0.2f;
    // Point eye_A()
    // Point eye_B()
    // Point eye_C()
    // Point eye_D()
    // get_quad(A, B, C, D, face);
    // for(int i=0; i<face.size(); ++i) vertex_buffer_data_face[pos_face++] = face[i];

    this->height = monster_height + leg_height;
    this->width = monster_width + leg_width;    

    if(isBoss) this->body = create3DObject(GL_TRIANGLES, pos_body / 3, vertex_buffer_data_body, COLOR_RED, GL_FILL);
    else this->body = create3DObject(GL_TRIANGLES, pos_body / 3, vertex_buffer_data_body, COLOR_PURPLE, GL_FILL);
    
    this->face = create3DObject(GL_TRIANGLES, pos_face / 3, vertex_buffer_data_face, COLOR_WHITE, GL_FILL);
}

void Monster::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->body);
    draw3DObject(this->face);
}

void Monster::set_position(float x, float y) {
    // this->position = glm::vec3(x, y, 0);
}

void Monster::tick() {
    // int x = rand() & 1, z = rand() & 1;
    // if(x) this->position.x += ((rand() % 10) * 1.0f / 50);
    // else this->position.x -= ((rand() % 10) * 1.0f / 50);
    // if(z) this->position.z += ((rand() % 10) * 1.0f / 50);
    // else this->position.z -= ((rand() % 10) * 1.0f / 50);
}

bounding_box_t Monster::getBoundingBox(){
    bounding_box_t BB;
    BB.x = this->position.x;
    BB.y = this->position.y;
    BB.z = this->position.z;
    BB.radius = max(this->width, this->height);
    return BB;
}