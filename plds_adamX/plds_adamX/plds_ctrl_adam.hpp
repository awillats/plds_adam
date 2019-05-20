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

	bool isSilent;
        adam::data_t r;
	adam::data_t u;
	adam::Vec x;

	adam::RowVec K;
	adam::data_t nbar;

	lds_ctrl_adam(): nX(2), nU(1) , nY(1){ loadGains();};

	
	void loadGains();
	void toggleSilent();
	double calcU(adam::data_t, adam::Vec);
	void printGains();

	void importProps(lds_ctrl_adam);
	void importSignals(lds_ctrl_adam);
};




///////////////////////////////////////////////


class slds_ctrl : public slds, public lds_ctrl_adam{
    public:
	//only need to override init methods!
	
	std::vector<lds_ctrl_adam> allSys;
	std::vector<lds_ctrl_adam>::iterator sysPtr;
	//int sys_idx;

	adam::Vec x;
	adam::data_t y;

//:slds()
	slds_ctrl() 
	{
		initSys();
	}
								void switchSys(int);
	//void resetSys();
	void initSys();
	//void calcU(adam::data_t, adam::data_t);
	

};


























#endif
