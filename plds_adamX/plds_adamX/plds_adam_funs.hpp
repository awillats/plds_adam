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


//from module_help
#include <eigen/Eigen/Dense>
#include <StAC_rtxi/dataFuns.h>


class plds_adam{
    //vars
    //empty constructor
    //real constructor 1
    
    public:
    
        int nX;
        int nU;
        int nY;

    
        Eigen::Matrix2d A;
        Eigen::Vector2d B;
        Eigen::RowVector2d C; //generalize later
        double D; //why are these floats not doubles?
    
        Eigen::Vector2d x;
        double y;
        double u;
    
        double dt;
    
        plds_adam(): nX(2), nU(1) , nY(1){ initSys();};
    
        //plds_adam(): nX(2), nU(1) , nY(1){ ;};

        void printSys();
        void loadParamsFromTxt();
        void resetSys();
        void initSys();


        void stepPlant(double);
	void stepPlant(Eigen::Vector2d, double);    

        //void randInit();
    
    private:   
};

class plds_noisy : public plds_adam{
    public:

	double sigma;

         plds_noisy(): plds_adam(), sigma(0.1) {initSys();};
        // plds_noisy(double sig):  nX(2), nU(1) , nY(1), sigma(sig) {initSys();};
	
};


#endif /* plds_adam_funs_hpp */
