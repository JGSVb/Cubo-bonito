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

/*
void main_loop(){

	Vec3 viewPoint{WINWID/2,WINHEI/2,-100};
	Plane viewPlane{Vec3(0, 0, -400), Vec3(0, 0, 1)};

	Cube cube{200, Vec3(WINWID/2 - 100, WINHEI/2 - 100, 500)};

	Vec3 t{400,0,400};
	Vec3 a{500,0,200};

	while(graphics::compute_events()){
		SDL_SetRenderDrawColor(graphics::renderer(), 255,255,255,255);
		SDL_RenderClear(graphics::renderer());

		SDL_SetRenderDrawColor(graphics::renderer(), 255,0,0,255);
		cube.draw(graphics::renderer(), viewPoint, viewPlane);
		cube.set_rotation(cube.rotation() + 0.01);

		SDL_RenderPresent(graphics::renderer());
		SDL_Delay(LOOPDELAY);
	};
}
*/

int main([[ maybe_unused ]] int argc, [[ maybe_unused ]] char **argv){

	graphics::init();
	Slider::init();

	Slider s1{graphics::renderer(), "Ponto X"};
	graphics::register_drawable((Drawable *)&s1);

	graphics::main_loop();

	s1.dispose();
	
	graphics::unregister_drawable((Drawable *)&s1);
	Slider::finish();
	graphics::quit();

	return 0;
}
