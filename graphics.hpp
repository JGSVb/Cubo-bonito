#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
#include <SDL2/SDL.h>
#include "vec3.hpp"
#include "plane.hpp"
#include "constants.hpp"

typedef enum {
	POSITION_BELOW,
	POSITION_ABOVE,
	POSITION_LEFT,
	POSITION_RIGHT
} position_relation_t;

void fill_circle(SDL_Renderer *renderer, int x, int y, double rad, double epsilon = 1e-3);
void fill_triangle(SDL_Renderer *renderer, double x1, double y1, double x2);

class Drawable {
	private:
	int priority;

	protected:
	int xPosition;
	int yPosition;
	int horizontalSize;
	int verticalSize;

	public:
	Drawable(int p) : priority {p}, xPosition {0}, yPosition {0}, horizontalSize {WINWID}, verticalSize{WINHEI} {}
	
	virtual void draw([[ maybe_unused ]]SDL_Renderer *renderer){
		puts("arroz");
		return;
	}
	virtual void process_event([[ maybe_unused ]] SDL_Event *ev){
		return;
	}

	virtual void get_rect(SDL_Rect &rect){
		rect.x = xPosition;
		rect.y = yPosition;
		rect.w = horizontalSize;
		rect.h = verticalSize;
	}
	virtual int get_priority(void) final {
		return priority;
	}

	virtual void set_position(int x, int y){
		xPosition = x;
		yPosition = y;
	}
	virtual void get_position(int &x, int &y){
		x = xPosition;
		y = yPosition;
	}
	virtual void get_size(int &w, int &h){
		w = horizontalSize;
		h = verticalSize;
	}
	virtual void set_size(int w, int h){
		horizontalSize = w;
		verticalSize = h;
	}

	virtual void position_related_to(Drawable *obj, position_relation_t pos){
		int w,h,x,y;
		obj->get_size(w,h);
		obj->get_position(x,y);

		switch(pos){
			case POSITION_BELOW:
				set_position(x, y+h);
				break;
			case POSITION_ABOVE:
				set_position(x, y-h);
				break;
			case POSITION_LEFT:
				set_position(x-w, y);
				break;
			case POSITION_RIGHT:
				set_position(x+w, y);
				break;
			default:
				break;
		}

	}

};

namespace graphics {
	void init(void);
	void quit(void);
	void register_drawable(Drawable *s);
	void unregister_drawable(Drawable *s);
	SDL_Renderer *renderer(void);
	void main_loop(void);
}

#endif
