#ifndef plds_obsv_adam_hpp
#define plds_obsv_adam_hpp


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

#include "plds_adam_funs.hpp" // namespace adam 
#include "plds_ctrl_adam.hpp"

//Luenberger observer

//  
class lds_obsv : public lds_adam{
    public:

	adam::data_t ymeas;
//
	//adam::data_t u;
	//adam::Vec x;
	//adam::data_t y;
	adam::Vec K;

	lds_obsv() : lds_adam()
	{
		loadObsvParams();
		std::cout<<"made luenberger"<<x;
		printParams();
	
	};

	void loadObsvParams();
	void printParams();
	void predict(adam::data_t, adam::data_t);


};

/*
//Kalman Filter
class glds_obsv : public glds_adam, lds_ctrl_adam{
};
*/


#endif
