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
    std::ifstream myfile;
    myfile.open("/Users/adam/Documents/GitHub/rtxi_vm_copy/ss_modules/ss_ctrl/params/plant_params.txt");
    //don't know how to correctly do this with relative paths
    
    if (myfile.good())
    {
        pullParamLine(myfile); //gets nx
        
        std::vector<double> numA = pullParamLine(myfile);
        Eigen::Map<Eigen::Matrix2d> tA(numA.data(),A.rows(),A.cols());
        A = tA;
        
        std::vector<double> numB = pullParamLine(myfile);
        Eigen::Map<Eigen::Vector2d> tB(numB.data(),B.rows(),1);
        B = tB;
        
        
        std::vector<double> numC = pullParamLine(myfile);
        Eigen::Map<Eigen::RowVector2d> tC(numC.data(),1,C.cols());
        C = tC;
        
        std::vector<double> numD = pullParamLine(myfile);
        D = numD[0];
        
        nX = (int) A.cols();
        nU = (int) B.cols();
        nY = (int) C.rows();
        
    }
    else
    {
        std::cout<<"\ncouldnt find\n";
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

