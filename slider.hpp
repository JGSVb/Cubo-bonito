#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>
#include "graphics.hpp"

// COR DE FUNDO ALEATÓRIA
#include <cstdlib>
#include <ctime>

class Slider: public Drawable{
	private:
	SDL_Renderer *renderer;
	SDL_Texture *labelTexture;
	SDL_Texture *valueTexture;
	double actualVal;
	double defaultVal;
	double minVal;
	double maxVal;
	std::string label;
	std::function<void(double)> valCallback;

	void make_texture(void);
	void update(void);

	SDL_Color bgColor;

	public:
	double increment;

	Slider(SDL_Renderer *render, std::string lbl) : Drawable { -100 } {
		renderer = render;
		label = lbl;
		actualVal = 0;
		minVal = -100;
		maxVal = +100;
		increment = 10;
		xPosition = 0;
		yPosition = 0;
		defaultVal = 0;

		labelTexture = NULL;
		valueTexture = NULL;

		valCallback = NULL;

		// FUNDO ALEATÓRIO
		bgColor.r = (double)std::rand() / RAND_MAX * 70 + (255-70);
		bgColor.g = (double)std::rand() / RAND_MAX * 70 + (255-70);
		bgColor.b = (double)std::rand() / RAND_MAX * 70 + (255-70);
		bgColor.a = 255;

		update();
	}

	~Slider(void){
		dispose();
	}

	void return_to_default(void){
		set_value(defaultVal);
	}

	void set_default(double d){
		defaultVal = d;
		set_value(d);
	}
	double get_default(void){
		return defaultVal;
	}

	void set_position(int x, int y){
		xPosition = x;
		yPosition = y;

		// necessário, visto alterada a posição
		update();
	}

	void set_value(double val){
		val = std::max(val, minVal);
		val = std::min(val, maxVal);
		actualVal = val;
		if(valCallback){
			valCallback(val);
		}
		// aquando duma alteração no valor, há uma atualização na componente gráfica
		update();
	}
	double get_value(void){
		return actualVal;
	}
	void set_increment(double i){
		increment = i;
	}
	double get_increment(void){
		return increment;
	}

	void increase(double fac = 1){
		set_value(get_value() + increment*fac);
	}
	void decrease(double fac = 1){
		set_value(get_value() - increment*fac);
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

	void set_callback(std::function<void(double)> cb){
		valCallback = cb;
	}
	std::function<void(double)> get_callback(void){
		return valCallback;
	}

	void get_size(int &w, int &h);
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
