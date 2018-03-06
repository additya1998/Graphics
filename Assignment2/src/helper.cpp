#include <vector>
#include <stdio.h>
#include "helper.h"
#include <math.h>

using namespace std;

void get_triangle(Point A, Point B, Point C, vector<float> &ans){
	ans.push_back(A.x); ans.push_back(A.y); ans.push_back(A.z);
	ans.push_back(B.x); ans.push_back(B.y); ans.push_back(B.z);
	ans.push_back(C.x); ans.push_back(C.y); ans.push_back(C.z);
}

void get_quad(Point A, Point B, Point C, Point D, vector<float> &ans){
	get_triangle(A, B, C, ans);
	get_triangle(A, D, C, ans);
}

void get_box(Point center, float height, float width, float depth, vector<float> &ans){
	Point A(center.x - width / 2.0, center.y - height / 2.0, center.z - depth / 2.0);
	Point B(center.x - width / 2.0, center.y - height / 2.0, center.z + depth / 2.0);
	Point C(center.x + width / 2.0, center.y - height / 2.0, center.z + depth / 2.0);
	Point D(center.x + width / 2.0, center.y - height / 2.0, center.z - depth / 2.0);

	Point P(center.x - width / 2.0, center.y + height / 2.0, center.z - depth / 2.0);
	Point Q(center.x - width / 2.0, center.y + height / 2.0, center.z + depth / 2.0);
	Point R(center.x + width / 2.0, center.y + height / 2.0, center.z + depth / 2.0);
	Point S(center.x + width / 2.0, center.y + height / 2.0, center.z - depth / 2.0);

    get_quad(A, B, C, D, ans);
    get_quad(P, Q, R, S, ans);
    get_quad(A, B, Q, P, ans);
    get_quad(D, C, R, S, ans);
    get_quad(P, S, D, A, ans);
    get_quad(B, C, R, Q, ans);
}

void get_sphere(Point center, float radius, vector<float> &ans){
    int k = 50;
    for (int i=0 ; i<k ; ++i){
        float y_up = (2 * radius * (k - i)) / k;
        float y_down = (2 * radius * (k - i - 1)) / k;
        y_up -= radius;
        y_down -= radius;
        float first_radius = radius * radius - y_up * y_up;
        first_radius = sqrt(first_radius);
        float second_radius = radius * radius - y_down * y_down;
        second_radius = sqrt(second_radius);
        float offset = 360.0f / k;
        for (int j=0 ; j<k ; ++j){
            float first_angle = j * offset * M_PI / 180.0f, second_angle = (j + 1) * offset * M_PI / 180.0f;
            Point A(center.x + (first_radius * cos(first_angle)), center.y + (first_radius * sin(first_angle)), center.z + y_up);
            Point B(center.x + (second_radius * cos(first_angle)), center.y + (second_radius * sin(first_angle)), center.z + y_down);
            Point C(center.x + (second_radius * cos(second_angle)), center.y + (second_radius * sin(second_angle)), center.z + y_down);
            Point D(center.x + (first_radius * cos(second_angle)), center.y + (first_radius * sin(second_angle)), center.z + y_up);
            get_quad(A, B, C, D, ans);
        }
    }
}
