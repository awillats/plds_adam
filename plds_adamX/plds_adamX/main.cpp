//
//  main.cpp
//  plds_adamX
//
//  Created by Adam Willats on 4/3/19.
//  Copyright © 2019 Adam Willats. All rights reserved.
//

#include <iostream>
#include "plds_adam_funs.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    plds_adam PA = plds_adam();
    std::cout << PA.nX;
    
    PA.loadParamsFromTxt();
    
    //std::cout << ">>"<<PA.nX<<","<<PA.nU;

    std::cout << "Hello, World!\n";
    return 0;
}
