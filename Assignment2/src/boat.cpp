#include "boat.h"
#include "main.h"
#include "helper.h"

Boat::Boat(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->update_time = 0;
    this->update_time = 0;
    this->speed = 0.15;
    this->length = 2.5;
    this->width = 1;
    this->health = 100;
    this->wind_time = 0;
    this->wind_dir = 1;
    this->cannon_dir = 0;
    this->y_speed = 0;

    int pos_base = 0, pos_sides = 0, pos_rod = 0, pos_flag = 0, pos_cannon;
    GLfloat vertex_buffer_data_base[18], vertex_buffer_data_sides[72], vertex_buffer_data_rod[108], vertex_buffer_data_flag[9], vertex_buffer_data_cannon[108];

    vector<float> t; t.clear();

    float length = 2.5f, width = 1.0f, height = 0.5f;
    Point A(-width / 2.0, 0, -length / 2.0), B(-width / 2.0, 0, length / 2.0), C(width / 2.0, 0, length / 2.0), D(width / 2.0, 0, -length / 2.0);
    Point P((-width / 2.0) - 0.1, height, -length / 2.0), Q((-width / 2.0) - 0.1, height, length / 2.0), R(width / 2.0 + 0.1, height, length / 2.0), S(width / 2.0 + 0.1, height, -length / 2.0);

    get_quad(A, B, C, D, t);
    for(int i=0; i<t.size(); ++i) vertex_buffer_data_base[pos_base++] = t[i];
    t.clear();

    get_quad(A, B, Q, P, t);
    get_quad(D, C, R, S, t);
    get_quad(P, S, D, A, t);
    get_quad(B, C, R, Q, t);
    for(int i=0; i<t.size(); ++i) vertex_buffer_data_sides[pos_sides++] = t[i]; 
    t.clear();
    
    float rod_width = 0.1, rod_depth = 0.1, rod_height = 2.0f;
    
    Point rod_A(-rod_width / 2.0, 0, -rod_depth / 2.0 + 1);
    Point rod_B(-rod_width / 2.0, 0, rod_depth / 2.0 + 1);
    Point rod_C(rod_width / 2.0, 0, rod_depth / 2.0 + 1);
    Point rod_D(rod_width / 2.0, 0, -rod_depth / 2.0 + 1);
    
    Point rod_P(-rod_width / 2.0, rod_height, -rod_depth / 2.0 + 1);
    Point rod_Q(-rod_width / 2.0, rod_height, rod_depth / 2.0 + 1);
    Point rod_R(rod_width / 2.0, rod_height, rod_depth / 2.0 + 1);
    Point rod_S(rod_width / 2.0, rod_height, -rod_depth / 2.0 + 1);
 
    get_quad(rod_A, rod_B, rod_C, rod_D, t);
    get_quad(rod_P, rod_Q, rod_R, rod_S, t);
    get_quad(rod_A, rod_B, rod_Q, rod_P, t);
    get_quad(rod_D, rod_C, rod_R, rod_S, t);
    get_quad(rod_P, rod_S, rod_D, rod_A, t);
    get_quad(rod_B, rod_C, rod_R, rod_Q, t);
    for(int i=0; i<t.size(); ++i) vertex_buffer_data_rod[pos_rod++] = t[i]; 
    t.clear();

    Point flag_A(0, rod_height, rod_depth / 2.0 + 1);
    Point flag_B(0, rod_height - 0.5, rod_depth / 2.0 + 1);
    Point flag_C(0.5, rod_height - 0.25, rod_depth / 2.0 + 1);
    get_triangle(flag_A, flag_B, flag_C, t);
    for(int i=0; i<t.size(); ++i) vertex_buffer_data_flag[pos_flag++] = t[i]; 
    t.clear();

    float cannon_width = 0.25, cannon_depth = 2, cannon_height = 0.2f;
    
    Point cannon_A(-cannon_width / 2.0, 0.5, -cannon_depth / 2.0 - 2);
    Point cannon_B(-cannon_width / 2.0, 0.5, cannon_depth / 2.0 - 2);
    Point cannon_C(cannon_width / 2.0, 0.5, cannon_depth / 2.0 - 2);
    Point cannon_D(cannon_width / 2.0, 0.5, -cannon_depth / 2.0 - 2);
    
    Point cannon_P(-cannon_width / 2.0, 0.5 + cannon_height, -cannon_depth / 2.0 - 2);
    Point cannon_Q(-cannon_width / 2.0, 0.5 + cannon_height, cannon_depth / 2.0 - 2);
    Point cannon_R(cannon_width / 2.0, 0.5 + cannon_height, cannon_depth / 2.0 - 2);
    Point cannon_S(cannon_width / 2.0, 0.5 + cannon_height, -cannon_depth / 2.0 - 2);
 
    get_quad(cannon_A, cannon_B, cannon_C, cannon_D, t);
    get_quad(cannon_P, cannon_Q, cannon_R, cannon_S, t);
    get_quad(cannon_A, cannon_B, cannon_Q, cannon_P, t);
    get_quad(cannon_D, cannon_C, cannon_R, cannon_S, t);
    get_quad(cannon_P, cannon_S, cannon_D, cannon_A, t);
    get_quad(cannon_B, cannon_C, cannon_R, cannon_Q, t);
    for(int i=0; i<t.size(); ++i) vertex_buffer_data_cannon[pos_cannon++] = t[i]; 
    t.clear();

    // printf("%d\n", pos_cannon);

    this->object = create3DObject(GL_TRIANGLES, pos_base / 3, vertex_buffer_data_base, COLOR_YELLOW, GL_FILL);
    this->object_1 = create3DObject(GL_TRIANGLES, pos_sides / 3, vertex_buffer_data_sides, COLOR_RED, GL_FILL);
    this->mast_rod = create3DObject(GL_TRIANGLES, pos_rod / 3, vertex_buffer_data_rod, COLOR_BROWN, GL_FILL);
    this->flag = create3DObject(GL_TRIANGLES, pos_flag / 3, vertex_buffer_data_flag, COLOR_RED, GL_FILL);
    this->cannon = create3DObject(GL_TRIANGLES, pos_cannon / 3, vertex_buffer_data_cannon, COLOR_BROWN, GL_FILL);
}

