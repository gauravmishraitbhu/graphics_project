//
//  Utils.cpp
//  Project
//
//  Created by Gaurav Mishra on 17/10/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#include "Utils.hpp"
#include <iostream>
#include <vector>
#include "Line.hpp"

using namespace std;


void assignParallelClass(vector<Line *> lines){
    for (Line *line : lines){
        cout << line->getAngle() << endl;
    }
    
    
}
