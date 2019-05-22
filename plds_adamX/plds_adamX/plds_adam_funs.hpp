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

class lds_adam{
    //vars
    //empty constructor
    //real constructor 1
    
    public:
    
        int nX;
        int nU;
        int nY;

    
        adam::Mat A; //Eigen::Matrix2d A;
        adam::Vec B; //Eigen::Vector2d B;
        adam::RowVec C; //Eigen::RowVector2d C; 
        adam::data_t D; 
    
        adam::Vec x;
        adam::data_t y;
        adam::data_t u;
    
        adam::data_t dt;
    
        lds_adam(): nX(2), nU(1) , nY(1){ initSys();};
    
        //lds_adam(): nX(2), nU(1) , nY(1){ ;};

        void printSys();
        void loadParamsFromTxt();
        void resetSys();
        void initSys();


        void stepPlant(double);
	void stepPlant(adam::Vec, double);  
	void importProps(lds_adam);

        //void randInit();
    
    private:   
};


////////////////////////////////////////////////////////////////////////////// GLDS CLASSDEF


class glds_adam : public lds_adam{
    private:
	//std::random_device rd; 
    	//std::mt19937 gen; 

    public:

	double qmag;
	double rmag;
	adam::Mat Q;
	adam::Mat R;

         glds_adam(): lds_adam(), qmag(1e-2), rmag(1e-1)
	{
	    initSys();
	};
	void initSys();
	void printSys();
	void stepPlant(double);
	void importProps(glds_adam);

}; //end of glds


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

	slds(): glds_adam(), switchScale(1.4)
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
#endif /* lds_adam_funs_hpp */








