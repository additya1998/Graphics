#include "main.h"

#ifndef GROUND_H
#define GROUND_H

class Ground {
public:
	Ground() {};
	Ground(float x, float y, float width, float height, color_t color);
    glm::vec3 position;
    float height, width;
    void draw(glm::mat4 VP) {
	    Matrices.model = glm::mat4(1.0f);
	    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
	    Matrices.model *= translate;
	    glm::mat4 MVP = VP * Matrices.model;
	    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	    draw3DObject(this->object);
    };
private:
    VAO *object;
};

Ground::Ground(float x, float y, float width, float height, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->height = height;
    this->width = width;
    
    printf("Center: %f %f\n", x, y);	
    printf("W, H: %f %f\n", width, height);

    printf("%f %f\n", this->position.x - (this->width / 2.0), this->position.y + (this->height / 2.0));
    printf("%f %f\n", this->position.x - (this->width / 2.0), this->position.y - (this->height / 2.0));
    printf("%f %f\n", this->position.x + (this->width / 2.0), this->position.y - (this->height / 2.0));

    printf("%f %f\n", this->position.x - (this->width / 2.0), this->position.y + (this->height / 2.0)); 
    printf("%f %f\n", this->position.x + (this->width / 2.0), this->position.y + (this->height / 2.0)); 
    printf("%f %f\n", this->position.x + (this->width / 2.0), this->position.y - (this->height / 2.0)); 

    static const GLfloat vertex_buffer_data[] = {
    	this->position.x - (this->width / 2.0), this->position.y + (this->height / 2.0), 0, 
    	this->position.x - (this->width / 2.0), this->position.y - (this->height / 2.0), 0, 
    	this->position.x + (this->width / 2.0), this->position.y - (this->height / 2.0), 0, 

    	this->position.x - (this->width / 2.0), this->position.y + (this->height / 2.0), 0, 
    	this->position.x + (this->width / 2.0), this->position.y + (this->height / 2.0), 0, 
    	this->position.x + (this->width / 2.0), this->position.y - (this->height / 2.0), 0, 
    };

    // printf("%f\n", this->position.y + (this->height / 2));

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}


#endif // GROUND_H