void Boat::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 wind_rotate    = glm::rotate((float) (this->wind_dir * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * wind_rotate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object_1);
    draw3DObject(this->mast_rod);
    draw3DObject(this->flag);

    glm::mat4 translate_cannon = glm::translate (glm::vec3 (0, 0, -1));
    glm::mat4 translate_revert = glm::translate (glm::vec3 (0, 0, 1));
    translate = glm::translate (this->position);    // glTranslatef
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 cannon    = glm::rotate((float) (this->cannon_dir * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate * translate_cannon * cannon * translate_revert);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);    
    glUniform1f(Matrices.Shader, 0.5);
    draw3DObject(this->cannon);
}

void Boat::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Boat::tick() {
	++this->update_time;
	if(this->update_time > 0 and this->update_time < 20) this->position.y += 0.005;
	else if(this->update_time >= 20 and this->update_time <= 40) this->position.y -= 0.005;
	else this->update_time = 0;
    ++this->speed_time;
    if(this->speed_time > 6000) this->speed = 0.25, this->speed_time = 0;
    ++this->wind_time;
    if(this->wind_time < 120) this->wind_dir = this->wind_time / 8;
    else if(this->wind_time < 360)  this->wind_dir = (240 - this->wind_time) / 8;
    else this->wind_dir = (this->wind_time - 480) / 8;
    if(this->wind_time > 480) this->wind_time = 0;
    if(this->wind_dir < 0) this->position.x += 0.01;
    else this->position.x -= 0.01;

    if(this->position.y > 0 or this->y_speed > 0){
        this->position.y = this->position.y + ((this->y_speed * (1.0 / 60)) + 0.5 * (-10) * (1.0 / 60) * (1.0 / 60));
        if(this->position.y < -0.5) this->position.y = -0.5;
        this->y_speed = this->y_speed + (-10 * (1.0 / 60));
    }
}

bounding_box_t Boat::getBoundingBox(){
    bounding_box_t BB;
    BB.x = this->position.x;
    BB.y = this->position.y;
    BB.z = this->position.z;
    BB.radius = this->length / 2.0;
    return BB;
}