#ifndef VEC3_HPP
#define VEC3_HPP
#include <cmath>
#include <cstdio>

struct Vec3Rot {
	double pitch, roll, yaw;
};

class Vec3{
	public:
	double e[3];
	Vec3(){
		e[0] = 0;
		e[1] = 0;
		e[2] = 0;
	}
	Vec3(double x, double y, double z){
		e[0] = x;
		e[1] = y;
		e[2] = z;
	}
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
	double length() const {
		return std::sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
	}
	Vec3 const operator-(){
		return Vec3(-e[0], -e[1], -e[2]);
	}
	void show(){
		printf("(x,y,z)=(%6.2f %6.2f %6.2f)\n", e[0], e[1], e[2]);
	}
};

inline Vec3 operator-(const Vec3& v00, const Vec3& v11){
	return Vec3(v00.e[0]-v11.e[0], v00.e[1]-v11.e[1], v00.e[2]-v11.e[2]);
}
inline Vec3 operator+(const Vec3& v00, const Vec3& v11){
	return Vec3(v00.e[0]+v11.e[0], v00.e[1]+v11.e[1], v00.e[2]+v11.e[2]);
}
inline Vec3 operator*(const Vec3& v, double d){
	return Vec3(v.e[0]*d, v.e[1]*d, v.e[2]*d);
}
inline Vec3 operator/(const Vec3& v, double d){
	return Vec3(v.e[0]/d, v.e[1]/d, v.e[2]/d);
}

inline Vec3 vec3_normalize(Vec3 v){
	double length = v.length();
	return v/length;
}

inline double vec3_dot(Vec3 v00, Vec3 v11){
	return v00.e[0]*v11.e[0] + v00.e[1]*v11.e[1] + v00.e[2]*v11.e[2];
}

inline Vec3 vec3_cross(Vec3 v00, Vec3 v11){
	return Vec3(v00.e[1] * v11.e[2] - v00.e[2] * v11.e[1],
	     v00.e[2] * v11.e[0] - v00.e[0] * v11.e[2],
	     v00.e[0] * v11.e[1] - v00.e[1] * v11.e[0]);
}

[[ deprecated ]] Vec3 vec3_rotate(Vec3 v00, double angle, Vec3 axis);
Vec3 vec3_rotate(Vec3 v00, struct Vec3Rot *rot, Vec3 axis);

#endif // VEC3_HPP
