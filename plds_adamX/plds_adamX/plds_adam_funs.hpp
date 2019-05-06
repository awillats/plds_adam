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


//correct these paths later
//#include "../../../../module_help/eigen/Eigen/Dense"
//#include "../../../../module_help/StAC_rtxi/dataFuns.h" //something weird about using this


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
    
        //void randInit();
        //void printMyParams();
        //void loadParamsFromTxt();
        //void initSys();
        //void stepPlant();
    
    private:
    
    
};


#endif /* plds_adam_funs_hpp */
