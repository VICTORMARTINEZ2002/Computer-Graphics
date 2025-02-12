#include <SDL2/SDL.h>
#include <vector>

#define PI 3.14159265358979323846f

struct point{
	float x,y,z;
};


class Screen{
public:
	SDL_Event e;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::vector<SDL_FPoint> points;

public:
	Screen(){
		SDL_Init(SDL_INIT_VIDEO);
		SDL_CreateWindowAndRenderer(640*2, 480*2, 0, &window, &renderer);
		SDL_RenderSetScale(renderer,2,2);
	}

	void pixel(float x, float y){points.emplace_back(x,y);}

	void cpixel(float x, float y){
		SDL_RenderDrawPointF(renderer, x, y);
	}

	void show(){
		SDL_SetRenderDrawColor(renderer, 255,255,255,255);
		for(size_t i=0; i<points.size(); i++){
			SDL_RenderDrawPointF(renderer, points[i].x, points[i].y);	
		}
		SDL_RenderPresent(renderer);
	}

	void clear(){
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		points.clear();
	}

	int input(bool& pause){
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_QUIT){
				SDL_Quit();
				exit(0);
			}
			if(e.type==SDL_KEYDOWN){
				if(e.key.keysym.sym==SDLK_SPACE){pause=!pause;}
			}
		}
		return pause;
	}

};