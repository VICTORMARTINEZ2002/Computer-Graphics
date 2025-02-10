#include "screen.h"
#include <windows.h>
#include <numeric>

struct vec3{
	float x,y,z;
};

struct connection{
	int a,b;
};

// The first transformation modifies point.y, then immediately uses the modified value when updating point.z, leading to incorrect results.
// Result -> cube gets smaller with is very funny
void rotate(vec3& point, float x=1, float y=2, float z=1){
	float rad = 0;

	rad = x;
	point.y =  std::cos(rad) * point.y - std::sin(rad) * point.z;
	point.z =  std::sin(rad) * point.y + std::cos(rad) * point.z;

	rad = y;
	point.x =  std::cos(rad) * point.x + std::sin(rad) * point.z;
	point.z = -std::sin(rad) * point.x + std::cos(rad) * point.z;

	rad = z;
	point.x =  std::cos(rad) * point.x - std::sin(rad) * point.y;
	point.y =  std::sin(rad) * point.x + std::cos(rad) * point.y;
}

void rotate2(vec3& point, float x=1, float y=2, float z=1) {
    float rad;
    float temp_x, temp_y, temp_z;

    // Rotation around X-axis
    rad = x;
    temp_y = std::cos(rad) * point.y - std::sin(rad) * point.z;
    temp_z = std::sin(rad) * point.y + std::cos(rad) * point.z;
    point.y = temp_y;
    point.z = temp_z;

    // Rotation around Y-axis
    rad = y;
    temp_x = std::cos(rad) * point.x + std::sin(rad) * point.z;
    temp_z = -std::sin(rad) * point.x + std::cos(rad) * point.z;
    point.x = temp_x;
    point.z = temp_z;

    // Rotation around Z-axis
    rad = z;
    temp_x = std::cos(rad) * point.x - std::sin(rad) * point.y;
    temp_y = std::sin(rad) * point.x + std::cos(rad) * point.y;
    point.x = temp_x;
    point.y = temp_y;
}


void line(Screen& screen, float x1, float y1, float x2, float y2){
	float dx = x2-x1;
	float dy = y2-y1;

	float length = std::sqrt(dx*dx + dy*dy);
	float angle  = std::atan2(dy,dx);

	for(int i=0; i<length; i++){
		screen.pixel(
			x1 + std::cos(angle)*i,
			y1 + std::sin(angle)*i
		);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Screen screen;

	std::vector<vec3> points{
		{100,100,100},
		{200,100,100},
		{200,200,100},
		{100,200,100},

		{100,100,200},
		{200,100,200},
		{200,200,200},
		{100,200,200},
	};

	std::vector<connection> connections{
		{0,1},{1,2},{2,3},{3,0},
		{4,5},{5,6},{6,7},{7,4},
		{0,4},{1,5},{2,6},{3,7}
	};

	// Calculate centroid
	vec3 c = {0,0,0};
	for(auto& p:points){
		c.x += p.x;
		c.y += p.y;
		c.z += p.z;
	}
	c.x /= points.size();
	c.y /= points.size();
	c.z /= points.size();

	while(true){
		for(auto& p : points){
			p.x -= c.x;
			p.y -= c.y;
			p.z -= c.z;
			rotate(p, 0.04, 0.02, 0.08);
			p.x += c.x;
			p.y += c.y;
			p.z += c.z;
			screen.pixel(p.x, p.y);
		}

		for(auto& conn : connections){
			line(screen,
				points[conn.a].x,
				points[conn.a].y,
				points[conn.b].x,
				points[conn.b].y);
		}

		screen.show();
		screen.clear();
		screen.input();
		SDL_Delay(30);
	}

	return 0;
}
