#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, float radius, int health, float score, color_t color, bool has_slab, int slab_angle) {

	// slab_angle is parametric co-ordinate of point where slab touches circle

    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius = radius;
    this->x_speed = 0.05;
    this->y_speed = 0;
    this->active = true;
    this->has_slab = has_slab;
    this->health = health;
    this->score = score;

    int position = 0;
    GLfloat vertex_buffer_data[100 * 3 * 3 * 1000];

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

    if(has_slab){
		this->has_slab = true;
		float slab_w = 0.05, slab_l = 1.5;
    	double slab_x = this->position.x + ((radius + (slab_w / 2.0)) * cos(slab_angle * M_PI / 180.0));
    	double slab_y = this->position.y + ((radius + (slab_w / 2.0)) * sin(slab_angle * M_PI / 180.0));
    	this->slab = Slab(slab_x, slab_y, slab_angle, slab_l, slab_w, COLOR_BROWN);
    }

    this->object = create3DObject(GL_TRIANGLES, position, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    if(this->has_slab) this->slab.draw(VP);
}

// void Ball::set_position(float x, float y) {
//     this->position = glm::vec3(x, y, 0);
// }

void Ball::tick() {
    this->position.x -= this->x_speed;
    this->position.y -= this->y_speed;
    if(this->has_slab) this->slab.tick(this->x_speed, this->y_speed);
}
