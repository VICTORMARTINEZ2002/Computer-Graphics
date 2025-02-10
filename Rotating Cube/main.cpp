#include "screen.h"
#include <windows.h>
#include <numeric>
 
#define M_PI 3.14159265358979323846
#define RAND_01 ((rand() / (double)RAND_MAX))

#define SIZE 50

struct vec3{
	float x,y,z;
};

// Calculate centroid
void centroid(vec3& c, std::vector<vec3>& points){
	c = {0,0,0};
	for(auto& p:points){
		c.x += p.x;
		c.y += p.y;
		c.z += p.z;
	}

	float invSize = 1.0f / points.size();
	c.x *= invSize; // Evita Divisão
	c.y *= invSize;
	c.z *= invSize;
}


struct connection{
	int a,b;
};

void translate(vec3& point, float x, float y, float z){
    point.x += x;
    point.y += y;
    point.z += z;
}

void rotate(vec3& point, float x=1, float y=2, float z=1) {
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

void scale(vec3& point, float scaleFactor){
	point.x *= scaleFactor;
	point.y *= scaleFactor;
	point.z *= scaleFactor;
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

void createVertices(std::vector<vec3>& points, float xc=640/2, float yc=480/2){
	int d = (int)SIZE/2;
	points = {
		{xc-d, yc+d, (float)d},
		{xc+d, yc+d, (float)d},
		{xc+d, yc-d, (float)d},
		{xc-d, yc-d, (float)d},

		{xc-d, yc+d, (float)-d},
		{xc+d, yc+d, (float)-d},
		{xc+d, yc-d, (float)-d},
		{xc-d, yc-d, (float)-d},
	};


}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Screen screen;

	vec3 c; //centroid
	std::vector<vec3> points;
	createVertices(points);

	std::vector<connection> connections{
		{0,1},{1,2},{2,3},{3,0},
		{4,5},{5,6},{6,7},{7,4},
		{0,4},{1,5},{2,6},{3,7}
	};

	bool paused = false;
	float speed = 1.5;
	float vx = RAND_01;
	float vy = RAND_01;
	float vz = RAND_01;
	
	int max = 50;
	int count=0;
	while(true){
		if(!paused){
			
			for(auto& p : points){translate(p, vx*speed, vy*speed, 0);}

			centroid(c, points);
			screen.pixel(c.x, c.y);

			for(auto& p : points){
				p.x -= c.x;
				p.y -= c.y;
				p.z -= c.z;
				rotate(p, 0.004, 0.002, 0.008);	
				p.x += c.x;
				p.y += c.y;
				p.z += c.z;
				//scale(p, 1.001f);
				screen.pixel(p.x, p.y);
			}
			if(count>max){
				if( (c.x>(640-3*SIZE)) || (c.x<3*SIZE) || (c.y>(480-3*SIZE)) || (c.y<3*SIZE) ){
					speed = - speed;
					// vx = RAND_01;
					// vy = RAND_01;
					// vz = RAND_01;
				}
				count = 0;
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
			
		}
		count++;
		screen.input(paused);
		SDL_Delay(30);
	}

	return 0;
}
