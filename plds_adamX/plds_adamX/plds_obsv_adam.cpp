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

