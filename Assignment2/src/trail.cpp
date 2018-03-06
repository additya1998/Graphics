#include "trail.h"
#include "main.h"
#include "helper.h"

Trail::Trail(float x, float y, float z, color_t color) {

    this->position = glm::vec3(x, y, z);
    this->time = 15;
    this->active = true;
    GLfloat vertex_buffer_data[108];

    int pos = 0;
    float upper = 0.15;
    Point A(-upper, 0, -upper), B(-upper, 0, upper), C(upper, 0, upper), D(upper, 0, -upper);
    
    vector<float> t; t.clear();
    get_quad(A, B, C, D, t); 
    
    for(int i=0; i<t.size(); ++i) vertex_buffer_data[pos++] = t[i];

    this->object = create3DObject(GL_TRIANGLES, pos / 3, vertex_buffer_data, color, GL_FILL);
}

void Trail::draw(glm::mat4 VP) {
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

void Trail::set_position(float x, float y) {
    // this->position = glm::vec3(x, y, 0);
}

void Trail::tick() {
	--this->time;
	if(this->time == 0) this->active = false;
	// ++this->update_time;
	// if(this->update_time > 0 and this->update_time < 20) this->position.y += 0.005;
	// else if(this->update_time >= 20 and this->update_time <= 40) this->position.y -= 0.005;
	// else this->update_time = 0;
}

bounding_box_t Trail::getBoundingBox(){
    bounding_box_t BB;
    return BB;
}