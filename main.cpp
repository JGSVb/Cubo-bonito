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
#include "graphics3d.hpp"

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

/*
class Triangle: public Drawable {
	private:
	struct {
		int pA[2];
		int pB[2];
		int pC[2];
	} triPoints;
	public:
	Triangle(int x1, int y1, int x2, int y2, int x3, int y3) : Drawable{100}, triPoints{{x1, y1}, {x2, y2}, {x3, y3}} {}
	void set_point_a(int x1, int y1){
		triPoints.pA[0] = x1;
		triPoints.pA[1] = y1;
	}
	void set_point_b(int x1, int y1){
		triPoints.pC[0] = x1;
		triPoints.pC[1] = y1;
	}
	void set_point_c(int x1, int y1){
		triPoints.pC[0] = x1;
		triPoints.pC[1] = y1;
	}

	void get_rect(SDL_Rect &r){
		int minX = std::numeric_limits<int>::max();
		int minY = std::numeric_limits<int>::max();
		int maxX = std::numeric_limits<int>::lowest();
		int maxY = std::numeric_limits<int>::lowest();

		// continuar
		
		r.x = minX;
		r.y = minY;
		r.w = maxX-minX;
		r.h = maxY-minY;

	}

	void draw(SDL_Renderer *renderer){

	}
	
};
*/

int main([[ maybe_unused ]] int argc, [[ maybe_unused ]] char **argv){

	graphics::init();
	Slider::init();

	Slider s1{graphics::renderer(), "Camera X"};
	s1.set_boundries(-1000, +1000);

	Vec3 camPos{0, 0, -1};
	Vec3 camDir{0,0,1};
	graphics3d::Camera cam{camPos, camDir, 6, WINWID, WINHEI};

	Vec3 cubePos{.5,.2,100};
	graphics3d::Cube cube{cubePos, 2, 0.2};

	cam.add_object(&cube);

	// Triangle test{10, 10, 30, 50, 70, 110};


	
	Background bg{255,255,255};

	graphics::register_drawable(&bg);
	graphics::register_drawable(&s1);
	graphics::register_drawable(&cam);
	// graphics::register_drawable(&test);

	graphics::main_loop();

	s1.dispose();
	
	Slider::finish();
	graphics::quit();

	return 0;
}
