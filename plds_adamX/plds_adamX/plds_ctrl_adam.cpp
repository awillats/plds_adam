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
	//std::cout<<"\nctrl import called";
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
    allSys[1].K = allSys[0].K/switchScale;//verify this is what we want

    sysPtr = allSys.begin();
    sys_idx=0;

    lds_ctrl_adam::importProps(*sysPtr);
}


//unfortunately this is copy-pasted from slds::switchSys. there has to be a more elegant way to accomplish this
void slds_ctrl::switchSys_inner(int sys_idx_new)
{
	  //std::cout<<"|inner switch called|"<<allSys.size();
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
			//slds_ctrl::importProps(*sysPtr); //switch A,B,C,D

			sys_idx = sys_idx_new; //update 
		} // end if-else

	}//endif
}



void slds_ctrl::switchSys(int switch_idx)
{
	  //std::cout<<"|ctrl switch called|";//debug outptus
	  //std::cout<<"$a"<<(*sysPtr).K;//debug outputs

	switchSys_inner(switch_idx);
	//slds::switchSys(switch_idx); //other way of switchSys code reuse

	//std::cout<<"$b"<<(*sysPtr).K; //debug outputs
	//std::cout<<"\n new import incoming:"; //debug outputs
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














