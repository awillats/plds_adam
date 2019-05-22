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

class glds_obsv : public glds_adam{
    private:
	adam::data_t pmag;
    public:
	adam::data_t ymeas;
	adam::Vec K;
	adam::Mat P; //covar of estimate

	int isUpdating;

	glds_obsv() : glds_adam(), pmag(2)//default pmag = 1e-4
	{
		loadParams();
		printParams();	
	}

	void resetSys();

	void loadParams();
	void predict(adam::data_t, adam::data_t);
	void update();
	void printParams();
	void toggleUpdating();
	void importProps(glds_obsv sysIn);
};


class s_glds_obsv : public slds, public glds_obsv{
    public:
	//only need to override init methods!
	
	std::vector<glds_obsv> allSys;
	std::vector<glds_obsv>::iterator sysPtr;
	//int sys_idx;

	adam::Vec x;
	adam::data_t y;

//:slds()
	s_glds_obsv() 
	{
		initSys();
	}
	void resetSys();
	void initSys();
	void predict(adam::data_t, adam::data_t);
	//void switchSys(int);

};



#endif
