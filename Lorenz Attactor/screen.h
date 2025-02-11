#include <SDL2/SDL.h>
#include <vector>

#define PI 3.14159265358979323846f

class Screen{
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

	void show(){
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);

		for(size_t i=0; i<points.size(); i++){
			float t = static_cast<float>(i) / static_cast<float>(points.size() - 1);
			int r = std::sin((t + 0.00f) * 2*PI) * 127 + 128;
			int g = std::sin((t + 0.33f) * 2*PI) * 127 + 128;
    		int b = std::sin((t + 0.66f) * 2*PI) * 127 + 128;
			SDL_SetRenderDrawColor(renderer, r,g,b,255);
			SDL_RenderDrawPointF(renderer, points[i].x, points[i].y);
		}
		SDL_RenderPresent(renderer);
	}

	void clear(){points.clear();}

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