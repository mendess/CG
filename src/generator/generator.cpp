#include "generator.hpp"
//#include <cstdlib>
//#include <point.hpp>
//#include <math.h>

using namespace std;

std::vector<Point*> draw_plane(double side_length){

	vector<Point*> coordsPlane;
	float axis = side_length/2;

        //first triangle
        coordsPlane.push_back(new Point(axis, 0, axis));
        coordsPlane.push_back(new Point(-axis, 0, -axis));
        coordsPlane.push_back(new Point(-axis, 0, axis));

        //second triangle
        coordsPlane.push_back(new Point(-axis, 0, -axis));
        coordsPlane.push_back(new Point(axis, 0, axis));
        coordsPlane.push_back(new Point(axis, 0, -axis));

	return coordsPlane;
}



std::vector<Point*> draw_box(double x, double y, double z, int divisions){

	vector<Point*> coordsBox;
	double axisX = x/2;
        double axisY = y/2;
        double axisZ = z/2;
        double spacingX = x/divisions;
        double spacingY = y/divisions;
        double spacingZ = z/divisions;

        for(int i = 0; i < divisions; i++){
                for(int k = 0; k < divisions; k++){
                        //Front
                        coordsBox.push_back(new Point (-axisX + spacingX * i, -axisY + spacingY * k, axisZ));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), -axisY + spacingY * k, axisZ));
                        coordsBox.push_back(new Point (-axisX + spacingX * i, -axisY + spacingY * (k+1), axisZ));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), -axisY + spacingY * k, axisZ));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), -axisY + spacingY * (k+1), axisZ));
                        coordsBox.push_back(new Point (-axisX + spacingX * i, -axisY + spacingY * (k+1), axisZ));

                        //Back
                        coordsBox.push_back(new Point (-axisX + spacingX * i, -axisY + spacingY * k, -axisZ));
                        coordsBox.push_back(new Point (-axisX + spacingX * i, -axisY + spacingY * (k+1), -axisZ));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), -axisY + spacingY * k, -axisZ));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), -axisY + spacingY * k, -axisZ));
                        coordsBox.push_back(new Point (-axisX + spacingX * i, -axisY + spacingY * (k+1), -axisZ));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), -axisY + spacingY * (k+1), -axisZ));

			//Up
			coordsBox.push_back(new Point (-axisX + spacingX * i, axisY, -axisZ + spacingZ * k));
                        coordsBox.push_back(new Point (-axisX + spacingX * i, axisY, -axisZ + spacingZ * (k+1)));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), axisY, -axisZ + spacingZ * (k+1)));
                        coordsBox.push_back(new Point (-axisX + spacingX * i, axisY, -axisZ + spacingZ * k));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), axisY, -axisZ + spacingZ * (k+1)));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), axisY, -axisZ + spacingZ * k));

			//Down
			coordsBox.push_back(new Point (-axisX + spacingX * i, -axisY, -axisZ + spacingZ * k));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), -axisY, -axisZ + spacingZ * (k+1)));
                        coordsBox.push_back(new Point (-axisX + spacingX * i, -axisY, -axisZ + spacingZ * (k+1)));
                        coordsBox.push_back(new Point (-axisX + spacingX * i, -axisY, -axisZ + spacingZ * k));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), -axisY, -axisZ + spacingZ * k));
                        coordsBox.push_back(new Point (-axisX + spacingX * (i+1), -axisY, -axisZ + spacingZ * (k+1)));

			//Left
			coordsBox.push_back(new Point (-axisX, -axisY + spacingY * i, -axisZ + spacingZ * k));
                        coordsBox.push_back(new Point (-axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k+1)));
                        coordsBox.push_back(new Point (-axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * k));
                        coordsBox.push_back(new Point (-axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * k));
                        coordsBox.push_back(new Point (-axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k+1)));
                        coordsBox.push_back(new Point (-axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * (k+1)));

			//Right
			coordsBox.push_back(new Point (axisX, -axisY + spacingY * i, -axisZ + spacingZ * k));
                        coordsBox.push_back(new Point (axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * k));
                        coordsBox.push_back(new Point (axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k+1)));
                        coordsBox.push_back(new Point (axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * k));
                        coordsBox.push_back(new Point (axisX, -axisY + spacingY * (i+1), -axisZ + spacingZ * (k+1)));
                        coordsBox.push_back(new Point (axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k+1)));


		}
	}

	return coordsBox;
}




