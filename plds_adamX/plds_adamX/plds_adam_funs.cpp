//
//  lds_adam_funs.cpp
//  lds_adamX
//
//  Created by Adam Willats on 4/3/19.
//  Copyright © 2019 Adam Willats. All rights reserved.
//

#include "plds_adam_funs.hpp"

using namespace adam;

/*
lds_adam aug_lds::augment4PI(lds_adam sysIn)
{
	sysIn.augments = aug_lds();
	return sysIn;
}
*/


void aug_lds::setAugMap(int nX)
{
	augMap.set_size(nX);
	augMap.fill(AUG_STATE);
	augMap(nX-1) = AUG_INTY;
}

//////////////////////////////////////////////

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
   if (isAug)
   {
	stepPlant(newU,0);//WARNING, simming augmented system without specifying reference!
	std::cout<<"err:";// aug sys w/o ref id:19123 ";
   }
   else
   {

    u = newU;
    x = arma::vectorise(    A*x + B*u    ); //+noise
    y = arma::as_scalar(    C*x + D*u    );


   }
}

//could instead automate augmented systems to request R?
void lds_adam::stepPlant(double newU, double newR)
{ //change this to accept vector input?
    u=newU;
    Vec U; U<<newU<<newR;
    x = arma::vectorise(    A*x + B*U    ); //+noise
    y = arma::as_scalar(    C*x + D*U    );
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
//////////////////////////////////////////////////////////////////////////////

void lds_adam::augment4PI()
{

	isAug=true;
	augments = aug_lds(++nX);
	int endi_ = nX-2;
	int endi = nX-1;
	std::cout<<"\nAugMap:\n"<<augments.augMap;

	A = join_vert(A,C);//err = {y} - r = {Cx} - r
	A.resize(nX,nX);

	//this approach stacks r in U~

	nU++;//need to update this in the map
	B.resize(nX, nU); B(endi,nU-1) = -1;//err = y - {r}
	//B.resize(nX,nU);	

	augments.Br = Vec(nX,1,arma::fill::zeros);
	augments.Br(endi)=-1;

	C.resize(1,nX);
	std::cout<<A;

	D.resize(1,nU);



	x.resize(nX,1);


	//update 

}


///////////////////////////////////////////////////////////////////////////////////////GLDS_ADAM


void glds_adam::initSys()
{
	lds_adam::initSys();
 		//to do: load these from file
	x.set_size(nX);//excessive?
	x.fill(0);
	Q = qmag*Mat(nX,nX,arma::fill::eye);
	R = rmag;
}

void glds_adam::printSys()
{
    lds_adam::printSys();
    std::cout <<"Here is the matrix Q:\n" << Q << "\n";
    std::cout <<"Here is the matrix R:\n" << R << "\n";
}


void glds_adam::stepPlant(data_t newU)
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
	std::cout<<" glds import_called, B:"<<std::endl<<B;
}
///////////////////////////////////////////////////////////////////////////////////////PLDS_ADAM


void plds_adam::setDt(data_t dt_in)
{
	dt=dt_in;
	std::cout<<"dt set to "<<dt;
}
void plds_adam::initSys()
{
	lds_adam::initSys();
	//to do: load these V from file
	x.set_size(nX);//excessive?
	x.fill(0);
	Q = qmag*Mat(nX,nX,arma::fill::eye);
}

void plds_adam::printSys()
{
	lds_adam::printSys();
	std::cout<<"y_nl = exp(Cx+nl_d), nl_d:"<<nl_d<<", dt:"<<dt<<"\n";//filler
}


void plds_adam::calcNL()
{
	//would like to generalize this later. i.e. log(1+exp(x)))
	y_nl = exp(y + nl_d)*dt;// double check this convention
}
void plds_adam::spike()
{
	std::random_device r;
	std::default_random_engine gen(r());//might be able to make this private?
	std::poisson_distribution<int> pDistr(y_nl);
	//std::bernoulli_distribution<int> pDistr(y_nl);
	z = (data_t)pDistr(gen); //generate sample, cast int to double
	//std::cout<<"p:"<<y_nl<<">"<<z<<"\n";//debug only
}
void plds_adam::stepPlant(data_t newU)
{
	u = newU;
        Vec w = Q*arma::vec(nX, arma::fill::randn);//should be sqrt(Q)

    	x = arma::vectorise(    A*x + B*u  + w  ); //+noise
    	y = arma::as_scalar(    C*x + D*u  );
	calcNL(); //updates y_NL
	spike(); //updates z_spk
}
void plds_adam::importProps(plds_adam sysIn)
{
	A = sysIn.A;
	B = sysIn.B;
	C = sysIn.C;
	D = sysIn.D;
	Q = sysIn.Q;

	nl_d = sysIn.nl_d;//replace with more general sys params

	//std::cout<<" plds import_called, B:"<<std::endl<<B;
}

///////////////////////////////////////////////////////////////////////////////////////SLDS_ADAM


void slds::initSys()
{
   std::cout<<"\n DEBUG: SLDS PRINTING\n";


   glds_adam sys0 = glds_adam();
	sys0.x.set_size(nX);//cut?
	sys0.x.fill(0);
   glds_adam sys1 = sys0;
   sys1.B = sys0.B*switchScale;

    allSys.push_back(sys0);
    allSys.push_back(sys1);
    //allSys[1].B = allSys[0].B*switchScale; //hardcoded default
	allSys.push_back(glds_adam());
    std::cout<<"\n DEBUG: END SLDS PRINTING\n";

   sysPtr = allSys.begin();
   sys_idx=0;
}


void slds::switchSys(int sys_idx_new)
{
	 // std::cout<<"|base switch called|"<<allSys.size();
	if (sys_idx_new!=sys_idx) 
	{
		if ( ((sys_idx_new+1) > allSys.size()) || (sys_idx_new<0) )
		{
		//uncomment me soon!//std::cout<<"\n\n idx violation: "<<sys_idx_new;
			return;
		}
		else
		{
		//uncomment me soon!//std::cout<<"\n sys,valid idx: "<<sys_idx_new;

			sysPtr = std::next(allSys.begin(), sys_idx_new); //point to new sys
			importProps(*sysPtr); //switch A,B,C,D

			sys_idx = sys_idx_new; //update 
		} // end if-else

	}//endif
}


///////////////////////////////////////////////////////////////////////////////////////SPLDS_ADAM


void splds::initSys()
{
   std::cout<<"\n DEBUG: S_PLDS PRINTING\n";


   plds_adam sys0 = plds_adam();
	sys0.x.set_size(nX);//cut?
	sys0.x.fill(0);
   plds_adam sys1 = sys0;
   sys1.B = sys0.B*switchScale;

    allSys.push_back(sys0);
    allSys.push_back(sys1);
    std::cout<<"\n DEBUG: END S PLDS PRINTING\n";

   sysPtr = allSys.begin();
   sys_idx=0;
}


void splds::switchSys(int sys_idx_new)
{
	 // std::cout<<"|base switch called|"<<allSys.size();
	if (sys_idx_new!=sys_idx) 
	{
		if ( ((sys_idx_new+1) > allSys.size()) || (sys_idx_new<0) )
		{
		//uncomment me soon!//std::cout<<"\n\n idx violation: "<<sys_idx_new;
			return;
		}
		else
		{
		//uncomment me soon!////std::cout<<"\n sys,valid idx: "<<sys_idx_new;

			sysPtr = std::next(allSys.begin(), sys_idx_new); //point to new sys
			importProps(*sysPtr); //switch A,B,C,D

			sys_idx = sys_idx_new; //update 
		} // end if-else

	}//endif
}

















