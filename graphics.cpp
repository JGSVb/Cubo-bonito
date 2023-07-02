#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <algorithm>
#include "line.hpp"
#include "plane.hpp"
#include "graphics.hpp"
#include "constants.hpp"

#define SDL_ERR_EXIT(label) do { fprintf(stderr, "%s: %s\n", label, SDL_GetError()); exit(-1); } while(0)

void fill_circle(SDL_Renderer *renderer, int x, int y, double rad, double epsilon){
	for(double i = 0; i < HALFPI; i+=epsilon){
		double adj = std::cos(i) * rad;
		double opp = std::sin(i) * rad;
		for(int i = 0; i < adj; i++){
			SDL_RenderDrawPoint(renderer, x + i, y + opp);
			SDL_RenderDrawPoint(renderer, x - i, y + opp);
			SDL_RenderDrawPoint(renderer, x + i, y - opp);
			SDL_RenderDrawPoint(renderer, x - i, y - opp);
		}
	}
	
}

static double det_2(double x, double y, double x1, double y1){
	return x*y1 - x1*y;
}

static bool point_in_triangle(double x, double y, double tx1, double ty1, double tx2, double ty2, double tx3, double ty3){
	double det1 = det_2(tx2-tx1, ty2-ty1, x-tx1, y-ty1);
	double det2 = det_2(tx3-tx2, ty3-ty2, x-tx2, y-ty2);
	double det3 = det_2(tx1-tx3, ty1-ty3, x-tx3, y-ty3);
	return (det1 >= 0 && det2 >= 0 && det3 >= 0) || (det1 <= 0 && det2 <= 0 && det3 <= 0);
}

void fill_triangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3){
	int xMax, xMin, yMax, yMin;

	xMax = std::max(x1, x2);
	xMax = std::max(xMax, x3);

	xMin = std::min(x1, x2);
	xMin = std::min(xMin, x3);

	yMax = std::max(y1, y2);
	yMax = std::max(yMax, y3);

	yMin = std::min(y1, y2);
	yMin = std::min(yMin, y3);

	for(int j = yMin; j <= yMax; j++){
		for(int i = xMin; i <= xMax; i++){
			if(point_in_triangle(i,j,x1,y1,x2,y2,x3,y3)){
				SDL_RenderDrawPoint(renderer, i,j);
			}
		}
	}

}

namespace graphics {
	static struct {
		bool initLock = false;
		bool quitLock = true;
		SDL_Window *window;
		SDL_Renderer *renderer;
		std::vector<Drawable*> registeredDrawables {}; 
	} G_app;

	static void switch_lock(void){
		G_app.quitLock = G_app.initLock;
		G_app.initLock = !G_app.initLock;
	}

	void init(void){

		if(G_app.initLock){
			fputs("Impossível iniciar os gráficos quando estes já se encontram iniciados.\n", stderr);
			exit(-1);
			return;
		}

		switch_lock();

		if(SDL_Init(SDL_INIT_VIDEO) != 0){
			SDL_ERR_EXIT("Impossível iniciar o vídeo SDL");
		}

		if(TTF_Init() != 0){
			SDL_ERR_EXIT("Impossível iniciar o texto SDL");
		}

		G_app.window = SDL_CreateWindow(WINTITLE,
					 SDL_WINDOWPOS_CENTERED,
					 SDL_WINDOWPOS_CENTERED,
					 WINWID,
					 WINHEI,
					 0);

		if(G_app.window == NULL){
			SDL_ERR_EXIT("Impossível criar janela");
		}

		G_app.renderer = SDL_CreateRenderer(G_app.window, -1, 0);

		if(G_app.renderer == NULL){
			SDL_ERR_EXIT("Impossível criar renderizador");
		}

	}

	void quit(void){
		if(G_app.quitLock){
			fputs("Impossível finalizar os gráficos quando estes não se encontram sequer iniciados.\n", stderr);
			exit(-1);
			return;
		}

		switch_lock();

		SDL_DestroyRenderer(G_app.renderer);
		SDL_DestroyWindow(G_app.window);
		SDL_Quit();
	}

	SDL_Renderer *renderer(void){
		return G_app.renderer;
	}

	static bool sort_func(Drawable *s1, Drawable *s2){
		return s1->get_priority() > s2->get_priority();
	}

	void register_drawable(Drawable *s){
		G_app.registeredDrawables.push_back(s);
		std::sort(G_app.registeredDrawables.begin(), G_app.registeredDrawables.end(), sort_func);
	}
	void unregister_drawable(Drawable *s){
		std::vector<Drawable*>::iterator it;
		for(it = G_app.registeredDrawables.begin(); it != G_app.registeredDrawables.end(); it++){
			if(*it == s){
				G_app.registeredDrawables.erase(it);
				return;
			}
		}

		fprintf(stderr, "Impossível anular registro de `Drawable *s` (%p) não encontrado.\n", (void *)s);
		exit(-1);
	}

	static bool compute_events(void){
		SDL_Event ev;
		
		SDL_Point point;
		SDL_GetMouseState(&point.x, &point.y);

		while(SDL_PollEvent(&ev)){
			Drawable *sLowestPriority {NULL};

			std::vector<Drawable *>::iterator it;

			for(it = G_app.registeredDrawables.begin(); it!= G_app.registeredDrawables.end(); it++){
				SDL_Rect rect;
				Drawable *s = *it;
				s->get_rect(rect);
				if(SDL_PointInRect(&point, &rect)){
					if(!sLowestPriority || sLowestPriority->get_priority() > s->get_priority()){
						sLowestPriority = s;
					}
				}
			}

			if(sLowestPriority){
				sLowestPriority->process_event(&ev);
			}
			
			switch(ev.type){
				case SDL_QUIT:
					return false;
					break;
				default:
					break;
			}
		}
		return true;
	}

	static inline long int get_time_in_milliseconds(void){
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
		return time.count();
	}

	static void timer(double &framerate, long int &cycleDelay){
		static long int frame { 0 };
		static long int timeStart = get_time_in_milliseconds();
		static long int lastTime = timeStart;
		frame++;

		long int timeNow = get_time_in_milliseconds();
		long int timeDiff = timeNow-timeStart;
		
		framerate = (double)frame/timeDiff*1000;
		cycleDelay = timeNow-lastTime;

		lastTime = timeNow;
	}

	void main_loop(void){

		std::vector<Drawable *>::iterator it;
		int frame { 0 };
		double fps;
		long int cycleDelay, cycleExtraTime, tStart;
		while(true){
			tStart = get_time_in_milliseconds();
			if(!compute_events()){
				break;
			}

			it = G_app.registeredDrawables.begin();
			while(it != G_app.registeredDrawables.end()){
				(*it)->draw(G_app.renderer);
				it++;
			}

			timer(fps, cycleDelay);
			if((frame++) >= (1.5*fps)){
				printf("Quadros por segundo:   %f\n", fps);
				printf("Duração de cada diclo: %ld\n", cycleDelay);
				frame = 0;
			}
			
			SDL_RenderPresent(G_app.renderer);
			cycleExtraTime = get_time_in_milliseconds() - tStart;
			SDL_Delay(std::max((long)0, LOOPDELAY-cycleExtraTime));
			
		}
	}
}

