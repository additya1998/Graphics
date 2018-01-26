#include "player.h"
#include "main.h"

Player::Player(float x, float y, float radius, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius = radius;
    this->x_speed = 0;
    this->y_speed = 0;

    int position = 0;
    static GLfloat vertex_buffer_data[100 * 3 * 3 * 1000];

    int sides = 100;
    double angle = 0, add = (360 * 3.14159265359) / (180 * sides);
    for(int i=1; i<=sides; ++i){
        vertex_buffer_data[position++] = 0.0f;
        vertex_buffer_data[position++] = 0.0f;
        vertex_buffer_data[position++] = 0.0f;

        vertex_buffer_data[position++] = radius * cos(angle);
        vertex_buffer_data[position++] = radius * sin(angle);
        vertex_buffer_data[position++] = 0.0f;

        vertex_buffer_data[position++] = radius * cos(angle + add);
        vertex_buffer_data[position++] = radius * sin(angle + add);
        vertex_buffer_data[position++] = 0.0f;

        angle = angle + add;
    }

    this->object = create3DObject(GL_TRIANGLES, position, vertex_buffer_data, color, GL_FILL);
}

void Player::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Player::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Player::tick(){
	this->position.y = this->position.y + (this->y_speed * (1 / 60)) + 0.5 * (-10) * (1.0 / 60) * (1.0 / 60);
	this->y_speed = this->y_speed + (- 10 * (1.0 / 60));
}
