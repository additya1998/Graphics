#include "main.h"
#include "trampoline.h"

Trampoline::Trampoline(float x, float y, float radius, color_t color, float height) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius = radius;

    static int position = 0;
    static GLfloat vertex_buffer_data[100 * 3 * 3 * 1000];

    int sides = 100;
    double angle = 3.14159265359, add = (180 * 3.14159265359) / (180 * sides);
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
        if(angle > 3.14159265359 + 3.14159265359) break;
    }

    // Right leg
    vertex_buffer_data[position++] = radius; vertex_buffer_data[position++] = 0.0f; vertex_buffer_data[position++] = 0.0f;
    vertex_buffer_data[position++] = radius + 0.15; vertex_buffer_data[position++] = 0.0f; vertex_buffer_data[position++] = 0.0f;
    vertex_buffer_data[position++] = radius; vertex_buffer_data[position++] = -height; vertex_buffer_data[position++] = 0.0f;

    vertex_buffer_data[position++] = radius + 0.15; vertex_buffer_data[position++] = 0.0f; vertex_buffer_data[position++] = 0.0f;
    vertex_buffer_data[position++] = radius; vertex_buffer_data[position++] = -height; vertex_buffer_data[position++] = 0.0f;
    vertex_buffer_data[position++] = radius + 0.15; vertex_buffer_data[position++] = -height; vertex_buffer_data[position++] = 0.0f;

    // Left leg
    vertex_buffer_data[position++] = -radius - 0.15; vertex_buffer_data[position++] = 0.0f; vertex_buffer_data[position++] = 0.0f;
    vertex_buffer_data[position++] = -radius; vertex_buffer_data[position++] = 0.0f; vertex_buffer_data[position++] = 0.0f;
    vertex_buffer_data[position++] = -radius - 0.15; vertex_buffer_data[position++] = -height; vertex_buffer_data[position++] = 0.0f;

    vertex_buffer_data[position++] = -radius; vertex_buffer_data[position++] = 0.0f; vertex_buffer_data[position++] = 0.0f;
    vertex_buffer_data[position++] = -radius - 0.15; vertex_buffer_data[position++] = -height; vertex_buffer_data[position++] = 0.0f;
    vertex_buffer_data[position++] = -radius; vertex_buffer_data[position++] = -height; vertex_buffer_data[position++] = 0.0f;
    
    this->object = create3DObject(GL_TRIANGLES, position, vertex_buffer_data, color, GL_FILL);
}

void Trampoline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Trampoline::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Trampoline::tick() {
	
}
