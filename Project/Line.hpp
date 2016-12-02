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
#include "Utils.hpp"
#include "DrawableObject.hpp"
#include "Point2D.hpp"

class Curve2D;
class Curve3D;
class Line : public DrawableObject{
    
private:
    
    /* start and end coordinates of Line segment */
    int _startX , _startY;
    int _endX , _endY;
    int _classNum ;
    float _colorR = 0, _colorG = 0 , _colorB = 0;
    int _isVertex1Corrected = 0 , _isVertex2Corrected = 0;
    
    /* 1 indexed ids assinged to each vertex in the graph */
    int _vertex1Id = -1 , _vertex2Id = -1;
    
    
    /* will be set for the lines which were relacements for curves during pre processing */
    bool _isProxy = false;
    Curve2D *originalCurve;
    Curve3D *reconstructedCurve;
    
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
    
    // this constructor will be used to create proxy lines
    Line(int startX , int startY , int endX , int endY , Curve2D *curve){
        
        _startX = startX;
        _startY = startY;
        _endX = endX;
        _endY = endY;
        originalCurve = curve;
        _isProxy = true;
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
    
    Point2D getVertex1(){
        return Point2D{_startX , _startY};
    }
    
    int getV2X(){
        return _endX;
    }
    
    int getV2Y(){
        return _endY;
    }
    
    Point2D getVertex2(){
        return Point2D{_endX , _endY};
    }
    
    int getVertex1Id(){
        return _vertex1Id;
    }
    
    void setVertex1Id(int id){
        _vertex1Id = id;
    }
    
    int getVertex2Id(){
        return _vertex2Id;
    }
    
    void setVertex2Id(int id){
        _vertex2Id = id;
    }
    
    int getParallelClassNum(){
        return _classNum;
    }
    
    bool isProxy(){
        return _isProxy;
    }
    
    Curve2D *getOriginalCurve(){
        return originalCurve;
    }
    
    void setReconstructedCurve(Curve3D *curve){
        reconstructedCurve = curve;
    }
    
    Curve3D *getReconstructedCurve(){
        return reconstructedCurve;
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
    
    // make sure that this line and given line is alinged in
    // same direction
    void alignLineDirection(Line* line);
  
    //given a vertex id reeturns checks if
    // given id belongs to this line if yes returns
    // the other vertexid
    int getNeighborVertexId(int id);
};

#endif /* Line_hpp */
