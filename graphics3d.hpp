#ifndef GRAPHICS3D_HPP
#define GRAPHICS3D_HPP
#include "vec3.hpp"
#include "graphics.hpp"
#include <vector>
#include <array>

namespace graphics3d {

	void project_point(Vec3 point, Vec3 viewPoint, Plane projectionPlane, double& ret_x, double& ret_y);

	class Object {
		friend class Camera;

		protected:
		std::vector<Vec3> objVertices;
		std::vector<int> objEdges; // a1, b1, a2, b2, an, bn ... -> Aresta AB: A-----B
		std::vector<int> objTriangles; // a1, b1, c1, a2, b2, c2, an, bn, cn -> Triangulo ABC.
		Vec3 objPos;
		double objScale;
		double objRotation;

		virtual void make_vertices(void)=0;
		virtual void make_edges(void)=0;
		virtual void make_triangles(void)=0;

		void apply_position_and_scale(void);
		void apply_rotation(void);
		virtual void build(void);

		public:
		Object(Vec3 pos, double scale, double rotation):  objVertices{}, objEdges{}, objTriangles{}, objPos{pos}, objScale{scale}, objRotation{rotation} {}
		
	};

	class Camera: public Drawable {
		private:
		inline int count_projected_points(int objIndex);
		inline void get_projected_point(int objIndex, int pointIndex, double &retX, double &retY);

		public:
		Vec3 cameraPosition;
		Vec3 cameraDirection;
		int cameraWidth;
		int cameraHeight;
		std::vector<Object*> objList;
		std::vector<std::vector<double>> projectedVertices; // { {x1, y1, x2, y2, xn, yn}, {...} }
		double cameraLength;
		
		Camera(Vec3 pos, Vec3 dir, double focalLen, int wid, int hei): Drawable{0}, objList{}, projectedVertices{},
			cameraPosition{pos}, cameraDirection{dir}, cameraWidth{wid}, cameraHeight{hei}, cameraLength{focalLen} {};
		
		void add_object(Object *obj){
			objList.push_back(obj);
		}
		void get_screen_coordinates(double x, double y, int &retX, int &retY){
			int maxDim = std::max(cameraWidth, cameraHeight);
			retX = std::round(x*(double)maxDim);
			retY = std::round(cameraHeight - y*(double)maxDim);
		}
		void draw(SDL_Renderer *renderer);
		void build_all(void);
		void project(void);
		
	};

	class Cube: public Object {
		private:
		void make_vertices(void);
		void make_edges(void);
		void make_triangles(void);
		public:
		Cube(Vec3 position, double size, double rotation): Object{position, size, rotation} {}
		
	};

};

#endif // GRAPHICS3D_HPP
