#include "boat.h"
#include "main.h"
#include "helper.h"

Boat::Boat(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->update_time = 0;
    this->length = 4;
    this->width = 1;
    speed = 1;

    int pos = 0;
    GLfloat vertex_buffer_data[90];

    vector<float> t; t.clear();

    Point A(0, 0, -2), B(-0.5, 0, 0), C(0, 0, 2), D(0.5, 0, 0);
    // Point P(0, 0.5, -2), Q(-0.7, 0.5, 0), R(0, 0.5, 2), S(0.1, 0.5, 0);


    get_triangle(A, B, D, t);
    get_triangle(B, C, D, t);

    // get_quad(A, B, Q, P, t);
    // get_quad(B, C, R, Q, t);
    // get_quad(C, D, S, R, t);
    // get_quad(D, A, R, S, t);

    // for(int i=0; i<t.size(); ++i) printf("%f\n", t[i]); 
    for(int i=0; i<t.size(); ++i) vertex_buffer_data[pos++] = t[i]; 
    printf("%d\n", pos);

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
  //   GLfloat vertex_buffer_data[] = {

  //   	0, 0, -2.5f,
		// 0, 0, 1.5f,
		// 0.5f, 0, 0,

  //   	0, 0, -2.5f,
		// 0, 0, 1.5f,
		// -0.5f, 0, 0,


  //       // -0.5f,-0.5f,-0.5f, // triangle 1 : begin
  //       // -0.5f,-0.5f, 0.5f,
  //       // -0.5f, 0.5f, 0.5f, // triangle 1 : end

  //       // 0.5f, 0.5f,-0.5f, // triangle 2 : begin
  //       // -0.5f,-0.5f,-0.5f,
  //       // -0.5f, 0.5f,-0.5f, // triangle 2 : end

  //       // 0.5f,-0.5f, 0.5f,
  //       // -0.5f,-0.5f,-0.5f,
  //       // 0.5f,-0.5f,-0.5f,

  //       // 0.5f, 0.5f,-0.5f,
  //       // 0.5f,-0.5f,-0.5f,
  //       // -0.5f,-0.5f,-0.5f,

  //       // -0.5f,-0.5f,-0.5f,
  //       // -0.5f, 0.5f, 0.5f,
  //       // -0.5f, 0.5f,-0.5f,

  //       // 0.5f,-0.5f, 0.5f,
  //       // -0.5f,-0.5f, 0.5f,
  //       // -0.5f,-0.5f,-0.5f,

  //       // -0.5f, 0.5f, 0.5f,
  //       // -0.5f,-0.5f, 0.5f,
  //       // 0.5f,-0.5f, 0.5f,

  //       // 0.5f, 0.5f, 0.5f,
  //       // 0.5f,-0.5f,-0.5f,
  //       // 0.5f, 0.5f,-0.5f,

  //       // 0.5f,-0.5f,-0.5f,
  //       // 0.5f, 0.5f, 0.5f,
  //       // 0.5f,-0.5f, 0.5f,

  //       // 0.5f, 0.5f, 0.5f,
  //       // 0.5f, 0.5f,-0.5f,
  //       // -0.5f, 0.5f,-0.5f,

  //       // 0.5f, 0.5f, 0.5f,
  //       // -0.5f, 0.5f,-0.5f,
  //       // -0.5f, 0.5f, 0.5f,

  //       // 0.5f, 0.5f, 0.5f,
  //       // -0.5f, 0.5f, 0.5f,
  //       // 0.5f,-0.5f, 0.5f
  //   };

    this->object = create3DObject(GL_TRIANGLES, pos / 3, vertex_buffer_data, color, GL_FILL);
}

void Boat::draw(glm::mat4 VP) {
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

void Boat::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Boat::tick() {
	++this->update_time;
	if(this->update_time > 0 and this->update_time < 20) this->position.y += 0.005;
	else if(this->update_time >= 20 and this->update_time <= 40) this->position.y -= 0.005;
	else this->update_time = 0;
}

bounding_box_t Boat::getBoundingBox(){
    bounding_box_t BB;
    BB.x = this->position.x;
    BB.y = this->position.y;
    BB.z = this->position.z;
    BB.radius = this->length / 2.0;
    return BB;
}