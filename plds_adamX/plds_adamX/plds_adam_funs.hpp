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
#include "../../../../module_help/eigen/Eigen/Dense"


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
        float D;
    
        Eigen::Vector2d x;
        float y;
    
        double dt;
    
    
        plds_adam(): nX(2), nU(1) , nY(1){ ;};

    
    
    
    
        //void printMyParams();
        //void initSys();
        //void stepPlant();
    
    private:
    
    
};


#endif /* plds_adam_funs_hpp */
