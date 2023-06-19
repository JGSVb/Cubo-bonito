#ifndef PLANE_HPP
#define PLANE_HPP
#include "vec3.hpp"

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

