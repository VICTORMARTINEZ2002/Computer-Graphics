#include "screen.h"
#include <windows.h>
#include <numeric>
 

#define RAND_01 ((rand() / (double)RAND_MAX))

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Screen screen;

	point p0{100,100,0};
	
	point p1{400,400,0};

	point p2{100,350,0};

	float dt=0.0001;
	bool paused = false;
	while(true){
		
		int x,y;
		SDL_GetMouseState(&x, &y);
		p1.x=x;
		p1.y=y;

		screen.pixel(p0.x, p0.y);
		screen.pixel(p1.x, p1.y);
		screen.pixel(p2.x, p2.y);
		for(float t=0; t<=1; t+=dt){
			point p;
			p.x = (1-t)*((1-t)*p0.x + t*p1.x) + t*((1-t)*p1.x + t*p2.x);
			p.y = (1-t)*((1-t)*p0.y + t*p1.y) + t*((1-t)*p1.y + t*p2.y);
			screen.pixel(p.x, p.y);
		}


		screen.show();	
		screen.clear();	
		screen.input(paused);
		SDL_Delay(30);	


	}

	return 0;
}