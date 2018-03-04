#include "boat.h"
#include "main.h"
#include "helper.h"

Boat::Boat(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->update_time = 0;
    this->update_time = 0;
    this->speed = 0.15;
    this->length = 2.5;
    this->width = 1;
    this->health = 100;

    int pos_base = 0, pos_sides = 0;
    GLfloat vertex_buffer_data_base[18], vertex_buffer_data_sides[100];

    vector<float> t; t.clear();

    float length = 2.5f, width = 1.0f, height = 1.0f;
    Point A(-width / 2.0, 0, -length / 2.0), B(-width / 2.0, 0, length / 2.0), C(width / 2.0, 0, length / 2.0), D(width / 2.0, 0, -length / 2.0);
    Point P((-width / 2.0) - 0.1, height, -length / 2.0), Q((-width / 2.0) - 0.1, height, length / 2.0), R(width / 2.0 + 0.1, height, length / 2.0), S(width / 2.0 + 0.1, height, -length / 2.0);

    get_quad(A, B, C, D, t);
    for(int i=0; i<t.size(); ++i) vertex_buffer_data_base[pos_base++] = t[i];
    t.clear();

    get_quad(A, B, Q, P, t);
    get_quad(D, C, R, S, t);
    get_quad(P, S, D, A, t);
    get_quad(B, C, R, Q, t);
    for(int i=0; i<t.size(); ++i) vertex_buffer_data_sides[pos_sides++] = t[i]; 

    this->object = create3DObject(GL_TRIANGLES, pos_base / 3, vertex_buffer_data_base, COLOR_YELLOW, GL_FILL);
    this->object_1 = create3DObject(GL_TRIANGLES, pos_sides / 3, vertex_buffer_data_sides, COLOR_RED, GL_FILL);
}

void Boat::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object_1);
}

void Boat::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Boat::tick() {
	++this->update_time;
	if(this->update_time > 0 and this->update_time < 20) this->position.y += 0.005;
	else if(this->update_time >= 20 and this->update_time <= 40) this->position.y -= 0.005;
	else this->update_time = 0;
    ++this->speed_time;
    if(this->speed_time > 6000) this->speed = 0.15, this->speed_time = 0;
}

bounding_box_t Boat::getBoundingBox(){
    bounding_box_t BB;
    BB.x = this->position.x;
    BB.y = this->position.y;
    BB.z = this->position.z;
    BB.radius = this->length / 2.0;
    return BB;
}