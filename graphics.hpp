#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
#include <SDL2/SDL.h>
#include "vec3.hpp"
#include "plane.hpp"
#include "constants.hpp"

void fill_circle(SDL_Renderer *renderer, double x, double y, double rad, double epsilon = 1e-3);
void project_point(Vec3 point, Vec3 viewPoint, Plane projectionPlane, double& ret_x, double& ret_y);

class Drawable {
	public:
	int priority;

	Drawable(int p){
		priority = 100;
	}
	virtual void draw([[ maybe_unused ]]SDL_Renderer *renderer){
		return;
	}
	virtual void process_event([[ maybe_unused ]] SDL_Event *ev){
		return;
	}
	virtual void get_rect(SDL_Rect &rect){
		rect.x = 0;
		rect.y = 0;
		rect.w = WINWID;
		rect.h = WINHEI;
	}

};

namespace graphics {
	void init(void);
	void quit(void);
	void register_drawable(Drawable *s);
	void unregister_drawable(Drawable *s);
	SDL_Renderer *renderer(void);
	void main_loop(void);
}

#endif
