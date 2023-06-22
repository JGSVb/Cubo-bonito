#include <SDL2/SDL.h>
#include "vec3.hpp"
#include "cube.hpp"
#include "graphics.hpp"
#include "graphics3d.hpp"

void Cube::apply_rotation(void){
	Vec3 bottomAxis = (this->bottomSquare[0] + this->bottomSquare[2])/2;
	Vec3 topAxis = (this->topSquare[0] + this->topSquare[2])/2;

	for(int i = 0; i < 4; i++){
		this->bottomSquare[i] = vec3_rotate(this->bottomSquare[i], this->cubeRotation + this->rotationAccumulation, bottomAxis);
		this->topSquare[i] = vec3_rotate(this->topSquare[i], this->cubeRotation + this->rotationAccumulation, topAxis);
		
	}
}
void Cube::build(void){
	this->topSquare[0] = this->cubePosition;
	this->topSquare[1] = this->cubePosition + Vec3(1, 0, 0)*this->cubeSize;
	this->topSquare[2] = this->cubePosition + Vec3(1, 0, 1)*this->cubeSize;
	this->topSquare[3] = this->cubePosition + Vec3(0, 0, 1)*this->cubeSize;

	Vec3 verticalTranslation = Vec3(0, 1, 0)*this->cubeSize;

	for(int i = 0; i < 4; i++){
		this->bottomSquare[i] = this->topSquare[i] + verticalTranslation;
	}

	apply_rotation();
}

void Cube::project(void){
	for(int i = 0; i < 4; i++){
		double x1,y1;
		double x2,y2;

		graphics3d::project_point(this->topSquare[i], this->viewPoint, this->viewPlane, x1, y1);
		this->projTop[i][0] = x1;
		this->projTop[i][1] = y1;

		graphics3d::project_point(this->bottomSquare[i], this->viewPoint, this->viewPlane, x2, y2);
		this->projBottom[i][0] = x2;
		this->projBottom[i][1] = y2;
	}

}

// TODO: a posição e o tamanho não estão a afetar o desenho
// como deveriam de acordo com Drawable::get_size e etc...
void Cube::draw(SDL_Renderer *renderer){
	if(isAnimated){
		rotationAccumulation += rotationSpeed;
		apply_rotation();
	}
	build();

	project();

	SDL_SetRenderDrawColor(renderer, 255,0,0,255);

	for(int i = 0; i < 4; i++){

		double topX, topY;
		double topX1, topY1;
		double botX, botY;
		double botX1, botY1;

		topX = this->projTop[i][0];
		topY = this->projTop[i][1];
		topX1 = this->projTop[(i+1)%4][0];
		topY1 = this->projTop[(i+1)%4][1];

		botX = this->projBottom[i][0];
		botY = this->projBottom[i][1];
		botX1 = this->projBottom[(i+1)%4][0];
		botY1 = this->projBottom[(i+1)%4][1];

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
