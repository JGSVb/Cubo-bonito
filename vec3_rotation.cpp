#include <cmath>
#include "vec3.hpp"

[[ deprecated ]] Vec3 vec3_rotate(Vec3 v00, double angle, Vec3 axis){
	double c = cos(angle);
	double s = sin(angle);
	v00 = v00-axis;
	return Vec3(v00.e[0]*c - v00.e[2]*s,
			v00.e[1],
			v00.e[0]*s + v00.e[2]*c) + axis;
}

// https://stackoverflow.com/questions/34050929/3d-point-rotation-algorithm
Vec3 vec3_rotate(Vec3 v00, struct Vec3Rot *rot, Vec3 axis){
	double cosa = std::cos(rot->yaw);
	double sina = std::sin(rot->yaw);

	double cosb = std::cos(rot->pitch);
	double sinb = std::sin(rot->pitch);

	double cosc = std::cos(rot->roll);
	double sinc = std::sin(rot->roll);

	double Axx = cosa*cosb;
	double Axy = cosa*sinb*sinc - sina*cosc;
	double Axz = cosa*sinb*cosc + sina*sinc;

	double Ayx = sina*cosb;
	double Ayy = sina*sinb*sinc + cosa*cosc;
	double Ayz = sina*sinb*cosc - cosa*sinc;

	double Azx = -sinb;
	double Azy = cosb*sinc;
	double Azz = cosb*cosc;

	v00 = v00-axis;
	double px = v00.x();
	double py = v00.y();
	double pz = v00.z();

	v00 = Vec3(Axx*px + Axy*py + Axz*pz,
		Ayx*px + Ayy*py + Ayz*pz,
		Azx*px + Azy*py + Azz*pz);
	
	v00 = v00+axis;

	return v00;
}

