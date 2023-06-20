#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdexcept>
#include "graphics.hpp"

class Slider: virtual public Drawable{
	private:
	SDL_Renderer *renderer;
	SDL_Texture *labelTexture;
	SDL_Texture *valueTexture;
	double actualVal;
	double minVal;
	double maxVal;
	int xPosition;
	int yPosition;
	std::string label;

	void make_texture(void);
	void update(void);

	public:
	double increment;

	Slider(SDL_Renderer *render, std::string lbl){
		Drawable();
		renderer = render;
		label = lbl;
		actualVal = 0;
		minVal = -100;
		maxVal = +100;
		increment = 10;
		xPosition = 0;
		yPosition = 0;

		labelTexture = NULL;
		valueTexture = NULL;

		update();
	}

	~Slider(void){
		dispose();
	}

	void set_position(int x, int y){
		xPosition = x;
		yPosition = y;

		// necessário, visto alterada a posição
		update();
	}
	void get_position(int &x, int &y){
		x = xPosition;
		y = yPosition;
	}

	void set_value(double val){
		val = std::max(val, minVal);
		val = std::min(val, maxVal);
		actualVal = val;
		// aquando duma alteração no valor, há uma atualização na componente gráfica
		update();
	}
	double get_value(void){
		return actualVal;
	}

	void increase(void){
		set_value(get_value() + increment);
	}
	void decrease(void){
		set_value(get_value() - increment);
	}

	void set_boundries(double min, double max){
		minVal = min;
		maxVal = max;
		// atualizar o valor, visto alterados os limites
		set_value(actualVal);
	}
	void get_boundries(double &min, double &max){
		min = minVal;
		max = maxVal;
	}

	void get_label_rect(SDL_Rect &rect);
	void get_value_rect(SDL_Rect &rect);
	void get_rect(SDL_Rect &rect);
	void draw(SDL_Renderer *renderer);
	void process_event(SDL_Event *ev);
	void dispose(void);

	static void init(void);
	static void finish(void);
};

#endif // SLIDER_HPP
