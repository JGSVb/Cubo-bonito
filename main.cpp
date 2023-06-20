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

class Background: public Drawable {
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
	Vec3 viewPoint{WINWID/2,WINHEI/2,-700};
	Plane viewPlane{Vec3(0, 0, 0), Vec3(0, 0, 1)};
	Cube cb{200, Vec3(WINWID/2 - 100, WINHEI/2 - 100, 500), viewPoint, viewPlane};
	cb.make_animated(0.01);

	Slider s1{graphics::renderer(), "Ponto X"};
	s1.set_boundries(-1000, +1000);
	s1.set_default(viewPoint.e[0]);
	s1.set_callback([&cb](double x){
		cb.get_view_point_pointer()->e[0] = x;
	});
	
	Slider s2{graphics::renderer(), "Ponto Y"};
	s2.set_boundries(-1000, +1000);
	s2.set_default(viewPoint.e[1]);
	s2.position_related_to(&s1, POSITION_BELOW);
	s2.set_callback([&cb](double y){
		cb.get_view_point_pointer()->e[1] = y;
	});

	Slider s3{graphics::renderer(), "Ponto Z"};
	s3.set_boundries(-1000, +1000);
	s3.set_default(viewPoint.e[2]);
	s3.position_related_to(&s2, POSITION_BELOW);
	s3.set_callback([&cb](double z){
		cb.get_view_point_pointer()->e[2] = z;
	});

	Slider s4{graphics::renderer(), "Plano Z"};
	s4.set_boundries(-1000, +1000);
	s4.set_default(viewPlane.p0.e[2]);
	s4.position_related_to(&s3, POSITION_RIGHT);
	s4.set_callback([&cb](double z){
		cb.get_view_plane_pointer()->p0.e[2] = z;
	});

	Slider s5{graphics::renderer(), "Rotacao Cubo"};
	s5.set_default(0);
	s5.set_boundries(-DOUBLEPI, +DOUBLEPI);
	s5.set_increment(0.1);
	s5.position_related_to(&s2, POSITION_RIGHT);
	s5.set_callback([&cb](double rad){
		cb.set_rotation(rad);
	});

	
	Background bg{255,255,255};

	graphics::register_drawable(&s1);
	graphics::register_drawable(&s2);
	graphics::register_drawable(&s3);
	graphics::register_drawable(&s4);
	graphics::register_drawable(&s5);
	graphics::register_drawable(&bg);
	graphics::register_drawable(&cb);

	graphics::main_loop();

	s1.dispose();
	
	Slider::finish();
	graphics::quit();

	return 0;
}
