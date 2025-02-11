#include "screen.h"
#include <windows.h>
#include <numeric>
 

#define RAND_01 ((rand() / (double)RAND_MAX))

// Lorenz Parameters (always positive)
#define A 10
#define B 28
#define C 8.0/3.0

struct point{
	float x,y,z;
};


void translate(point& p, float x, float y, float z){
    p.x += x;
    p.y += y;
    p.z += z;
}

void rotate(point& p, float x=1, float y=2, float z=1) {
    float rad;
    float temp_x, temp_y, temp_z;

    // Rotation around X-axis
    rad = x;
    temp_y = std::cos(rad) * p.y - std::sin(rad) * p.z;
    temp_z = std::sin(rad) * p.y + std::cos(rad) * p.z;
    p.y = temp_y;
    p.z = temp_z;

    // Rotation around Y-axis
    rad = y;
    temp_x = std::cos(rad) * p.x + std::sin(rad) * p.z;
    temp_z = -std::sin(rad) * p.x + std::cos(rad) * p.z;
    p.x = temp_x;
    p.z = temp_z;

    // Rotation around Z-axis
    rad = z;
    temp_x = std::cos(rad) * p.x - std::sin(rad) * p.y;
    temp_y = std::sin(rad) * p.x + std::cos(rad) * p.y;
    p.x = temp_x;
    p.y = temp_y;
}

void scale(point& p, float scaleFactor){
	p.x *= scaleFactor;
	p.y *= scaleFactor;
	p.z *= scaleFactor;
}

void centroid(point& c, std::vector<point>& points){
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


void nextPos(float& x, float& y, float& z){
	float dt = 0.01;
	float dx = (A * (y-x))*dt;
	float dy = (x*(B-z)-y)*dt;
	float dz = (x*y - C*z)*dt;
	x += dx;
	y += dy;
	z += dz;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Screen screen;

	point c; //centroid
	std::vector<point> points;

	float x=0.01;
	float y=0;
	float z=0;
	points.emplace_back(point{x, y, z});

	float angleY=1;
	bool paused = true;
	while(true){
		if(!paused){
			centroid(c, points);
			screen.pixel(c.x, c.y);

			nextPos(x,y,z);
			printf("%lf %lf %lf\n", x,y,z);

			points.emplace_back(point{x,y,z});
			std::vector<point> output = points; 
			for(size_t i=0; i<output.size(); i++){
				output[i].x -= c.x;
				output[i].y -= c.y;
				output[i].z -= c.z;
				rotate(output[i], 0.0, angleY, 0.0);	
				output[i].x += c.x;
				output[i].y += c.y;
				output[i].z += c.z;
				screen.pixel(3*output[i].x+320, 3*output[i].y+240);
			}
			angleY += 0.005;
			
			screen.show();	
			screen.clear();	
		}

		screen.input(paused);
		SDL_Delay(7);	


	}

	return 0;
}