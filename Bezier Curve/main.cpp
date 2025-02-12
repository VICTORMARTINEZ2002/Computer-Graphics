#include "screen.h"
#include <windows.h>
#include <numeric>
#include <cmath>
#include <iostream> // Para std::cout

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

void Cline(Screen& screen, float x1, float y1, float x2, float y2, int r, int g, int b, int alfa){
    float dx = x2-x1;
    float dy = y2-y1;

    float length = std::sqrt(dx*dx + dy*dy);
    float angle  = std::atan2(dy,dx);

    SDL_SetRenderDrawColor(screen.renderer, r,g,b,alfa);
    for(int i=0; i<length; i++){
        screen.cpixel(
            x1 + std::cos(angle)*i,
            y1 + std::sin(angle)*i
            // float t = static_cast<float>(i) / static_cast<float>(points.size() - 1);
            // int r = std::sin((t + 0.00f) * 2*PI) * 127 + 128;
            // int g = std::sin((t + 0.33f) * 2*PI) * 127 + 128;
            // int b = std::sin((t + 0.66f) * 2*PI) * 127 + 128;
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
    return false; 
}

constexpr long long binomial_coefficient(int n, int k) {
    if (k>n||k<0) return 0;
    if (k>n-k) k = n - k; // Usa a simetria C(n, k) = C(n, n-k)

    long long resultado = 1;
    for(int i=1; i<=k; i++){
        resultado = resultado * (n-i+1)/i;
    }
    return resultado;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Screen screen;

    // Verifique se a tela foi inicializada corretamente

    std::vector<point> points;
    points.push_back(point{100,480/2,0});
    points.push_back(point{400,400,0});
    points.push_back(point{300,100,0});
    points.push_back(point{540,480/2,0});

    float dt=0.01;
    bool paused = false;
    while(true){
        int x,y;
        SDL_GetMouseState(&x, &y);
        points[1].x=x;
        points[1].y=y;

        // Curva Generica
        int n = points.size();
        std::vector<point> bezier;

        for(float t=0; t<=1; t+=dt){
            point p{0, 0, 0};
            for(size_t i=0; i<n; i++){ // Fix: Change i <= n to i < n
                float aux = binomial_coefficient(n-1, i)*std::pow(1-t, n-1-i)*std::pow(t, i);
                p.x += aux * points[i].x;
                p.y += aux * points[i].y;
                p.z += aux * points[i].z;
            }
            bezier.push_back(p);
        }


        // Impressão
    // Draw intermediate points for cubic Bézier construction
    if(points.size()==4){ // Only for cubic Bézier curves
        point Q0, Q1, Q2, R0, R1;

        for(float t = 0; t <= 1.0001; t += 5*dt) {
            // Linear interpolation for Q0, Q1, Q2
            Q0.x = points[0].x + t * (points[1].x - points[0].x);
            Q0.y = points[0].y + t * (points[1].y - points[0].y);

            Q1.x = points[1].x + t * (points[2].x - points[1].x);
            Q1.y = points[1].y + t * (points[2].y - points[1].y);

            Q2.x = points[2].x + t * (points[3].x - points[2].x);
            Q2.y = points[2].y + t * (points[3].y - points[2].y);

            // Quadratic interpolation for R0, R1
            R0.x = Q0.x + t * (Q1.x - Q0.x);
            R0.y = Q0.y + t * (Q1.y - Q0.y);

            R1.x = Q1.x + t * (Q2.x - Q1.x);
            R1.y = Q1.y + t * (Q2.y - Q1.y);

            // Draw intermediate lines
            Cline(screen, Q0.x, Q0.y, Q1.x, Q1.y, 255,   0,   0, 255);
            Cline(screen, Q1.x, Q1.y, Q2.x, Q2.y,   0, 255,   0, 255);
            Cline(screen, R0.x, R0.y, R1.x, R1.y,   0,   0, 255, 255);
        }
    }

        for(size_t i=0; i<points.size(); i++) {
            screen.pixel(points[i].x, points[i].y);
        }

        for(auto& p : bezier){screen.pixel(p.x, p.y);}  

        screen.show();  
        screen.clear(); 
        screen.input(paused);
        SDL_Delay(30);  
    }

    return 0;
}