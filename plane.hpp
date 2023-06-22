#ifndef PLANE_HPP
#define PLANE_HPP
#include "vec3.hpp"

// TODO: ponto baseado nas coordenadas dos planos
// x do plano e y do plano, mas terá de haver uma reta de referencia, seja ela das
// abcissas ou das ordenadas (visto estas serem perpendiculares). A partir de uma
// dessas retas, forma-se um completo referencial cartesiano para, posteriormente,
// obter os pontos.
class Plane {
	public:
	Vec3 p0;
	Vec3 n;

	Plane(){
		p0 = Vec3();
		n = Vec3();
	}
	Plane(Vec3 p00, Vec3 normal){
		p0 = p00;
		n = normal;
	}
	Vec3 point() const {
		return p0;
	}
	Vec3 normal() const {
		return n;
	}

};

#endif

