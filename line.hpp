#ifndef LINE_HPP
#define LINE_HPP
#include "vec3.hpp"
#include "plane.hpp"

class Line{
	public:
	Vec3 ori;
	Vec3 dir;

	Line(){
		ori = Vec3();
		dir = Vec3();
	}
	Line(Vec3 v1, Vec3 v2){
		ori = v1;
		dir = v2;
	}
	Vec3 direction() const {
		return dir;
	}
	Vec3 origin() const {
		return ori;
	}
	Vec3 point(double d) const {
		return ori + dir*d;
	}
	Vec3 intersect(Plane p){
		double d_num = vec3_dot(p.point()-ori, p.normal());
		double d_den = vec3_dot(dir, p.normal());
		double d = d_num/d_den;
		
		return point(d);
	}

};

#endif

