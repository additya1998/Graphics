#include "power.h"
#include "main.h"
#include "helper.h"

Power::Power(float x, float y, float z, color_t color, int type) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->update_time = 0;
    this->length = 2.5;
    this->width = 0.25;
    this->active = true;
    this->type = type;

    int pos = 0, width = this->width;
    GLfloat vertex_buffer_data[90];

    Point A(-this->width, 4, -this->width), B(-this->width, 4, this->width), C(this->width, 4, this->width), D(this->width, 4, -this->width);
    Point E(0, 4 + 0.5, 0), F(0, 4 - 0.5, 0);

    vector<float> t; t.clear();
    get_quad(A, B, C, D, t);

    get_triangle(A, B, E, t);
    get_triangle(B, C, E, t);
    get_triangle(C, D, E, t);
    get_triangle(D, A, E, t);

    get_triangle(A, B, F, t);
    get_triangle(B, C, F, t);
    get_triangle(C, D, F, t);
    get_triangle(D, A, F, t);

    for(int i=0; i<t.size(); ++i) vertex_buffer_data[pos++] = t[i];
    t.clear();

    int pos_t = 0;
    GLfloat vertex_buffer_data_base[18];

    Point A_t(-1.25, 0.0, 0), B_t(0, 0.0, 1.25), C_t(1.25, 0.0, 0), D_t(0, 0.0, -1.25);
    get_quad(A_t, B_t, C_t, D_t, t);

    for(int i=0; i<t.size(); ++i) vertex_buffer_data_base[pos_t++] = t[i];
    t.clear();

    this->object = create3DObject(GL_TRIANGLES, pos / 3, vertex_buffer_data, color, GL_FILL);
    this->base = create3DObject(GL_TRIANGLES, pos_t / 3, vertex_buffer_data_base, COLOR_RED, GL_FILL);
}

void Power::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Shader, 0.5);
    draw3DObject(this->object);
    draw3DObject(this->base);
}

void Power::tick() {
    this->rotation = this->rotation + 5;
}

bounding_box_t Power::getBoundingBox(){
    bounding_box_t BB;
    BB.x = this->position.x;
    BB.y = this->position.y;
    BB.z = this->position.z;
    BB.radius = this->width / 2.0;
    return BB;
}