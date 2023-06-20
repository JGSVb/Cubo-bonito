#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <vector>
#include <chrono>
#include "line.hpp"
#include "plane.hpp"
#include "graphics.hpp"
#include "constants.hpp"

#define SDL_ERR_EXIT(label) do { fprintf(stderr, "%s: %s\n", label, SDL_GetError()); exit(-1); } while(0)

void fill_circle(SDL_Renderer *renderer, double x, double y, double rad, double epsilon){
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

void project_point(Vec3 point, Vec3 viewPoint, Plane projectionPlane, double& ret_x, double& ret_y){
	Line l = Line(viewPoint, point-viewPoint);
	Vec3 ip = l.intersect(projectionPlane);
	ret_x = ip.x();
	ret_y = ip.y();
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

	void register_drawable(Drawable *s){
		std::vector<Drawable*>::iterator it;
		for(it = G_app.registeredDrawables.begin(); it != G_app.registeredDrawables.end(); it++){
			if((*it)->get_priority() <= s->get_priority()){
				G_app.registeredDrawables.insert(it, s);
				return; // sai da função, para que possa, posterior-
					// mente, ser adicionado, conforme escrito
					// abaixo \/ 
			}
		}
		G_app.registeredDrawables.insert(G_app.registeredDrawables.begin(), s);
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

	static double count_fps(void){
		static long int frame { 0 };
		static long int timeStart = get_time_in_milliseconds();
		frame++;

		long int timeNow = get_time_in_milliseconds();
		long int timeDiff = timeNow-timeStart;
		
		return (double)frame/timeDiff*1000;
	}

	void main_loop(void){

		std::vector<Drawable *>::iterator it;
		int frame { 0 };
		double fps;
		while(compute_events()){
			SDL_SetRenderDrawColor(G_app.renderer, 0,0,0,255);
			SDL_RenderClear(G_app.renderer);

			it = G_app.registeredDrawables.begin();
			while(it != G_app.registeredDrawables.end()){
				(*it)->draw(G_app.renderer);
				it++;
			}
			SDL_RenderPresent(G_app.renderer);
			SDL_Delay(LOOPDELAY);

			fps = count_fps();
			if((frame++) >= 80){
				printf("FPS: %f\n", fps);
				frame = 0;
			}
			
		}
	}
}

