#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
#include <SDL2/SDL.h>
#include "vec3.hpp"
#include "plane.hpp"

void fill_circle(SDL_Renderer *renderer, double x, double y, double rad, double epsilon = 1e-3);
void project_point(Vec3 point, Vec3 viewPoint, Plane projectionPlane, double& ret_x, double& ret_y);

namespace graphics {
	void init();
	void quit();
	SDL_Renderer *renderer();
}

#endif
