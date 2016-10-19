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
#include <algorithm>
#include <math.h>
#include "Line.hpp"

using namespace std;
const int ANGLE_THRESHOLD = 10;

bool sortFunction(Line *a , Line *b){
    return a->getAngle() < b->getAngle();
}


void assignParallelClass(vector<Line *> lines){
    
    
    for (Line *line : lines){
        cout << line->getAngle() << endl;
    }
    
    cout << "sorting..."<<endl;
    // we will sort the angles vertor.
    std::sort( lines.begin() , lines.end() , sortFunction);
    
    for (Line * line : lines){
        cout << line->getAngle() << endl;
    }
    
    // now start assinging classnumbers and colors
    float currClassAngle = -999;
    int currClassNum = 0;
    float red = 0.0 ;float green = 0.0; float blue = 0.0;
    for (Line * line : lines){
        float angle = line->getAngle();
        // either this is first class or the diff of angles is high
        // we begin a new class and start assigning following lines to new class
        if( currClassAngle == -999 || angle - currClassAngle > ANGLE_THRESHOLD){
            currClassNum += 1;
            currClassAngle = angle;
            red = ((float) rand() / (RAND_MAX)) ;
            green = ((float) rand() / (RAND_MAX)) ;
            blue = ((float) rand() / (RAND_MAX)) ;
            
        }
        
        line->assignParallelClass(currClassNum);
        line->assignColors(red, green, blue);
    }
}

void fixSketch(vector <Line *> lines){
    for (int i = 0 ; i < lines.size() ; i++){
        
        // if both the vertex of line have been corrected then move on
        Line *line1 = lines.at(i);
        
        if(line1 ->vertex1Corrected() && line1->vertex2Corrected() ){
            continue;
        }
        
        for(int j = i+1 ; j < lines.size() ; j++){
            // check vertex1 of a line and find its nearest matches
            int x = line1->getV1X();
            int y = line1->getV1Y();
            
            Line *line2 = lines.at(j);
            line2->findAndFixVertex(x, y);
            
            // now check vertex2 of line1
            x = line1->getV2X();
            y = line1->getV2Y();
            line2->findAndFixVertex(x, y);
        }
    }
}


float getCartesianDistance(int x1 , int y1 , int x2 , int y2){
    return sqrt( pow( x1 - x2 , 2 ) + pow( y1 - y2 , 2) );
}



