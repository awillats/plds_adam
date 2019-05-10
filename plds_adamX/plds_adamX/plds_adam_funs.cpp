//
//  plds_adam_funs.cpp
//  plds_adamX
//
//  Created by Adam Willats on 4/3/19.
//  Copyright © 2019 Adam Willats. All rights reserved.
//

#include "plds_adam_funs.hpp"

using namespace adam;

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
        nX = (int) pullParamNum(myfile); //gets nx

	A = pullParamLine(myfile); A.reshape(nX,nX);
	B = pullParamLine(myfile);
	C = pullParamLine(myfile);
	D = pullParamNum(myfile); 	

        //nX = (int) A.cols();
        nU = (int) B.n_cols;
        nY = (int) C.n_rows;

	//namespace dependent
/*
	A = stdVec2EigenM(pullParamLine(myfile), A.rows(), A.cols());
       	B = stdVec2EigenV(pullParamLine(myfile), B.rows());
	C = stdVec2EigenRV(pullParamLine(myfile), C.cols());

	std::vector<double> numD = pullParamLine(myfile); 	
	D = numD[0]; 
        
        nX = (int) A.cols();
        nU = (int) B.cols();
        nY = (int) C.rows();
*/
        
    }
    else
    {
        std::cout<<"\ncouldnt find plds params\n";
    }
    myfile.close();

}

void plds_adam::resetSys()
{
    //x = Eigen::Vector2d::Zero(nX); //namespace dependent

    x.set_size(nX);    
    x.fill(0);
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
    x = arma::vectorise(    A*x + B*u    ); //+noise
    y = arma::as_scalar(    C*x + D*u    );
 
}
void plds_adam::stepPlant(Vec newX, double newU)
{
    x = newX; //allows overriding x at step, as a solution for switching
    stepPlant(newU);
}


///////////////////////////////////////////////////////////////////////////////////////PLDS_NOISY
//class is secretly gLDS

/*
void plds_noisy::initSys()
{
	plds_adam::initSys();
	//Q = 0.001*Mat(nX,nX,arma::fill::eye);
	//R = 0.1;
}

void plds_noisy::printSys()
{
    plds_adam::printSys();
    //std::cout <<"Here is the matrix Q:\n" << Q << "\n";
    //std::cout <<"Here is the matrix R:\n" << R << "\n";
}
*/

void plds_noisy::stepPlant(double newU)
{	

    Vec w = 0.00001*arma::vec(nX, arma::fill::randn);
    Vec v = 0.00001*arma::vec(nY, arma::fill::randn);
    //std::cout << w <<"," << v;

    u = newU;
    x = arma::vectorise(    A*x + B*u  +w  ); //+noise
    y = arma::as_scalar(    C*x + D*u  +v  );
}






























