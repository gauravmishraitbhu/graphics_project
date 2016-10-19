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
    int _isVertex1Corrected = 0 , _isVertex2Corrected = 0;
    
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
    
    int vertex1Corrected(){
        return _isVertex1Corrected;
    }
    
    int vertex2Corrected(){
        return _isVertex2Corrected;
    }
    
    int getV1X(){
        return _startX;
    }
    
    int getV1Y(){
        return _startY;
    }
    
    int getV2X(){
        return _endX;
    }
    
    int getV2Y(){
        return _endY;
    }
    
    void draw();
    void updateEndPoints(int newEndX , int newEndY);
    float getLength();
    float getAngle();
    int getObjectType();
    void assignParallelClass(int classNum);
    void assignColors(float r , float g , float b);
    
    // given a point this function will look at both vertex of line
    // if any of the vertex lies very close to given point it will move
    // that vertex to given point.
    void findAndFixVertex(int x , int y);
    
    // will set a convention that if angle of line is between -45 and 45
    // then endX > startX else endY > startY
    void uniformifyVertices();
    
};

#endif /* Line_hpp */
