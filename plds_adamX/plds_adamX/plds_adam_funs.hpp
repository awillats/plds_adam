//
//  plds_adam_funs.hpp
//  plds_adamX
//
//  Created by Adam Willats on 4/3/19.
//  Copyright © 2019 Adam Willats. All rights reserved.
//

#ifndef plds_adam_funs_hpp
#define plds_adam_funs_hpp

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

class plds_adam{
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
    
        plds_adam(): nX(2), nU(1) , nY(1){ initSys();};
    
        //plds_adam(): nX(2), nU(1) , nY(1){ ;};

        void printSys();
        void loadParamsFromTxt();
        void resetSys();
        void initSys();


        void stepPlant(double);
	void stepPlant(adam::Vec, double);    

        //void randInit();
    
    private:   
};

class glds_adam : public plds_adam{
    private:
	//std::random_device rd; 
    	//std::mt19937 gen; 

    public:

	double qmag;
	double rmag;
	adam::Mat Q;
	adam::Mat R;

         glds_adam(): plds_adam(), qmag(2e-2), rmag(1e-3)
	{
	    initSys();
	};
	void initSys();
	void printSys();
	void stepPlant(double);
};


#endif /* plds_adam_funs_hpp */








