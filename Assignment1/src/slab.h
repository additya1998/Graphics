#include "main.h"

#ifndef SLAB_H
#define SLAB_H

class Slab {
public:
	Slab() {};

	Slab(float x, float y, int angle, float length, float width, color_t color){

		this->position = glm::vec3(x, y, 0);
		this->length = length;
		this->width = width;
		this->rotation = angle;

		float f_x = (this->width / 2.0), f_y = (this->length / 2.0);
		float s_x = -(this->width / 2.0), s_y = (this->length / 2.0);

		float t_x = -(this->width / 2.0), t_y = -(this->length / 2.0) ;
		float ff_x = (this->width / 2.0), ff_y = -(this->length / 2.0);

		static GLfloat vertex_buffer_data[] = {
			f_x, f_y, 0,
			s_x, s_y, 0,
			t_x, t_y, 0,

			f_x, f_y, 0,
			ff_x, ff_y, 0,
			t_x, t_y, 0
		};

		this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);

	}

	glm::vec3 position;
	float rotation, radius;
	float length, width;
	void draw(glm::mat4 VP){
		Matrices.model = glm::mat4(1.0f);
		glm::mat4 translate = glm::translate (this->position);    // glTranslatef
		glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
		rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
		Matrices.model *= (translate * rotate);
		glm::mat4 MVP = VP * Matrices.model;
		glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
		draw3DObject(this->object);
	};

	void tick(float x_shift, float y_shift){
		this->position.x -= x_shift;
		this->position.y -= y_shift;
	}

private:
	VAO *object;
};

// Slab::Slab(float x, float y, int angle, float length, float width, color_t color){
//     this->position = glm::vec3(x, y, 0);
//     this->angle = angle;
//     this->length = length;
//     this->width = width;

//     static const GLfloat vertex_buffer_data[] = {
// 		-this->width / 2.0, this->length / 2.0, 0, 
//     	-this->width / 2.0, -this->length / 2.0, 0, 
//     	this->width / 2.0, -this->length / 2.0, 0, 

//     	-this->width / 2.0, this->length / 2.0, 0, 
//     	this->width / 2.0, this->length / 2.0, 0, 
//     	this->width / 2.0, -this->length / 2.0, 0, 
//     };

//     // this->object = create3DObject(GL_TRIANGLES, position, vertex_buffer_data, color, GL_FILL);
// }

// void Slab::draw(glm::mat4 VP) {
//     Matrices.model = glm::mat4(1.0f);
//     glm::mat4 translate = glm::translate (this->position);    // glTranslatef
//     glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
//     rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
//     Matrices.model *= (translate * rotate);
//     glm::mat4 MVP = VP * Matrices.model;
//     glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
//     draw3DObject(this->object);
// }

#endif // SLAB_H
