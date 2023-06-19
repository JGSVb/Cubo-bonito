#ifndef CUBE_HPP
#define CUBE_HPP
#include <SDL2/SDL.h>
#include "vec3.hpp"
#include "graphics.hpp"

class Cube {
	private:
	double siz;
	double rot;
	Vec3 pos;
	Vec3 bottomSquare[4];
	Vec3 topSquare[4];
	double projBottom[4][2];
	double projTop[4][2];
	private:
	void apply_rotation(){
		Vec3 bottomAxis = (bottomSquare[0] + bottomSquare[2])/2;
		Vec3 topAxis = (topSquare[0] + topSquare[2])/2;

		for(int i = 0; i < 4; i++){
			bottomSquare[i] = vec3_rotate(bottomSquare[i], rot, bottomAxis);
			topSquare[i] = vec3_rotate(topSquare[i], rot, topAxis);
			
		}
	}
	void build(){
		topSquare[0] = pos;
		topSquare[1] = pos + Vec3(1, 0, 0)*siz;
		topSquare[2] = pos + Vec3(1, 0, 1)*siz;
		topSquare[3] = pos + Vec3(0, 0, 1)*siz;

		Vec3 verticalTranslation = Vec3(0, 1, 0)*siz;

		for(int i = 0; i < 4; i++){
			bottomSquare[i] = topSquare[i] + verticalTranslation;
		}

		apply_rotation();

	}

	void project(Vec3 viewPoint, Plane viewPlane){
		for(int i = 0; i < 4; i++){
			double x1,y1;
			double x2,y2;

			project_point(topSquare[i], viewPoint, viewPlane, x1, y1);
			projTop[i][0] = x1;
			projTop[i][1] = y1;

			project_point(bottomSquare[i], viewPoint, viewPlane, x2, y2);
			projBottom[i][0] = x2;
			projBottom[i][1] = y2;
		}

	}

	public:
	Cube(){
		siz = 0;
		pos = Vec3();
		rot = 0;
		build();
	}
	Cube(double size, Vec3 position){
		siz = size;
		pos = position;
		rot = 0;
		build();
	}

	void set_size(double s){
		siz = s;
		build();
	}

	double size(){
		return siz;
	}
	void set_position(Vec3 p){
		pos = p;
		build();
	}
	Vec3 position(){
		return pos;
	}
	double rotation(){
		return rot;
	}
	void set_rotation(double r){
		rot = r;
		build();
	}
	void draw(SDL_Renderer *renderer, Vec3 viewPoint, Plane viewPlane){
		project(viewPoint, viewPlane);
	
		for(int i = 0; i < 4; i++){

			double topX, topY;
			double topX1, topY1;
			double botX, botY;
			double botX1, botY1;

			topX = projTop[i][0];
			topY = projTop[i][1];
			topX1 = projTop[(i+1)%4][0];
			topY1 = projTop[(i+1)%4][1];

			botX = projBottom[i][0];
			botY = projBottom[i][1];
			botX1 = projBottom[(i+1)%4][0];
			botY1 = projBottom[(i+1)%4][1];

			SDL_RenderDrawLine(renderer,
				topX, topY,
				botX, botY);

			SDL_RenderDrawLine(renderer,
				topX, topY,
				topX1, topY1);

			SDL_RenderDrawLine(renderer,
				botX, botY,
				botX1, botY1);

			fill_circle(renderer, topX, topY, 5);
			fill_circle(renderer, botX, botY, 5);

		}

	}

};

#endif // CUBE_HPP
