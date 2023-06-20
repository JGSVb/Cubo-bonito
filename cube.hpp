#ifndef CUBE_HPP
#define CUBE_HPP
#include <SDL2/SDL.h>
#include "vec3.hpp"
#include "graphics.hpp"

class Cube: virtual public Drawable {
	private:
	double cubeSize;
	double cubeRotation;
	Vec3 cubePosition;
	Vec3 bottomSquare[4];
	Vec3 topSquare[4];
	double projBottom[4][2];
	double projTop[4][2];
	Vec3 viewPoint;
	Plane viewPlane;

	double rotationSpeed;
	double rotationAccumulation;
	bool isAnimated;

	void apply_rotation(void);
	void build(void);
	void project(void);

	public:
	Cube(double size, Vec3 position, Vec3 vPoint, Plane vPlane): Drawable {0}{
		cubeSize = size;
		cubePosition = position;
		cubeRotation = 0;
		viewPoint = vPoint;
		viewPlane = vPlane;

		isAnimated = false;
		rotationSpeed = 0;
		rotationAccumulation = 0;
		build();
	}

	void set_cube_size(double s){
		cubeSize = s;
		build();
	}

	double get_cube_size(void){
		return cubeSize;
	}
	void set_cube_position(Vec3 p){
		cubePosition = p;
		build();
	}
	Vec3 get_cube_position(){
		return cubePosition;
	}
	double get_rotation(){
		return cubeRotation;
	}
	void set_rotation(double r){
		cubeRotation = r;
		build();
	}

	Vec3 *get_view_point_pointer(void){
		return &viewPoint;
	}
	Plane *get_view_plane_pointer(void){
		return &viewPlane;
	}

	void make_animated(double speed){
		isAnimated = true;
		rotationSpeed = speed;
	}
	void make_unanimated(void){
		isAnimated = false;
		rotationAccumulation = 0;
	}

	void draw(SDL_Renderer *renderer);
};

#endif // CUBE_HPP
