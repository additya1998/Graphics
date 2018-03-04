#include <vector>
using namespace std;

struct Point{
	Point(float _x = 0, float _y = 0, float _z = 0){
		x = _x, y = _y, z = _z;
	}
	float x, y, z;
};

void get_triangle(Point A, Point B, Point C, vector<float> &t);
void get_quad(Point A, Point B, Point C, Point D, vector<float> &t);