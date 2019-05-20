//
//  lds_adam_funs.cpp
//  lds_adamX
//
//  Created by Adam Willats on 4/3/19.
//  Copyright © 2019 Adam Willats. All rights reserved.
//

#include "plds_adam_funs.hpp"

using namespace adam;

void lds_adam::printSys()
{
    std::cout <<"Here is the matrix A:\n" << A << "\n";
    std::cout <<"Here is the matrix B:\n" << B << "\n";
    std::cout <<"Here is the matrix C:\n" << C << "\n";
    std::cout <<"Here is the matrix D:\n" << D << "\n";
}
void lds_adam::loadParamsFromTxt()
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

void lds_adam::resetSys()
{
    //x = Eigen::Vector2d::Zero(nX); //namespace dependent

    x.set_size(nX);    
    x.fill(0);
    y = 0;
    u = 0;
}

void lds_adam::initSys()
{
    loadParamsFromTxt();
    resetSys();
    printSys();
}

void lds_adam::stepPlant(double newU)
{
    u = newU;
    x = arma::vectorise(    A*x + B*u    ); //+noise
    y = arma::as_scalar(    C*x + D*u    );
 
}
void lds_adam::stepPlant(Vec newX, double newU)
{
    x = newX; //allows overriding x at step, as a solution for switching
    stepPlant(newU);
}
void lds_adam::importProps(lds_adam sysIn)
{
	A = sysIn.A;
	B = sysIn.B;
	C = sysIn.C;
	D = sysIn.D;
	std::cout<<"Lds import called";
}



///////////////////////////////////////////////////////////////////////////////////////GLDS_ADAM


void glds_adam::initSys()
{
	lds_adam::initSys();
 		//to do: load these from file
	Q = qmag*Mat(nX,nX,arma::fill::eye);
	R = rmag;
}

void glds_adam::printSys()
{
    lds_adam::printSys();
    std::cout <<"Here is the matrix Q:\n" << Q << "\n";
    std::cout <<"Here is the matrix R:\n" << R << "\n";
}


void glds_adam::stepPlant(double newU)
{	

    Vec w = Q*arma::vec(nX, arma::fill::randn);//should be sqrt(Q)
    Vec v = R*arma::vec(nY, arma::fill::randn);

    u = newU;
    x = arma::vectorise(    A*x + B*u  + w  ); //+noise
    y = arma::as_scalar(    C*x + D*u  + v  );
}

void glds_adam::importProps(glds_adam sysIn)
{
	A = sysIn.A;
	B = sysIn.B;
	C = sysIn.C;
	D = sysIn.D;
	Q = sysIn.Q;
	R = sysIn.R;
	std::cout<<"glds import called";
}



///////////////////////////////////////////////////////////////////////////////////////SLDS_ADAM


void slds::initSys()
{
   std::cout<<"\n DEBUG: SLDS PRINTING\n";

    allSys.push_back(glds_adam());
    allSys.push_back(glds_adam());
    allSys[1].B = allSys[0].B*switchScale; //hardcoded default
    std::cout<<"\n DEBUG: END SLDS PRINTING\n";

   sysPtr = allSys.begin();
   sys_idx=0;
}


void slds::switchSys(int sys_idx_new)
{
	if (sys_idx_new!=sys_idx) 
	{
		if ( ((sys_idx_new+1) > allSys.size()) || (sys_idx_new<0) )
		{
			std::cout<<"\n\n idx violation: "<<sys_idx_new;
			return;
		}
		else
		{
			std::cout<<"\n valid idx: "<<sys_idx_new;

			sysPtr = std::next(allSys.begin(), sys_idx_new); //point to new sys
			importProps(*sysPtr); //switch A,B,C,D

			sys_idx = sys_idx_new; //update 
		} // end if-else

	}//endif
}























