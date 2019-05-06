//
//  plds_adam_funs.cpp
//  plds_adamX
//
//  Created by Adam Willats on 4/3/19.
//  Copyright © 2019 Adam Willats. All rights reserved.
//

#include "plds_adam_funs.hpp"


void plds_adam::printSys()
{
    std::cout <<"Here is the matrix A:\n" << A << "\n";
    std::cout <<"Here is the matrix B:\n" << B << "\n";
    std::cout <<"Here is the matrix C:\n" << C << "\n";
    std::cout <<"Here is the matrix D:\n" << D << "\n";
}
void plds_adam::loadParamsFromTxt()
{
	std::string homepath = getenv("HOME");
	std::ifstream myfile;
	myfile.open(homepath+"/RTXI/modules/ss_modules/ss_ctrl/params/plant_params.txt");
    
    if (myfile.good())
    {
        pullParamLine(myfile); //gets nx
        
	A=stdVec2EigenM(pullParamLine(myfile), A.rows(), A.cols());
       	B = stdVec2EigenV(pullParamLine(myfile), B.rows());
	C = stdVec2EigenRV(pullParamLine(myfile), C.cols());

	std::vector<double> numD = pullParamLine(myfile); 	
	D = numD[0]; 
        
        nX = (int) A.cols();
        nU = (int) B.cols();
        nY = (int) C.rows();
        
    }
    else
    {
        std::cout<<"\ncouldnt find plds params\n";
    }
    myfile.close();

}

void plds_adam::resetSys()
{
    x = Eigen::Vector2d::Zero(nX);
    y = 0;
    u = 0;
}

void plds_adam::initSys()
{
    loadParamsFromTxt();
    resetSys();
    printSys();
}

void plds_adam::stepPlant(double newU)
{
    u = newU;
    x = A*x + B*u; //+noise
    y = C*x + D*u;  //+noise
}
void plds_adam::stepPlant(Eigen::Vector2d newX, double newU)
{
    x = newX; //allows overriding x at step, as a solution for switching
    stepPlant(newU);
}

