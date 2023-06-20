#include "slider.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>

static constexpr char *FONTPATH {"./font.ttf"};
static constexpr int FONTSIZE {24};
static constexpr SDL_Color FONTCOLOR {0,0,0,255};

static TTF_Font *G_font;
static bool G_loaded {false};

void Slider::init(void){
	if(G_loaded){
		throw std::runtime_error("Impossível iniciar: já está iniciado");
	}

	G_loaded = true;

	G_font = TTF_OpenFont(FONTPATH, FONTSIZE);

}

void Slider::finish(void){
	if(!G_loaded){
		throw std::runtime_error("Impossível terminar: já está terminado");
	}

	G_loaded = false;

	TTF_CloseFont(G_font);
}

void Slider::dispose(void){
	if(this->labelTexture){
		SDL_DestroyTexture(this->labelTexture);
		this->labelTexture = NULL;
	}
	if(this->valueTexture){
		SDL_DestroyTexture(this->valueTexture);
		this->valueTexture = NULL;
	}
}

void Slider::make_texture(void){
	this->dispose();

	SDL_Surface *labelSurface = TTF_RenderText_Solid(G_font, this->label.c_str(), FONTCOLOR);
	SDL_Surface *valueSurface = TTF_RenderText_Solid(G_font, std::to_string(this->actualVal).c_str(), FONTCOLOR);

	this->labelTexture = SDL_CreateTextureFromSurface(this->renderer, labelSurface);
	this->valueTexture = SDL_CreateTextureFromSurface(this->renderer, valueSurface);

	SDL_FreeSurface(labelSurface);
	SDL_FreeSurface(valueSurface);
}

void Slider::get_label_rect(SDL_Rect &rect){
	rect.x = this->xPosition;
	rect.y = this->yPosition;
	rect.w = 20*this->label.length();
	rect.h = 30;
}

void Slider::get_value_rect(SDL_Rect &rect){
	SDL_Rect labelRect;
	this->get_label_rect(labelRect);
	rect.x = this->xPosition;
	rect.y = this->yPosition + labelRect.h;
	rect.w = 15 * std::to_string(this->actualVal).length();
	rect.h = 23;
}

void Slider::get_rect(SDL_Rect &rect){
	SDL_Rect labelRect;
	SDL_Rect valueRect;
	this->get_label_rect(labelRect);
	this->get_value_rect(valueRect);

	rect.x = this->xPosition;
	rect.y = this->yPosition;
	rect.w = std::max(labelRect.w, valueRect.w);
	rect.h = labelRect.h + valueRect.h;
}

void Slider::draw(SDL_Renderer *renderer){
	SDL_Rect rect, valueRect, labelRect;
	get_rect(rect);
	get_value_rect(valueRect);
	get_label_rect(labelRect);

	SDL_SetRenderDrawColor(renderer, 200,200,255,0);
	SDL_RenderFillRect(renderer, &rect);

	SDL_RenderCopy(renderer, this->labelTexture, NULL, &labelRect);
	SDL_RenderCopy(renderer, this->valueTexture, NULL, &valueRect);
}

void Slider::process_event(SDL_Event *ev){
	if(ev->type == SDL_MOUSEBUTTONUP){
		if(ev->button.button == SDL_BUTTON_LEFT && ev->button.clicks == 2){
			this->return_to_default();
		}
		return;
	}

	if(ev->type == SDL_MOUSEWHEEL){
		if(ev->wheel.y > 0){
			this->increase();
		} else if(ev->wheel.y < 0){
			this->decrease();
		}
	}
}

void Slider::update(void){
	this->make_texture();
}
