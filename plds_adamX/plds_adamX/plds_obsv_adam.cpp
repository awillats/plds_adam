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



///////////////////////////////////////////////////////////   GLDS


void glds_obsv::loadParams()
{
	//lds_obsv::loadObsvParams();
	P = pmag*arma::eye<Mat>(nX,nX);
	isUpdating = 1;
}

void glds_obsv::predict(data_t u_in, data_t ymeas)
{
	u=u_in;
	Mat I = arma::eye<Mat>(nX,nX);

//PREDICTION STEP
   // a priori error covariance
	P = A*P*A.t() + Q;
//shouldn't be necessary
	/*
	if (arma::norm(P)>1)
	{
		std::cout<<"danger";
		//sP = Q;
		loadParams();
	}
	if (arma::norm(P)<1e-10)
	{
		std::cout<<",";//P,silent";
		loadParams();
		//P=Q;
	}
	*/
//UPDATE STEP
   //innovation covariance
	Mat S = R + C*P*C.t(); 
   //optimal gain
	K = P*C.t()*S.i();
	//K.fill(1);//override
   // a posteriori updates
	x = A*x + B*u - (K*(y-ymeas))*isUpdating;

   // a post. covar
	P = (I-K*C) * P;

	y = arma::as_scalar(C*x);
}
void glds_obsv::resetSys()
{
	lds_adam::resetSys();
	loadParams();
	
	//P.fill(0);
	std::cout<<"zerod P,"<<pmag;
}
void glds_obsv::printParams()
{
	glds_adam::printSys();
	std::cout<<"\nKF print ended,new\n";
}

void glds_obsv::toggleUpdating()
{
	isUpdating = ((isUpdating==1) ? 0 : 1);
	std::cout<<"KF toggle_newx";
}

void glds_obsv::importProps(glds_obsv sysIn)
{
	//glds_adam::importProps()
	A = sysIn.A;
	B = sysIn.B;
	C = sysIn.C;
	D = sysIn.D;
	Q = sysIn.Q;
	R = sysIn.R;

	P = sysIn.P;
}

void glds_obsv::importSignals(glds_obsv sysIn)
{
	x=sysIn.x;
	y=sysIn.y;
}


///////////////////////////////////////////////////////////   PLDS


void plds_obsv::loadParams()
{
	//lds_obsv::loadObsvParams();
	P = pmag*arma::eye<Mat>(nX,nX);
	isUpdating = 1;
}

void plds_obsv::predict(data_t u_in, data_t spike_meas)
{
	u=u_in;
	calcNL();//update y_nl
//replace inv with inv_sympd

	P = A*P*A.t() + Q;//verify
	P = arma::inv_sympd(P.i() + C.t()*y_nl*C);//posterior covariance
	x = A*x + B*u - P*C.t() * (y_nl-spike_meas)*isUpdating; //posterior mean//
	y = arma::as_scalar(  C*x  );

	calcNL();
	spike();
}
void plds_obsv::resetSys()
{
	lds_adam::resetSys();
	loadParams();
	
	std::cout<<"reset P,"<<pmag;
}
void plds_obsv::printParams()
{
	plds_adam::printSys();
	std::cout<<"\nPPF print ended,new\n";
}

void plds_obsv::toggleUpdating()
{
	isUpdating = ((isUpdating==1) ? 0 : 1);
	std::cout<<"PPF toggle_newx";
}

void plds_obsv::importProps(plds_obsv sysIn)
{
	A = sysIn.A;
	B = sysIn.B;
	C = sysIn.C;
	D = sysIn.D;
	Q = sysIn.Q;
	nl_d = sysIn.nl_d;

	//R = sysIn.R;

	P = sysIn.P;
}

void plds_obsv::importSignals(plds_obsv sysIn)
{
	x=sysIn.x;
	y=sysIn.y;
	y_nl = sysIn.y_nl;
	z = sysIn.z;
}



////////////////////////////////////////////////////    Switch GLDS


void s_glds_obsv::initSys()
{
   std::cout<<"\n DEBUG: SKF PRINTING\n";


   glds_obsv obsv0 = glds_obsv();
	//obsv0.x.set_size(glds_adam::nX);
	//obsv0.x.fill(0);
   glds_obsv obsv1 = obsv0;
	obsv1.B = obsv0.B*switchScale;

    allSys.push_back(obsv0);
    allSys.push_back(obsv1);
    //allSys[1].B = allSys[0].B*switchScale; //hardcoded default
    std::cout<<"\n DEBUG: END SKF PRINTING\n";

   sysPtr = allSys.begin();
   sys_idx=0;

   glds_obsv::importProps(*sysPtr);
   glds_obsv::importSignals(*sysPtr);
  
   x.set_size(obsv0.nX);//has to be more elegant way to do this...
   x.fill(0);//
   y=arma::as_scalar(obsv0.C*x);
}


