#include "island.h"
#include "main.h"
#include "helper.h"

Island::Island(float x, float y, float z, float radius, color_t color) {

    this->position = glm::vec3(x, y, z);
    this->radius = radius;
    this->rotation = 0;

    vector<float> t, t_treasure;
    int pos = 0, pos_treasure = 0;
    GLfloat vertex_buffer_data[1080], vertex_buffer_treasure[1080];

    int sides = 50;
    float cur = 0, add = 2 * M_PI / sides;
    for(int i=0; i<50; ++i){
        Point center(0, 0, 0);
        Point A(radius * cos(cur), 0, radius * sin(cur));
        Point B(radius * cos(cur + add), 0, radius * sin(cur + add));
        get_triangle(center, A, B, t);
        cur = cur + add;
    }

    Point A(0, 0, 0);
    get_box(A, 2, 2, 2, t_treasure);
    
    for(int i=0; i<t.size(); ++i) vertex_buffer_data[pos++] = t[i]; 
    for(int i=0; i<t_treasure.size(); ++i) vertex_buffer_treasure[pos_treasure++] = t_treasure[i]; 

    this->object = create3DObject(GL_TRIANGLES, pos / 3, vertex_buffer_data, COLOR_GREEN, GL_FILL);
    this->treasure = create3DObject(GL_TRIANGLES, pos_treasure / 3, vertex_buffer_treasure, COLOR_BROWN, GL_FILL);
}

void Island::draw(glm::mat4 VP) {
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
    draw3DObject(this->treasure);
}

void Island::set_position(float x, float y) {
    // this->position = glm::vec3(x, y, 0);
}

void Island::tick() {
	// ++this->update_time;
	// if(this->update_time > 0 and this->update_time < 20) this->position.y += 0.005;
	// else if(this->update_time >= 20 and this->update_time <= 40) this->position.y -= 0.005;
	// else this->update_time = 0;
}

bounding_box_t Island::getBoundingBox(){
    bounding_box_t BB;
    BB.x = this->position.x;
    BB.y = this->position.y;
    BB.z = this->position.z;
    BB.radius = this->radius;
    return BB;
}