//
//  Line.hpp
//  Project
//
//  Created by Gaurav Mishra on 28/09/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef Line_hpp
#define Line_hpp

#include <stdio.h>
#include "DrawableObject.hpp"
class Line : public DrawableObject{
    
private:
    
    /* start and end coordinates of Line segment */
    int _startX , _startY;
    int _endX , _endY;
    int _classNum ;
    float _colorR , _colorG , _colorB;
    
public:
    Line(){
        _startX = 0;
        _startY = 0;
        _endX = 0;
        _endY = 0;
    }
    
    Line(int startX , int startY){
        _startX = startX;
        _startY = startY;
       
    }

    
    Line(int startX , int startY , int endX , int endY){
        _startX = startX;
        _startY = startY;
        _endX = endX;
        _endY = endY;
    }
    
    void draw();
    void updateEndPoints(int newEndX , int newEndY);
    float getLength();
    float getAngle();
    int getObjectType();
    void assignParallelClass(int classNum);
    void assignColors(float r , float g , float b);
    
};

#endif /* Line_hpp */
