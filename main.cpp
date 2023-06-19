#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdexcept>
#include <array>
#include "vec3.hpp"
#include "plane.hpp"
#include "cube.hpp"
#include "constants.hpp"


class Slider {
	private:
	SDL_Renderer *renderer;
	SDL_Texture *labelTexture;
	SDL_Rect labelRect;
	SDL_Texture *valueTexture;
	SDL_Rect valueRect;

	TTF_Font *font;
	bool loaded;

	int xPosition;
	int yPosition;
	
	void make_texture(const char *text, SDL_Texture *&retTexture, SDL_Rect &retRect){
		SDL_Color black {0,0,0,255};
		SDL_Surface *surface = TTF_RenderText_Solid(font, text, black);

		SDL_Rect rect;
		rect.x = xPosition;
		rect.y = yPosition;
		rect.w = 15*strlen(text);
		rect.h = 25;

		retTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		retRect = rect;

	}

	void make_label_texture(){
		make_texture(label.c_str(), labelTexture, labelRect);
	}
	void make_value_texture(){
		make_texture(std::to_string(actualVal).c_str(), valueTexture, valueRect);
		valueRect.y = labelRect.h + labelRect.y;
	}

	public:
	std::string label;
	double actualVal;
	double increment;

	Slider(SDL_Renderer *render){
		renderer = render;
		loaded = false;
		label = "";
		actualVal = 0;

		set_position(0, 0);
	}

	Slider(SDL_Renderer *render, std::string l, double def){
		renderer = render;
		loaded = false;
		label = l;
		actualVal = def;
		increment = 10;

		set_position(0, 0);
	}

	void set_position(int x, int y){
		if(loaded){
			throw std::runtime_error("Impossível mudar posição quando já se carregaram as coisas");
		}
		xPosition = x;
		yPosition = y;
	}
	void get_position(int &x, int &y){
		x = xPosition;
		y = yPosition;
	}

	void load(){
		if(loaded){
			throw std::runtime_error("Impossível carregar quando já se carregou");
		}
		loaded = true;
		font = TTF_OpenFont("./font.ttf", 24);
		make_label_texture();
		make_value_texture();

	}

	void draw(){
		if(!loaded) {
			throw std::runtime_error("Impossível desenhar sem carregamento prévio");
		}

		SDL_Rect rect;
		get_rect(rect);

		SDL_SetRenderDrawColor(renderer, 200,200,255,0);
		SDL_RenderFillRect(renderer, &rect);

		SDL_RenderCopy(renderer, labelTexture, NULL, &labelRect);
		SDL_RenderCopy(renderer, valueTexture, NULL, &valueRect);


	}

	void set_value(double val){
		actualVal = val;
		SDL_DestroyTexture(valueTexture);
		make_value_texture();
	}
	double get_value(){
		return actualVal;
	}

	void finish(){
		if(!loaded) {
			throw std::runtime_error("Impossível finalizar sem carregamento prévio");
		}
		loaded = false;
		SDL_DestroyTexture(labelTexture);
		SDL_DestroyTexture(valueTexture);
		TTF_CloseFont(font);
		
	}
	void get_rect(SDL_Rect &rect){

		rect.x = xPosition;
		rect.y = yPosition;
		rect.w = std::max(valueRect.w, labelRect.w);
		rect.h = valueRect.h + labelRect.h;
	}
	void process_event(SDL_Event *ev){
		if(ev->type != SDL_MOUSEWHEEL){
			return;
		}


		int x, y;
		SDL_GetMouseState(&x, &y);

		SDL_Point point{x, y};
		SDL_Rect rect;
		get_rect(rect);

		if(!SDL_PointInRect(&point, &rect)){
			return;
		}

		if(ev->wheel.y > 0){
			set_value(get_value() + increment);
		} else if(ev->wheel.y < 0){
			set_value(get_value() - increment);
		}
	}

	void put_under(Slider *s){
		if(!s->loaded){
			throw std::runtime_error("Impossível posicionar abaixo de um controlador de deslize não carregado");
		}
		SDL_Rect rect;
		s->get_rect(rect);

		set_position(rect.x, rect.y+rect.h);
	}
	
	
};

constexpr int sliderCount = 5;

bool compute_events(std::array<Slider*, sliderCount> sArr){
	SDL_Event ev;


	while(SDL_PollEvent(&ev)){

		for(Slider *s : sArr){
			s->process_event(&ev);
		}

		switch(ev.type){
			case SDL_QUIT:
				return false;
				break;
			default:
				break;
		}
	}

	return true;
}


int main([[ maybe_unused ]] int argc, [[ maybe_unused ]] char **argv){
	graphics::init();

	Vec3 viewPoint{WINWID/2,WINHEI/2,-100};
	Plane viewPlane{Vec3(0, 0, -400), Vec3(0, 0, 1)};

	Cube cube{200, Vec3(WINWID/2 - 100, WINHEI/2 - 100, 500)};

	Vec3 t{400,0,400};
	Vec3 a{500,0,200};

	Slider vpxxSlider{graphics::renderer(), "Ponto X", WINWID/2};
	Slider vpyySlider{graphics::renderer(), "Ponto Y", WINHEI/2};
	Slider vpzzSlider{graphics::renderer(), "Ponto Z", -100};
	Slider speedSlider{graphics::renderer(), "Velocide de Rotacao", DOUBLEPI*1e-2};
	Slider planeSlider{graphics::renderer(), "Plano Z", -400};
	speedSlider.increment = 0.01;
	
	vpxxSlider.load();

	vpyySlider.put_under(&vpxxSlider);
	vpyySlider.load();

	vpzzSlider.put_under(&vpyySlider);
	vpzzSlider.load();

	planeSlider.put_under(&vpzzSlider);
	planeSlider.load();
	
	speedSlider.put_under(&planeSlider);
	speedSlider.load();

	std::array<Slider*, sliderCount> sliders{&vpxxSlider, &vpyySlider, &vpzzSlider, &speedSlider, &planeSlider};

	while(compute_events(sliders)){
		SDL_SetRenderDrawColor(graphics::renderer(), 255,255,255,255);
		SDL_RenderClear(graphics::renderer());

		SDL_SetRenderDrawColor(graphics::renderer(), 255,0,0,255);
		cube.draw(graphics::renderer(), viewPoint, viewPlane);
		cube.set_rotation(cube.rotation() + speedSlider.actualVal);

		vpxxSlider.draw();
		vpyySlider.draw();
		vpzzSlider.draw();
		viewPoint.e[0] = vpxxSlider.actualVal;
		viewPoint.e[1] = vpyySlider.actualVal;
		viewPoint.e[2] = vpzzSlider.actualVal;

		planeSlider.draw();
		viewPlane.p0.e[2] = planeSlider.actualVal;

		speedSlider.draw();


		SDL_RenderPresent(graphics::renderer());
		SDL_Delay(LOOPDELAY);
	};
	vpxxSlider.finish();
	vpyySlider.finish();
	vpzzSlider.finish();
	speedSlider.finish();


	graphics::quit();
	return 0;
}
