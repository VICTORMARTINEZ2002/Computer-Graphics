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

bool intersectLines(float x1, float y1, float x2, float y2, // Line 1
                    float x3, float y3, float x4, float y4, // Line 2
                    float& ix, float& iy) {                // Intersection point
    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (denom == 0) return false; // Lines are parallel or coincident

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        ix = x1 + t * (x2 - x1);
        iy = y1 + t * (y2 - y1);
        return true;
    }
    return false; // No intersection within the line segments
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Screen screen;

	point p0{100,100,0};
	point p1{400,400,0};
	point p2{100,350,0};

	float dt=0.01;
	bool paused = false;
	while(true){
		
		int x,y;
		SDL_GetMouseState(&x, &y);
		p1.x=x;
		p1.y=y;

		screen.pixel(p0.x, p0.y);
		screen.pixel(p1.x, p1.y);
		screen.pixel(p2.x, p2.y);


		// Curva
		std::vector<point> points;
		for(float t=0; t<=1; t+=dt){
			point p;
			p.x = (1-t)*((1-t)*p0.x + t*p1.x) + t*((1-t)*p1.x + t*p2.x);
			p.y = (1-t)*((1-t)*p0.y + t*p1.y) + t*((1-t)*p1.y + t*p2.y);
			points.push_back(p);
			screen.pixel(p.x, p.y);
		}

		// Tangents
        for (float t = 0; t <= 1; t += 5*dt) {
            // Calculate the point on the curve at parameter t
            float curve_x = (1 - t) * ((1 - t) * p0.x + t * p1.x) + t * ((1 - t) * p1.x + t * p2.x);
            float curve_y = (1 - t) * ((1 - t) * p0.y + t * p1.y) + t * ((1 - t) * p1.y + t * p2.y);

            // Calculate the tangent vector at parameter t
            float tangent_x = 2 * (1 - t) * (p1.x - p0.x) + 2 * t * (p2.x - p1.x);
            float tangent_y = 2 * (1 - t) * (p1.y - p0.y) + 2 * t * (p2.y - p1.y);

            // Normalize the tangent vector (optional, for consistent length)
            float length = std::sqrt(tangent_x * tangent_x + tangent_y * tangent_y);
            if (length > 0) {
                tangent_x /= length;
                tangent_y /= length;
            }

            // Define the tangent line segment (extend it far enough to intersect)
            float tangent_end_x1 = curve_x + tangent_x * 1000; // Extend tangent line
            float tangent_end_y1 = curve_y + tangent_y * 1000;
            // Find intersection with Line 1 (p0 to p1)
            float ix1, iy1;
            bool intersectsLine1 = intersectLines(curve_x, curve_y, tangent_end_x1, tangent_end_y1,
                                                  p1.x, p1.y, p2.x, p2.y, ix1, iy1);

            
            float tangent_end_x2 = curve_x - tangent_x * 1000; // Extend tangent line
            float tangent_end_y2 = curve_y - tangent_y * 1000;
            float ix2, iy2;
            bool intersectsLine2 = intersectLines(curve_x, curve_y, tangent_end_x2, tangent_end_y2,
                                                 p0.x, p0.y, p1.x, p1.y, ix2, iy2);

            // Draw the tangent line to the closest intersection
            if (intersectsLine1 || intersectsLine2) {
                if (intersectsLine1 && intersectsLine2) {
                    // Choose the closest intersection
                    float dist1 = std::hypot(ix1 - curve_x, iy1 - curve_y);
                    float dist2 = std::hypot(ix2 - curve_x, iy2 - curve_y);
                    //if (dist1 < dist2) {
                        line(screen, curve_x, curve_y, ix1, iy1);
                    //} else {
                        line(screen, curve_x, curve_y, ix2, iy2);
                    //}
                } else if (intersectsLine1) {
                    line(screen, curve_x, curve_y, ix1, iy1);
                } else if (intersectsLine2) {
                    line(screen, curve_x, curve_y, ix2, iy2);
                }
            }
        }


	

		line(screen, p0.x, p0.y, p1.x, p1.y);
		line(screen, p1.x, p1.y, p2.x, p2.y);


		screen.show();	
		screen.clear();	
		screen.input(paused);
		SDL_Delay(30);	


	}

	return 0;
}