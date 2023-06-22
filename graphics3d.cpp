#include "graphics3d.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "vec3.hpp"
#include <array>

using namespace graphics3d;

void graphics3d::project_point(Vec3 point, Vec3 viewPoint, Plane projectionPlane, double& ret_x, double& ret_y){
	Line l = Line(viewPoint, point-viewPoint);
	Vec3 ip = l.intersect(projectionPlane);
	ret_x = ip.x();
	ret_y = ip.y();
}

void Object::apply_position_and_scale(void){
	int i = 0;
	for(Vec3 p: this->objVertices){
		p = p + (this->objPos * this->objScale);
		this->objVertices[i] = p;
		i += 1;
	}
	
}

void Object::apply_rotation(void){
	int i = 0;
	Vec3 axis = this->objVertices[0];
	for(Vec3 p: this->objVertices){
		p = vec3_rotate(p, this->objRotation, axis);
		this->objVertices[i] = p;
		i += 1;
	}
}

void Object::build(void){
	this->objVertices.clear();
	this->objEdges.clear();
	this->objTriangles.clear();
	this->make_vertices();
	this->make_edges();
	this->make_triangles();
	this->apply_position_and_scale();
	this->apply_rotation();
}

void Cube::make_vertices(void){
	Vec3 verticalTranslation{0,1,0};

	Vec3 v1{0,0,0};
	Vec3 v2{1,0,0};
	Vec3 v3{1,0,1};
	Vec3 v4{0,0,1};

	this->objVertices.push_back(v1);
	this->objVertices.push_back(v2);
	this->objVertices.push_back(v3);
	this->objVertices.push_back(v4);

	this->objVertices.push_back(v1+verticalTranslation);
	this->objVertices.push_back(v2+verticalTranslation);
	this->objVertices.push_back(v3+verticalTranslation);
	this->objVertices.push_back(v4+verticalTranslation);

}


void Cube::make_edges(void){
	int edgeBottom[2], edgeTop[2], edgeSide[2], edgeDiagonal[2];
	for(int i = 0; i < 4; i++){
		edgeBottom[0] = i;
		edgeBottom[1] = (i+1)%4;
		this->objEdges.push_back(edgeBottom[0]);
		this->objEdges.push_back(edgeBottom[1]);

		edgeTop[0] = i+4;
		edgeTop[1] = (i+1)%4 + 4;
		this->objEdges.push_back(edgeTop[0]);
		this->objEdges.push_back(edgeTop[1]);

		edgeSide[0] = i;
		edgeSide[1] = i+4;
		this->objEdges.push_back(edgeSide[0]);
		this->objEdges.push_back(edgeSide[1]);

		// Diagonais das faces lateriais
		edgeDiagonal[0] = i;
		edgeDiagonal[1] = (i+1)%4 + 4;
		this->objEdges.push_back(edgeDiagonal[0]);
		this->objEdges.push_back(edgeDiagonal[1]);
	}

	// Diagonais das faces superiores
	edgeDiagonal[0] = 0;
	edgeDiagonal[1] = 2;
	this->objEdges.push_back(edgeDiagonal[0]);
	this->objEdges.push_back(edgeDiagonal[1]);

	edgeDiagonal[0] = 4;
	edgeDiagonal[1] = 6;
	this->objEdges.push_back(edgeDiagonal[0]);
	this->objEdges.push_back(edgeDiagonal[1]);
}

void Cube::make_triangles(void){

}

void Camera::build_all(void){
	for(Object *obj: this->objList){
		obj->build();
	}
}

void Camera::project(void){
	this->projectedVertices.clear();
	Plane viewPlane{this->cameraPosition, this->cameraDirection};
	Vec3 viewPoint{this->cameraPosition - this->cameraDirection*this->cameraLength};
	
	for(Object *obj: this->objList){
		std::vector<double> proj;
		for(Vec3 v: obj->objVertices){
			double x,y;
			project_point(v, viewPoint, viewPlane, x, y);

			proj.push_back(x);
			proj.push_back(y);
		}
		this->projectedVertices.push_back(proj);
		
	}
}

inline int Camera::count_projected_points(int objIndex){
	return this->projectedVertices[objIndex].size() / 2;
}

inline void Camera::get_projected_point(int objIndex, int pointIndex, double &retX, double &retY){
	auto vVec = this->projectedVertices[objIndex];
	retX = vVec[pointIndex*2];
	retY = vVec[pointIndex*2+1];
}

void Camera::draw(SDL_Renderer *renderer){
	this->build_all();
	this->project();


	for(int i = 0; i < this->objList.size(); i++){
		Object *obj = this->objList[i];

		auto vertices = this->projectedVertices[i];
		int vertSize = vertices.size();

		auto objEdges = obj->objEdges;
		int edgeSize = objEdges.size();

		SDL_SetRenderDrawColor(renderer, 255,0,0,255);
		for(int j = 0; j < (vertSize/2); j++){
			double x,y;
			int screenX, screenY;
			this->get_projected_point(i, j, x,y);
			this->get_screen_coordinates(x, y, screenX, screenY);

			fill_circle(renderer, screenX, screenY, 5);
		}

		SDL_SetRenderDrawColor(renderer,0,0,255,255);
		for(int k = 0; k <(edgeSize/2); k++){
			double x1, y1, x2, y2;
			int pi1, pi2, screenX1, screenY1, screenX2, screenY2;
			pi1 = objEdges[k*2];
			pi2 = objEdges[k*2+1];

			this->get_projected_point(i, pi1, x1, y1);
			this->get_projected_point(i, pi2, x2, y2);

			this->get_screen_coordinates(x1,y1, screenX1,screenY1);
			this->get_screen_coordinates(x2,y2, screenX2,screenY2);

			SDL_RenderDrawLine(renderer, screenX1,screenY1,screenX2,screenY2);
		}
	}
}
