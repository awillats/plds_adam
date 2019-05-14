//Adam Willats, awilats3@gatech.edu
//
//
//
#ifndef lds_ctrl_adam_hpp
#define lds_ctrl_adam_hpp


#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "plds_adam_funs.hpp"


//from module_help
#include <eigen/Eigen/Dense>
#include <StAC_rtxi/dataFuns.h>

#include <dynCtrlEst>


class lds_ctrl_adam {
    public:
	int nX;
        int nU;
        int nY;

        adam::data_t r;
	adam::data_t u;
	adam::Vec x;

	adam::RowVec K;
	adam::data_t nbar;

	lds_ctrl_adam(): nX(2), nU(1) , nY(1){ loadGains();};

	void loadGains();
	double calcU(adam::data_t, adam::Vec);
	void printGains();
};






























#endif
