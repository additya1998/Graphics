#include <vector>
#include "helper.h"

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