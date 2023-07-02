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

class Triangle: public Drawable {
	private:
	struct {
		int pA[2];
		int pB[2];
		int pC[2];
	} triPoints;
	public:
	Triangle(int x1, int y1, int x2, int y2, int x3, int y3) : Drawable{-1000}, triPoints{{x1, y1}, {x2, y2}, {x3, y3}} {}
	void set_point_a(int x1, int y1){
		triPoints.pA[0] = x1;
		triPoints.pA[1] = y1;
	}
	void set_point_b(int x1, int y1){
		triPoints.pB[0] = x1;
		triPoints.pB[1] = y1;
	}
	void set_point_c(int x1, int y1){
		triPoints.pC[0] = x1;
		triPoints.pC[1] = y1;
	}

	void draw(SDL_Renderer *renderer){
		SDL_SetRenderDrawColor(renderer, 0,255,0,255);
		fill_triangle(renderer, triPoints.pA[0], triPoints.pA[1],
		triPoints.pB[0], triPoints.pB[1],
		triPoints.pC[0], triPoints.pC[1]);
		SDL_SetRenderDrawColor(renderer, 255,0,0,255);
		fill_circle(renderer, triPoints.pA[0], triPoints.pA[1], 5);
		fill_circle(renderer, triPoints.pB[0], triPoints.pB[1], 5);
		fill_circle(renderer, triPoints.pC[0], triPoints.pC[1], 5);

		SDL_RenderDrawLine(renderer,triPoints.pA[0], triPoints.pA[1], triPoints.pB[0], triPoints.pB[1]);
		SDL_RenderDrawLine(renderer,triPoints.pB[0], triPoints.pB[1], triPoints.pC[0], triPoints.pC[1]);
		SDL_RenderDrawLine(renderer,triPoints.pC[0], triPoints.pC[1], triPoints.pA[0], triPoints.pA[1]);
	}
	
};

int main([[ maybe_unused ]] int argc, [[ maybe_unused ]] char **argv){

	graphics::init();
	Slider::init();

	Vec3 camPos{0, 0, -1};
	Vec3 camDir{0,0,1};
	graphics3d::Camera cam{camPos, camDir, 6, WINWID, WINHEI};

	Vec3 cubePos{.5,.2,100};
	struct Vec3Rot cubeRot = vec3rot_from_degrees(120, 40, 29);
	graphics3d::Cube cube{cubePos, 2, cubeRot};

	cam.add_object(&cube);

	Triangle test{100, 120, 210, 130, 320, 510};
	test.set_point_a(100, 120);
	test.set_point_c(210, 130);
	test.set_point_b(320, 510);

	Slider camSliderX{graphics::renderer(), "Camera X"};
	camSliderX.set_boundries(-5, +5);
	camSliderX.set_increment(0.5);
	camSliderX.set_callback([&cam](double val){
		double x,y,z;
		cam.get_camera_position(x,y,z);
		x = val;
		cam.set_camera_position(x,y,z);
	});
	camSliderX.set_default(camPos.x());

	Slider camSliderY{graphics::renderer(), "Camera Y"};
	camSliderY.set_boundries(-5, +5);
	camSliderY.set_increment(0.5);
	camSliderY.set_callback([&cam](double val){
		double x,y,z;
		cam.get_camera_position(x,y,z);
		y = val;
		cam.set_camera_position(x,y,z);
	});
	camSliderY.set_default(camPos.y());


	Slider camSliderLength{graphics::renderer(), "Camera Length"};
	camSliderLength.set_boundries(-100, +100);
	camSliderLength.set_increment(1);
	camSliderLength.set_callback([&cam](double val){
		cam.cameraLength = val;
	});
	camSliderLength.set_default(cam.cameraLength);

	camSliderY.set_position_related_to(&camSliderX, POSITION_BELOW);
	camSliderLength.set_position_related_to(&camSliderY, POSITION_BELOW);

	Slider cubeSliderZ{graphics::renderer(), "Cube Z"};
	cubeSliderZ.set_boundries(-5, +5);
	cubeSliderZ.set_increment(0.1);
	cubeSliderZ.set_callback([&cube](double val){
		double x,y,z;
		cube.get_position(x,y,z);
		z = val;
		cube.set_position(x,y,z);
	});
	cubeSliderZ.set_position_related_to(&camSliderY, POSITION_RIGHT);

	Slider cubeSliderPitch{graphics::renderer(), "Cubo pitch"};
	cubeSliderPitch.set_boundries(-360, +360);
	cubeSliderPitch.set_increment(5);
	cubeSliderPitch.set_callback([&cube](double val){
		struct Vec3Rot rot;
		cube.get_rotation(rot);
		rot.pitch = rad_from_degrees(val);
		cube.set_rotation(rot);
	});
	cubeSliderPitch.set_position_related_to(&camSliderLength, POSITION_RIGHT);

	
	Background bg{255,255,255};

	graphics::register_drawable(&bg);
	graphics::register_drawable(&camSliderX);
	graphics::register_drawable(&camSliderY);
	graphics::register_drawable(&camSliderLength);
	graphics::register_drawable(&cubeSliderPitch);
	graphics::register_drawable(&cubeSliderZ);
	graphics::register_drawable(&cam);
	graphics::register_drawable(&test);

	graphics::main_loop();

	camSliderY.dispose();
	
	Slider::finish();
	graphics::quit();

	return 0;
}
