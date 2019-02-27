#include "generator.hpp"
//#include <cstdlib>
//#include <point.hpp>
//#include <math.h>

using namespace std;

vector<Point> draw_plane(double side_length){

	float axis = sideLength/2;

        //first triangle
        coords.push_back(new Point(axis, 0, axis));
        coords.push_back(new Point(-axis, 0, axis));
        coords.push_back(new Point(-axis, 0, -axis));

        //second triangle
        coords.push_back(new Point(-axis, 0, -axis));
        coords.push_back(new Point(axis, 0, -axis));
        coords.push_back(new Point(axis, 0, axis));

	return vector<Point>();
}



vector<Point> draw_box(double x, double y, double z){

	double axisX = x/2;
        double axisY = y/2;
        double axisZ = z/2;
        double spacingX = x/divisions;
        double spacingY = y/divisions;
        double spacingZ = z/divisions;

        for(int i = 0; i < divisions; i++){
                for(int k = 0; k < divisions; k++){
                        //Front
                        coords.push_back(new Point (−axisX + spacingX * i, -axisY + spacingY * k, axisZ));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), -axisY + spacingY * k, axisZ));
                        coords.push_back(new Point (−axisX + spacingX * i, -axisY + spacingY * (k+1), axisZ));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), -axisY + spacingY * k, axisZ));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), -axisY + spacingY * (k+1), axisZ));
                        coords.push_back(new Point (−axisX + spacingX * i, -axisY + spacingY * (k+1), axisZ));

                        //Back
                        coords.push_back(new Point (−axisX + spacingX * i, -axisY + spacingY * k, -axisZ));
                        coords.push_back(new Point (−axisX + spacingX * i, -axisY + spacingY * (k+1), -axisZ));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), -axisY + spacingY * k, -axisZ));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), -axisY + spacingY * k, -axisZ));
                        coords.push_back(new Point (−axisX + spacingX * i, -axisY + spacingY * (k+1), -axisZ));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), -axisY + spacingY * (k+1), -axisZ));

			//Up
			coords.push_back(new Point (−axisX + spacingX * i, axisY, -axisZ + spacingZ * k));
                        coords.push_back(new Point (−axisX + spacingX * i, axisY, -axisZ + spacingZ * (k+1)));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), axisY, -axisZ + spacingZ * (k+1)));
                        coords.push_back(new Point (−axisX + spacingX * i, axisY, -axisZ + spacingZ * k));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), axisY, -axisZ + spacingZ * (k+1)));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), axisY, -axisZ + spacingZ * k));

			//Down
			coords.push_back(new Point (−axisX + spacingX * i, -axisY, -axisZ + spacingZ * k));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), -axisY, -axisZ + spacingZ * (k+1)));
                        coords.push_back(new Point (−axisX + spacingX * i, -axisY, -axisZ + spacingZ * (k+1)));
                        coords.push_back(new Point (−axisX + spacingX * i, -axisY, -axisZ + spacingZ * k));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), -axisY, -axisZ + spacingZ * k));
                        coords.push_back(new Point (−axisX + spacingX * (i+1), -axisY, -axisZ + spacingZ * (k+1)));

			//Left
			coords.push_back(new Point (−axisX, -axisY + spacingY * i, -axisZ + spacingZ * k));
                        coords.push_back(new Point (−axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k+1)));
                        coords.push_back(new Point (−axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * k));
                        coords.push_back(new Point (−axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * k));
                        coords.push_back(new Point (−axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k+1)));
                        coords.push_back(new Point (−axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * (k+1)));

			//Right
			coords.push_back(new Point (axisX, -axisY + spacingY * i, -axisZ + spacingZ * k));
                        coords.push_back(new Point (axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * k));
                        coords.push_back(new Point (axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k+1)));
                        coords.push_back(new Point (axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * k));
                        coords.push_back(new Point (axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * (k+1)));
                        coords.push_back(new Point (axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k+1)));


		}
	}

	return vector<Point>();
}




vector<Point> draw_sphere(double radius, double slices, double stacks){
   
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
	

	return vector<Point>();
}

vector<Point> draw_cone(double radius, double height, int slices, int stacks){

	float phi = (2 * M_PI) / slices;
	float stackSpacing = sqrt(pow(r, 2) + pow(h, 2) / stacks;
	float theta = radius / slices;

	for(int i = 0; i < stacks; i++){
		for(int k = 0; k < slices; k++){

			if(!i){
				//Base
				coords.push_back(new Point(0, 0, 0));
				coords.push_back(new Point(radius * sin(phi * (k+1)), 0, radius * cos(phi * (k+1)));
				coords.push_back(new Point(radius * sin(phi * (k+1)), 0, radius * cos(phi * (k+1)));


	return vector<Point>();
}


//create vector to store the points (coords)



