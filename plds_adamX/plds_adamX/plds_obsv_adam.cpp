//
//  plds_obsv_adam.cpp
//  plds_adamX
//
//  Created by Adam Willats on 4/15/19.
//  Copyright © 2019 Adam Willats. All rights reserved.
//

#include "plds_obsv_adam.hpp"

using namespace adam;

void lds_obsv::predict(data_t u_in, data_t ymeas_in)
{
	u = u_in;
	ymeas = ymeas_in;

	x = A*x + B*u - K*(y-ymeas);   //transpose?
	y = arma::as_scalar(  C*x  );

	//std::cout<<"prediction";
}

void lds_obsv::loadObsvParams()
{
	std::string homepath = getenv("HOME");
	std::ifstream myfile;
	myfile.open(homepath+"/RTXI/modules/ss_modules/ss_obsv/params/obsv_params.txt");
    
    if (myfile.good())
    {
        int nX = (int) pullParamNum(myfile); //gets nx
	K = pullParamLine(myfile);
        
    }
    else
    {
        std::cout<<"\ncouldnt find plds params\n";
    }

    myfile.close();
}
void lds_obsv::printParams()
{
	lds_adam::printSys();
	std::cout<<"\nK_obsv:"<<K;
}


///////////////////////////////////////////////////////////


void glds_obsv::loadParams()
{
	//lds_obsv::loadObsvParams();
	P = pmag*arma::eye<Mat>(nX,nX);
	isUpdating = 1;
}

void glds_obsv::predict(data_t u_in, data_t ymeas)
{
	u=u_in;


//PREDICTION STEP
	//deterministic update
   // a priori state estimate
	//x=Ax+Bu;//lds_adam::stepPlant(u_in); 	
   // a priori error covariance
	P = A*P*A.t() + Q;

//UPDATE STEP
   //innovation
	//data_t y_res = ymeas - y;
   //innovation covariance
	Mat S = R + C*P*C.t(); 
   //optimal gain
	K = P*C.t()*S.i();
   // a posteriori updates
	x = A*x + B*u + (K*(ymeas - y))*isUpdating;

   // a post. covar
	Mat I_KC = arma::eye<Mat>(nX,nX) - K*C;
	P = (I_KC) * P;
      //P = (I_KC)*P*(I_KC.t()) + K*R*K.t(); //Joseph form


	y = arma::as_scalar(C*x);
}

void glds_obsv::printParams()
{
	glds_adam::printSys();
}

void glds_obsv::toggleUpdating()
{
	isUpdating = ((isUpdating==1) ? 0 : 1);
}




