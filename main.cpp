#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdexcept>
#include <array>
#include "vec3.hpp"
#include "plane.hpp"
#include "cube.hpp"
#include "constants.hpp"
#include "slider.hpp"

class Background: virtual public Drawable {
	public:
	SDL_Color col;
	Background(int r, int g, int b, int a = 255): Drawable{1000} {
		col.r = r;
		col.g = g;
		col.b = b;
		col.a = a;
	};
	void draw(SDL_Renderer *renderer){
		SDL_Rect rect;
		get_rect(rect);
		SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
		SDL_RenderFillRect(renderer, &rect);
	}
};

int main([[ maybe_unused ]] int argc, [[ maybe_unused ]] char **argv){

	graphics::init();
	Slider::init();

	Vec3 viewPoint{WINWID/2,WINHEI/2,-100};
	Plane viewPlane{Vec3(0, 0, -400), Vec3(0, 0, 1)};
	Cube cb{200, Vec3(WINWID/2 - 100, WINHEI/2 - 100, 500), viewPoint, viewPlane};

	Slider s1{graphics::renderer(), "Ponto Z"};
	s1.set_boundries(-1000, +1000);
	s1.set_default(-100);
	s1.link(&(cb.get_view_point_pointer()->e[2]));
	
	Background bg{255,255,255};

	graphics::register_drawable(&s1);
	graphics::register_drawable(&bg);
	graphics::register_drawable(&cb);

	graphics::main_loop();

	s1.dispose();
	
	Slider::finish();
	graphics::quit();

	return 0;
}
