//
//  plds_ctrl_adam.cpp
//  
//
//  Created by Adam Willats on 4/14/19.
//  Copyright © 2019 Adam Willats. All rights reserved.
//

#include "plds_ctrl_adam.hpp"

using namespace adam;

void lds_ctrl_adam::loadGains()
{
	std::string homepath = getenv("HOME");
	std::ifstream myfile;
	myfile.open(homepath+"/RTXI/modules/ss_modules/ss_ctrl/params/gain_params.txt");

	nX = (int) pullParamNum(myfile); //gets nx
	K = pullParamLine(myfile);
	nbar = pullParamNum(myfile);

	myfile.close();

	isSilent = false;
}

void lds_ctrl_adam::toggleSilent()
{
	isSilent = (isSilent==false); //heh
}

data_t lds_ctrl_adam::calcU(data_t r_in, Vec x_in)
{
	r=r_in;
	x=x_in;

	u = (isSilent ? 0 : arma::as_scalar(    r*nbar - K*x    ));

	return u;
}

void lds_ctrl_adam::printGains()
{
	std::cout<<"\nController has gains K:"<<K<<std::endl;
	std::cout<<"\nnbar :"<<nbar<<std::endl;
}

void lds_ctrl_adam::importProps(lds_ctrl_adam sysIn)
{
	nX = sysIn.nX;
	K = sysIn.K;
	nbar = sysIn.nbar;
	std::cout<<"ctrl import called";
}

void lds_ctrl_adam::importSignals(lds_ctrl_adam sysIn)
{
	x=sysIn.x;
	u=sysIn.u;
	r=sysIn.r;
}


///////

void slds_ctrl::initSys()
{
    allSys.push_back(lds_ctrl_adam());
    allSys.push_back(lds_ctrl_adam());
    allSys[1].K = allSys[0].K/2;//verify

    sysPtr = allSys.begin();
    sys_idx=0;

    lds_ctrl_adam::importProps(*sysPtr);
}


void slds_ctrl::switchSys(int switch_idx)
{
	//slds::switchSys(switch_idx);
	std::cout<<"\n new import incoming:";
	lds_ctrl_adam::importProps(*sysPtr);
}

/*
void slds_ctrl::calcU(data_t r_in, Vec x_in)
{
	//this import business should be unnecessary
	//lds_ctrL_adam::calcU(r_in,x_in);
	(*sysPtr).calcU(r_in, x_in);
	x=(*sysPtr).x;
	u=(*sysPtr).u;
	r=(*sysPtr).r;
}
*/














