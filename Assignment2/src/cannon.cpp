#include "cannon.h"
#include "main.h"
#include "helper.h"

Cannon::Cannon(float x, float y, float z, float angle, bool fire_ball, color_t color) {

    this->position = glm::vec3(x, y, z);
    this->active = false;
    this->radius = 0.25;
    this->x_speed = 0.25;
    this->y_speed = 5;
    this->angle = angle;
    this->fire_ball = fire_ball;

    GLfloat vertex_buffer_data[45000];
    int pos = 0;
    vector<float> t;

    Point center(0, 0, 0);
    get_sphere(center, this->radius, t);

    for(int i=0; i<t.size(); ++i) vertex_buffer_data[pos++] = t[i]; 

    this->object = create3DObject(GL_TRIANGLES, pos / 3, vertex_buffer_data, color, GL_FILL);
}

void Cannon::draw(glm::mat4 VP) {
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
}

void Cannon::set_position(float x, float y) {
    // this->position = glm::vec3(x, y, 0);
}

void Cannon::tick() {
    if(!this->fire_ball){
        this->position.x = this->position.x - this->x_speed * sin(this->angle * M_PI / 180);   
        this->position.z = this->position.z - this->x_speed * cos(this->angle * M_PI / 180);
    }
    this->y_speed = this->y_speed + (-10 * (1.0 / 60));
    this->position.y = this->position.y + ((this->y_speed * (1.0 / 60)) + 0.5 * (-10) * (1.0 / 60) * (1.0 / 60));
    if(this->position.y < -1) this->active = false;
}

bounding_box_t Cannon::getBoundingBox(){
    bounding_box_t BB;
    BB.x = this->position.x;
    BB.y = this->position.y;
    BB.z = this->position.z;
    BB.radius = this->radius;
    return BB;
}