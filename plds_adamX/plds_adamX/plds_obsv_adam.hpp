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

//#include <dynCtrlEst>

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
	void importSignals(glds_obsv sysIn);
};
//should be able to do a better job of sharing code between glds_obsv and plds_obsv
class plds_obsv : public plds_adam{
    private:
	adam::data_t pmag;
    public:
	adam::data_t ymeas;
	adam::Vec K;
	adam::Mat P; //covar of estimate

	int isUpdating;

	plds_obsv() : plds_adam(), pmag(2)
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

	void importProps(plds_obsv sysIn);
	void importSignals(plds_obsv sysIn);
	
};

class s_glds_obsv : public slds, public glds_obsv{
    private:
	void switchSys_inner(int);	
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

	void switchSys(int);

};

//yes, definitely lots of code shared with s_glds_obsv, must be a more efficient way to write these

class s_plds_obsv : public slds, public plds_obsv{
    private:
	void switchSys_inner(int);	
    public:
	//only need to override init methods!
	
	std::vector<plds_obsv> allSys;
	std::vector<plds_obsv>::iterator sysPtr;
	//int sys_idx;

	adam::Vec x;
	adam::data_t y;
	adam::data_t y_nl;
	adam::data_t z;

//:slds()
	s_plds_obsv() 
	{
		initSys();
	}
	void resetSys();
	void initSys();
	void predict(adam::data_t, adam::data_t);
	void switchSys(int);
};


#endif
