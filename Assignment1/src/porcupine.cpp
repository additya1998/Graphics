#include "main.h"
#include "porcupine.h"

Porcupine::Porcupine(float x, float y, float width, float height, color_t color) {

    this->position = glm::vec3(x, y, 0);
    this->x_speed = 0.05;
    this->width = width;
    this->height = height;
    this->active = true;

    GLfloat vertex_buffer_data[] = {
		-3.0 * width / 2.0, 0.0f, 0.0f,
		-width, height, 0.0f,
		-width / 2.0, 0.0f, 0.0f,

		-width / 2.0, 0.0f, 0.0f,
		0.0f, height, 0,
		width / 2.0, 0, 0,

		width / 2.0, 0, 0,
		width, height, 0,
		3.0 * width / 2.0, 0, 0
    };

    this->object = create3DObject(GL_TRIANGLES, 9, vertex_buffer_data, color, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Porcupine::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Porcupine::tick() {
    this->position.x += this->x_speed;
}
