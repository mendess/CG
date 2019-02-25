#include "generator.hpp"
//#include <cstdlib>
//#include <point.hpp>
//#include <math.h>

using namespace std;

vector<Point> draw_plane(double side_length){
    return vector<Point>();
}

vector<Point> draw_box(double x, double y, double z){
    return vector<Point>();
}

vector<Point> draw_sphere(double radius, double slices, double stacks){
    return vector<Point>();
}

vector<Point> draw_cone(double radius, double slices, double stacks){
    return vector<Point>();
}


//create vector to store the points (coords)

void planeCoords(float sideLength){ 
	
	float axis = sideLength/2;

	//first triangle
	coords.push_back(new Point(axis, 0, axis));
	coords.push_back(new Point(-axis, 0, axis));
	coords.push_back(new Point(-axis, 0, -axis));

	//second triangle
	coords.push_back(new Point(-axis, 0, -axis));
	coords.push_back(new Point(axis, 0, -axis));
	coords.push_back(new Point(axis, 0, axis));
}


void sphereCoords(float radius, int stacks, int slices){

	//calculate the Phi Movement (triangle's up/down side length) 
	//PI * 2 because it is necessary to go all around to count all of the slices 
	float phiMove = M_PI * 2 / slices;
	
	//calculate the Theta Movement (triangle's left/right side length)
	//only PI because it is only necessary to go half the perimeter in the 
	//vertical direction to count all of the stacks;
	float thetaMove = M_PI / stacks;

	
	for(float phi = 0; phi < slices; phi++)
		for(float theta = 0; theta < stacks; theta++){
			
			float currentStack = theta * thetaMove;
			float currentSlice = phi * phiMove;

			coords.push_back(new Point(radius * sin(currentStack + thetaMovement) * sin(currentSlice + phiMovement),
						   radius * cos(currentStack + thetaMovement),
						   radius * sin(currentStack + thetaMovement) * cos(currentSlice + phiMovement))); 
			
			coords.push_back(new Point(radius * sin(currentStack) * sin(currentSlice),
                                                   radius * cos(currentStack),
                                                   radius * sin(currentStack) * cos(currentSlice)));

			coords.push_back(new Point(radius * sin(currentStack + thetaMovement) * sin(currentSlice),
                                                   radius * cos(currentStack + thetaMovement),
                                                   radius * sin(currentStack + thetaMovement) * cos(currentSlice)));

			coords.push_back(new Point(radius * sin(currentStack) * sin(currentSlice + phiMovement),
                                                   radius * cos(currentStack),
                                                   radius * sin(currentStack) * cos(currentSlice + phiMovement)));

			coords.push_back(new Point(radius * sin(currentStack) * sin(currentSlice),
                                                   radius * cos(currentStack),
                                                   radius * sin(currentStack) * cos(currentSlice)));

			coords.push_back(new Point(radius * sin(currentStack + thetaMovement) * sin(currentSlice + phiMovement),
                                                   radius * cos(currentStack + thetaMovement),
                                                   radius * sin(currentStack + thetaMovement) * cos(currentSlice + phiMovement)));
		}
}


