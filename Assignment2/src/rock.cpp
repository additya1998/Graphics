#include "rock.h"
#include "main.h"
#include "helper.h"

Rock::Rock(float x, float y, float z, color_t color) {

    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->update_time = 0;
    this->active = true;
    GLfloat vertex_buffer_data[108];

    int pos = 0; float height = 0.5f, upper = 0.15f, lower = 0.25f;
    Point A(-upper, height, -upper), B(-upper, height, upper), C(upper, height, upper), D(upper, height, -upper);
    Point P(-lower, 0, -lower), Q(-lower, 0, lower), R(lower, 0, lower), S(lower, 0, -lower);
    
    vector<float> t; t.clear();
    get_quad(A, B, C, D, t); 
    get_quad(A, B, Q, P, t); 
    get_quad(B, C, R, Q, t); 
    get_quad(D, C, R, S, t); 
    get_quad(A, D, S, P, t); 
    get_quad(P, Q, R, S, t); 
    
    for(int i=0; i<t.size(); ++i) vertex_buffer_data[pos++] = t[i]; 

    this->object = create3DObject(GL_TRIANGLES, sizeof(vertex_buffer_data) / (3 * sizeof(float)), vertex_buffer_data, color, GL_FILL);
}

void Rock::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Rock::set_position(float x, float y) {
    // this->position = glm::vec3(x, y, 0);
}

void Rock::tick() {
	// ++this->update_time;
	// if(this->update_time > 0 and this->update_time < 20) this->position.y += 0.005;
	// else if(this->update_time >= 20 and this->update_time <= 40) this->position.y -= 0.005;
	// else this->update_time = 0;
}

bounding_box_t Rock::getBoundingBox(){
    bounding_box_t BB;
    BB.x = this->position.x;
    BB.y = this->position.y;
    BB.z = this->position.z;
    BB.radius = max(this->length / 2.0, max(this->width / 2.0, this->height / 2.0));
    return BB;
}