std::vector<Point*> draw_sphere(double radius, double slices, double stacks){
   
	vector<Point*> coordsSphere;

	//calculate the Phi Movement (triangle's up/down side length) 
        //PI * 2 because it is necessary to go all around to count all of the slices 
        float phiMovement = M_PI * 2 / slices;

        //calculate the Theta Movement (triangle's left/right side length)
        //only PI because it is only necessary to go half the perimeter in the 
        //vertical direction to count all of the stacks;
        float thetaMovement = M_PI / stacks;


        for(float phi = 0; phi < slices; phi++)
                for(float theta = 0; theta < stacks; theta++){

                        float currentStack = theta * thetaMovement;
                        float currentSlice = phi * phiMovement;

                        coordsSphere.push_back(new Point(radius * sin(currentStack + thetaMovement) * sin(currentSlice + phiMovement),
                                                   radius * cos(currentStack + thetaMovement),
                                                   radius * sin(currentStack + thetaMovement) * cos(currentSlice + phiMovement)));

                        coordsSphere.push_back(new Point(radius * sin(currentStack) * sin(currentSlice),
                                                   radius * cos(currentStack),
                                                   radius * sin(currentStack) * cos(currentSlice)));

                        coordsSphere.push_back(new Point(radius * sin(currentStack + thetaMovement) * sin(currentSlice),
                                                   radius * cos(currentStack + thetaMovement),
                                                   radius * sin(currentStack + thetaMovement) * cos(currentSlice)));

                        coordsSphere.push_back(new Point(radius * sin(currentStack) * sin(currentSlice + phiMovement),
                                                   radius * cos(currentStack),
                                                   radius * sin(currentStack) * cos(currentSlice + phiMovement)));

                        coordsSphere.push_back(new Point(radius * sin(currentStack) * sin(currentSlice),
                                                   radius * cos(currentStack),
                                                   radius * sin(currentStack) * cos(currentSlice)));

                        coordsSphere.push_back(new Point(radius * sin(currentStack + thetaMovement) * sin(currentSlice + phiMovement),
                                                   radius * cos(currentStack + thetaMovement),
                                                   radius * sin(currentStack + thetaMovement) * cos(currentSlice + phiMovement)));
                }
	

	return coordsSphere;
}

std::vector<Point*> draw_cone(double radius, double height, int slices, int stacks){

	vector<Point*> coordsCone;
	float phi = (2 * M_PI) / slices;
	float stackSpacing = sqrt(pow(radius,2) + pow(height,2)) / stacks;
	float theta = radius / stacks;

	for(int i = 0; i < stacks; i++){
		for(int k = 0; k < slices; k++){

			if(!i){
				//Base
				coordsCone.push_back(new Point(0, 0, 0));
				coordsCone.push_back(new Point(radius * sin(phi * (k+1)), 0, radius * cos(phi * (k+1))));
				coordsCone.push_back(new Point(radius * sin(phi * k), 0, radius * cos(phi * k)));
				}
			
			if(i == stacks-1){
				//Top
				coordsCone.push_back(new Point((radius - theta * i) * sin(phi * k), i * stackSpacing, (radius - theta * i) * cos(phi * k)));
				coordsCone.push_back(new Point((radius - theta * i) * sin(phi * (k+1)), i * stackSpacing, (radius - theta * i) * cos(phi * (k+1))));
				coordsCone.push_back(new Point(0, stacks * stackSpacing, 0));
				}

			else{	//Around
				coordsCone.push_back(new Point((radius - theta * i) * sin(phi * k), i * stackSpacing, (radius - theta * i) * cos(phi * k)));
				coordsCone.push_back(new Point((radius - theta * (i+1)) * sin(phi * (k+1)), (i+1) * stackSpacing, (radius - theta * (i+1)) * cos(phi * (k+1))));
				coordsCone.push_back(new Point((radius - theta * (i+1)) * sin(phi * k), (i+1) * stackSpacing, (radius - theta * (i+1)) * cos(phi * k)));
				coordsCone.push_back(new Point((radius - theta * i) * sin(phi * k), i * stackSpacing, (radius - theta * i) * cos(phi * k)));
				coordsCone.push_back(new Point((radius - theta * i) * sin(phi * (k+1)), i * stackSpacing, (radius - theta * i) * cos(phi * (k+1))));
				coordsCone.push_back(new Point((radius - theta * (i+1)) * sin(phi * (k+1)), (i+1) * stackSpacing, (radius - theta * (i+1)) * cos(phi * (k+1))));
				} 

		/*	if(i == 0){
				//Base
				coordsCone.push_back(new Point(0, 0, 0));
				coordsCone.push_back(new Point(radius * cos(0) * cos(phi * (k+1)), 0, radius * cos(0) * sin(phi * (k+1))));
                                coordsCone.push_back(new Point(radius * cos(0)  * cos(phi * k), 0, radius * cos(0) * sin(phi * k)));

			 if(i == stacks-1){
                                //Top
                                coordsCone.push_back(new Point(radius * cos(theta) * cos(phi * k), i * stackSpacing, radius * cos(theta) * cos(phi * k)));
                                coordsCone.push_back(new Point((radius - theta * i) * sin(phi * (k+1)), i * stackSpacing, (radius - theta * i) * cos(phi * (k+1))));
                                coordsCone.push_back(new Point(0, stacks * stackSpacing, 0));
                                }

                        else{   //Around
                                coordsCone.push_back(new Point((radius - theta * i) * sin(phi * k), i * stackSpacing, (radius - theta * i) * cos(phi * k)));
                                coordsCone.push_back(new Point((radius - theta * (i+1)) * sin(phi * (k+1)), (i+1) * stackSpacing, (radius - theta * (i+1)) * cos(phi * (k+1))));
                                coordsCone.push_back(new Point((radius - theta * (i+1)) * sin(phi * k), (i+1) * stackSpacing, (radius - theta * (i+1)) * cos(phi * k)));
                                coordsCone.push_back(new Point((radius - theta * i) * sin(phi * k), i * stackSpacing, (radius - theta * i) * cos(phi * k)));
                                coordsCone.push_back(new Point((radius - theta * i) * sin(phi * (k+1)), i * stackSpacing, (radius - theta * i) * cos(phi * (k+1))));
                                coordsCone.push_back(new Point((radius - theta * (i+1)) * sin(phi * (k+1)), (i+1) * stackSpacing, (radius - theta * (i+1)) * cos(phi * (k+1))));
                                } 

				
			}*/
		}
	}


	return coordsCone;
}
