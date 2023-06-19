#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
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
	} G_app;

	static void switch_lock();

	static void switch_lock(){
		G_app.quitLock = G_app.initLock;
		G_app.initLock = !G_app.initLock;
	}

	void init(){

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

	void quit(){
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

	SDL_Renderer *renderer(){
		return G_app.renderer;
	}
}
