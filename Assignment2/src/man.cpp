#include "man.h"
#include "main.h"
#include "helper.h"

Man::Man(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->on_boat = true;
    this->speed = 0.15;
    this->angle = 0;
    this->update_time = 0;

    // this->update_time = 0;
    // this->speed = 0.15;
    // this->length = 2.5;
    // this->width = 1;
    // this->health = 100;
    // this->wind_time = 0;
    // this->wind_dir = 1;
    // this->cannon_dir = 0;
    this->y_speed = 0;

    float body_depth = 0.25f;
    float torso_height = 0.75f, leg_height = 0.75f, arm_height = 0.5f, face_height = 0.25f;
    float torso_width = 0.5, face_width = 0.25f, arm_width = 0.1f, leg_width = 0.1f;   

    Point torso(0, 0, 0);
    Point left_leg(torso.x - (torso_width / 2.0) + (leg_width / 2.0), torso.y - ((torso_height + leg_height) / 2.0), 0);
    Point right_leg(torso.x + (torso_width - leg_width) / 2.0, torso.y - ((torso_height + leg_height) / 2.0), 0);
    Point left_arm(torso.x - ((torso_width + arm_width) / 2.0), torso.y + ((torso_height - arm_height) / 2.0), 0);
    Point right_arm(torso.x + ((torso_width + arm_width) / 2.0), torso.y + ((torso_height - arm_height) / 2.0), 0);
    Point face(torso.x, torso.y + ((torso_height + face_height) / 2.0), 0);

    vector<float> face_v, torso_v, left_leg_v, right_leg_v, left_arm_v, right_arm_v;
    int pos_face = 0, pos_torso = 0, pos_left_leg = 0, pos_right_leg = 0, pos_left_arm = 0, pos_right_arm = 0;
    GLfloat vertex_face[1080], vertex_torso[1080], vertex_left_leg[1080], vertex_right_leg[1080], vertex_left_arm[1080], vertex_right_arm[1080];

    get_box(face, face_height, face_width, body_depth, face_v);
    get_box(torso, torso_height, torso_width, body_depth, torso_v);
    get_box(left_leg, leg_height, leg_width, body_depth, left_leg_v);
    get_box(right_leg, leg_height, leg_width, body_depth, right_leg_v);
    get_box(left_arm, arm_height, arm_width, body_depth, left_arm_v);
    get_box(right_arm, arm_height, arm_width, body_depth, right_arm_v);

    for(int i=0; i<face_v.size(); ++i) vertex_face[pos_face++] = face_v[i];
    for(int i=0; i<torso_v.size(); ++i) vertex_torso[pos_torso++] = torso_v[i];
    for(int i=0; i<left_arm_v.size(); ++i) vertex_left_arm[pos_left_arm++] = left_arm_v[i];
    for(int i=0; i<right_arm_v.size(); ++i) vertex_right_arm[pos_right_arm++] = right_arm_v[i];
    for(int i=0; i<left_leg_v.size(); ++i) vertex_left_leg[pos_left_leg++] = left_leg_v[i];
    for(int i=0; i<right_leg_v.size(); ++i) vertex_right_leg[pos_right_leg++] = right_leg_v[i];
    // printf("%d\n", pos_left_arm);

    this->face = create3DObject(GL_TRIANGLES, pos_face / 3, vertex_face, COLOR_SKIN, GL_FILL);
    this->torso = create3DObject(GL_TRIANGLES, pos_torso / 3, vertex_torso, COLOR_RED, GL_FILL);
    this->left_arm = create3DObject(GL_TRIANGLES, pos_left_arm / 3, vertex_left_arm, COLOR_SKIN, GL_FILL);
    this->right_arm = create3DObject(GL_TRIANGLES, pos_right_arm / 3, vertex_right_arm, COLOR_SKIN, GL_FILL);
    this->left_leg = create3DObject(GL_TRIANGLES, pos_left_leg / 3, vertex_left_leg, COLOR_BLUE, GL_FILL);
    this->right_leg = create3DObject(GL_TRIANGLES, pos_right_leg / 3, vertex_right_leg, COLOR_BLUE, GL_FILL);
}

void Man::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Shader, 0.5);
    draw3DObject(this->face);
    draw3DObject(this->torso);

    glm::mat4 translate_arm = glm::translate (glm::vec3 (0, -1.5, 0));
    glm::mat4 revert_arm = glm::translate (glm::vec3 (0, 1.5, 0));
    glm::mat4 translate_leg = glm::translate (glm::vec3 (0, -0.75, 0));
    glm::mat4 revert_leg = glm::translate (glm::vec3 (0, 0.75, 0));

    glm::mat4 needed_rotation = glm::rotate((float) (this->angle * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 opposite_rotation = glm::rotate((float) (-1 * this->angle * M_PI / 180.0f), glm::vec3(1, 0, 0));

    Matrices.model = glm::mat4(1.0f);
    translate = glm::translate (this->position);
    rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    Matrices.model *= (translate * rotate * translate_arm * needed_rotation * revert_arm);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Shader, 0.5);
    draw3DObject(this->left_arm);
    
    Matrices.model = glm::mat4(1.0f);
    translate = glm::translate (this->position);
    rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    Matrices.model *= (translate * rotate * translate_arm * (opposite_rotation) * revert_arm);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Shader, 0.5);
    draw3DObject(this->right_arm);


    Matrices.model = glm::mat4(1.0f);
    translate = glm::translate (this->position);
    rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    Matrices.model *= (translate * rotate * translate_leg * needed_rotation * revert_leg);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Shader, 0.5);
    draw3DObject(this->right_leg);
    
    Matrices.model = glm::mat4(1.0f);
    translate = glm::translate (this->position);
    rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    Matrices.model *= (translate * rotate * translate_leg * (opposite_rotation) * revert_leg);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Shader, 0.5);
    draw3DObject(this->left_leg);
}

void Man::set_position(float x, float y) {
    // this->position = glm::vec3(x, y, 0);
}

void Man::tick() {
	// ++this->update_time;
	// if(this->update_time > 0 and this->update_time < 20) this->position.y += 0.005;
	// else if(this->update_time >= 20 and this->update_time <= 40) this->position.y -= 0.005;
	// else this->update_time = 0;
    // ++this->speed_time;
    // if(this->speed_time > 6000) this->speed = 0.25, this->speed_time = 0;
    // ++this->wind_time;
    // if(this->wind_time < 120) this->wind_dir = this->wind_time / 8;
    // else if(this->wind_time < 360)  this->wind_dir = (240 - this->wind_time) / 8;
    // else this->wind_dir = (this->wind_time - 480) / 8;
    // if(this->wind_time > 480) this->wind_time = 0;
    // if(this->wind_dir < 0) this->position.x += 0.01;
    // else this->position.x -= 0.01;

    // if(this->position.y > 0 or this->y_speed > 0){
    //     this->position.y = this->position.y + ((this->y_speed * (1.0 / 60)) + 0.5 * (-10) * (1.0 / 60) * (1.0 / 60));
    //     if(this->position.y < -0.5) this->position.y = -0.5;
    //     this->y_speed = this->y_speed + (-10 * (1.0 / 60));
    // }
}

bounding_box_t Man::getBoundingBox(){
    bounding_box_t BB;
    BB.x = this->position.x;
    BB.y = this->position.y;
    BB.z = this->position.z;
    BB.radius = this->length / 2.0;
    return BB;
}