//unfortunately this is copy-pasted from slds::switchSys. there has to be a more elegant way to accomplish this
void s_glds_obsv::switchSys_inner(int sys_idx_new)
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
			std::cout<<"\n obsv,valid idx: "<<sys_idx_new;

			sysPtr = std::next(allSys.begin(), sys_idx_new); //point to new sys
			//slds_ctrl::importProps(*sysPtr); //switch A,B,C,D

			sys_idx = sys_idx_new; //update 
		} // end if-else
		//for debug//std::cout<<"\n\n switch obsv, B:\n"<<(*sysPtr).B;

	}//endif
}

void s_glds_obsv::switchSys(int sys_idx_new)
{
	//should be done more elegantly...
	switchSys_inner(sys_idx_new);
	glds_obsv::importProps(*sysPtr);
//toggle these two lines of code to reintroduce switching transients???
	(*sysPtr).x=x;
	(*sysPtr).y=y;
	
}


void s_glds_obsv::resetSys()
{
	(*sysPtr).glds_obsv::resetSys();
	x = (*sysPtr).x;
	y= (*sysPtr).y;
}

void s_glds_obsv::predict(data_t u_in, data_t ymeas)
{
//Note this method of gating observations is inefficient and code be handled more elegantly.
//more direct class inheritance of the "isUpdating" tag is what's needed here
	
	if (isUpdating) { (*sysPtr).predict(u_in,ymeas); }
	else { (*sysPtr).lds_adam::stepPlant(u_in); }
//also note that this bypasses any updates in the kalman filter algo. so P stays constant
	x = (*sysPtr).x;
	y= (*sysPtr).y;

}





////////////////////////////////////////////////////    Switch PoissonLDS


void s_plds_obsv::initSys()
{
   std::cout<<"\n DEBUG: SKF PRINTING\n";


   plds_obsv obsv0 = plds_obsv();
   plds_obsv obsv1 = obsv0;
	obsv1.B = obsv0.B*switchScale;

    allSys.push_back(obsv0);
    allSys.push_back(obsv1);
    //allSys[1].B = allSys[0].B*switchScale; //hardcoded default
    std::cout<<"\n DEBUG: END SKF PRINTING\n";

   sysPtr = allSys.begin();
   sys_idx=0;

   plds_obsv::importProps(*sysPtr);
   plds_obsv::importSignals(*sysPtr);
  
   x.set_size(obsv0.nX);//has to be more elegant way to do this...
   x.fill(0);//
   y=arma::as_scalar(obsv0.C*x);
}


//unfortunately this is copy-pasted from slds::switchSys. there has to be a more elegant way to accomplish this
void s_plds_obsv::switchSys_inner(int sys_idx_new)
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
			std::cout<<"\n obsv,valid idx: "<<sys_idx_new;

			sysPtr = std::next(allSys.begin(), sys_idx_new); //point to new sys
			//slds_ctrl::importProps(*sysPtr); //switch A,B,C,D

			sys_idx = sys_idx_new; //update 
		} // end if-else
		

	}//endif
}

void s_plds_obsv::switchSys(int sys_idx_new)
{
	//should be done more elegantly...
	switchSys_inner(sys_idx_new);
	plds_obsv::importProps(*sysPtr);
//toggle these two lines of code to reintroduce switching transients???
	(*sysPtr).x=x;
	(*sysPtr).y=y;
	(*sysPtr).y_nl = y_nl;
	(*sysPtr).z = z;
	
}


void s_plds_obsv::resetSys()
{
	(*sysPtr).plds_obsv::resetSys();
	plds_obsv::importSignals();//verify that this is sufficient
/*
	x = (*sysPtr).x;
	y= (*sysPtr).y;
	y_nl = (*sysPtr).y_nl;
	z = (*sysPtr).z;
*/
}

void s_plds_obsv::predict(data_t u_in, data_t spike_meas)
{
//Note this method of gating observations is inefficient and code be handled more elegantly.
//more direct class inheritance of the "isUpdating" tag is what's needed here
	
	if (isUpdating) { (*sysPtr).predict(u_in,spike_meas); }
	else { (*sysPtr).lds_adam::stepPlant(u_in); }
//also note that this bypasses any updates in the kalman filter algo. so P stays constant
	
	plds_obsv::importSignals();
/*
	x = (*sysPtr).x;
	y= (*sysPtr).y;
	y_nl = (*sysPtr).y_nl;
	z = (*sysPtr).z;
*/
}






























