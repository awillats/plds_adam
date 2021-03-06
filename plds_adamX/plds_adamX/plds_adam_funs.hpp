//
//  lds_adam_funs.hpp
//  lds_adamX
//
//  Created by Adam Willats on 4/3/19.
//  Copyright © 2019 Adam Willats. All rights reserved.
//

#ifndef lds_adam_funs_hpp
#define lds_adam_funs_hpp

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <random>

#include <math.h> //exp


//from module_help
#include <eigen/Eigen/Dense>
#include <StAC_rtxi/dataFuns.h>

#include <dynCtrlEst>


namespace adam{
	typedef double data_t;
	typedef std::vector<data_t> stdVec;
	typedef arma::Row<data_t> RowVec;
	typedef arma::Col<data_t> Vec;
	typedef arma::Mat<data_t> Mat;
	typedef arma::subview_col<data_t> SubVec;
	typedef arma::subview<data_t> SubMat;

	static const int AUG_STATE = 0x1;
	static const int AUG_INTY = 0x2;

}

namespace adam_e{
	typedef double data_t;
	typedef std::vector<data_t> stdVec;
	typedef Eigen::RowVector2d RowVec;
	typedef Eigen::Vector2d Vec;
	typedef Eigen::Matrix2d Mat; //generalize to Xd
	//typedef arma::subview_col<data_t> SubVec;
//	typedef arma::subview<data_t> SubMat;
}



///////
class lds_adam;

class aug_lds{
    public:


	aug_lds(int nX)
	{
		setAugMap(nX);	
		std::cout<<"augmented system!";
	}
	aug_lds() { aug_lds(2); }
	
	arma::Col<int> augMap;
	adam::Vec Br;

	void setAugMap(int);
	//static lds_adam augment4PI(lds_adam);
};

class lds_adam{
    //vars
    //empty constructor
    //real constructor 1
    
    public:
    
        int nX;
        int nU;
        int nY;

    
        adam::Mat A; //Eigen::Matrix2d A;
        adam::Mat B; //Eigen::Vector2d B; //B shoudl be a matrix!!!
        adam::RowVec C; //Eigen::RowVector2d C; 
        adam::RowVec D; 
    
        adam::Vec x;
        adam::data_t y;
        adam::data_t u;
    
        adam::data_t dt;
    
	bool isAug;
	aug_lds augments;



        lds_adam(): nX(2), nU(1) , nY(1), isAug(false){ initSys();};
    
        //lds_adam(): nX(2), nU(1) , nY(1){ ;};

        void printSys();
        void loadParamsFromTxt();
        void resetSys();
        void initSys();


        void stepPlant(double);
	void stepPlant(adam::Vec, double);  
	void stepPlant(double, double);//for augmented systems
	void importProps(lds_adam);

        //void randInit();
	void augment4PI();
	//friend lds_adam aug_lds::augment4PI(lds_adam);
        
    private:   
};


//To-do: Specify observation classes? like gaussian, poisson, have GLDS inherit






////////////////////////////////////////////////////////////////////////////// GLDS CLASSDEF


class glds_adam : public lds_adam{
    private:
	//std::random_device rd; 
    	//std::mt19937 gen; 

    public:
	//A,B,C,D
	double qmag;
	double rmag;
	adam::Mat Q;
	adam::Mat R;

         glds_adam(): lds_adam(), qmag(1e-2), rmag(1e-1)//rmag=1e-1 
	{
	    initSys();
	};
	void initSys();
	void printSys();
	void stepPlant(adam::data_t);
	void importProps(glds_adam);

}; //end of glds


////////////////////////////////////////////////////////////////////////////// PLDS CLASSDEF
class plds_adam: public lds_adam{
    private:
	
    public:
adam::data_t dt;//should be private later
	//A,B,C,D
	adam::data_t qmag;
	adam::Mat Q;

	adam::data_t nl_d;//yNL = exp(Cx+nl_d);

	adam::data_t y_nl;
	adam::data_t z;//should be an integer?

	plds_adam(): lds_adam(), qmag(1e-2), nl_d(2)//10 is too big?-1 too small, 2 slightly too big
	{
//adam::data_t dt_in
		dt = 1e-3;//dt_in; //ehh
		initSys();
	}
	void setDt(adam::data_t);

	void initSys();
	void printSys();

	void calcNL();
	void spike();
	void stepPlant(adam::data_t);
	void importProps(plds_adam);

}; //end of plds

////////////////////////////////////////////////////////////////////////////// SLDS CLASSDEF

//Switched linear dynamical systems ("vector of sys" approach. should also consider tensor-valued properties approach)

class slds : public glds_adam{
    private:
	//void appendSys();
	//have a family of push/pop options??

    public:
	
	std::vector<glds_adam> allSys;
	std::vector<glds_adam>::iterator sysPtr;
	int sys_idx;
	double switchScale;

	slds(): glds_adam(), switchScale(2)
	{
		initSys();
	};

	void initSys();
	void switchSys(int);
}; 

/*
class slds_tensor : public lds_adam{
    private:
	void appendSys();
    public:
	void switchPlant();
}
*/


class splds : public plds_adam{
    private:
	//void appendSys();
	//have a family of push/pop options??

    public:
	
	std::vector<plds_adam> allSys;
	std::vector<plds_adam>::iterator sysPtr;
	int sys_idx;
	double switchScale;

	splds(): plds_adam(), switchScale(2)
	{
		initSys();
	};

	void initSys();
	void switchSys(int);
}; 



#endif /* lds_adam_funs_hpp */








