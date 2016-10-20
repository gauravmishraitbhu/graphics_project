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
#include "Point2D.hpp"
#include "PMatrix.hpp"

using namespace std;
const int ANGLE_THRESHOLD = 10;

bool sortFunction(Line *a , Line *b){
    return a->getAngle() < b->getAngle();
}


int assignParallelClass(vector<Line *> lines){
    
    
//    for (Line *line : lines){
////        cout << line->getAngle() << endl;
//        cout << "-----"<<endl;
//        cout << line->getVertex1().x << ","<<line->getVertex1().y<<endl;
//        cout << line->getVertex2().x << ","<<line->getVertex2().y<<endl;
//    }
    
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
    
    return currClassNum;
}


/**
 Given a set of lines which represent a graph. this method finds small gaps in the sketch
 and fix them my moving some vertices
 */
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


int getAssignedId(vector<Point2D> verticesWithId , Point2D  vertexToCheck){
    
    for (int i = 0 ; i < verticesWithId.size() ; i++){
        Point2D point = verticesWithId.at(i);
        if(point.x == vertexToCheck.x && point.y == vertexToCheck.y){
            return i + 1;
        }
    }
    
    return -1;
}

/**
 Assing a unique Id to each vertex in the graph.
 */
vector<Point2D> assignVertexIds(vector<Line*> lines){
    
    // to keep track of which vertices has already been assinged an id
    vector<Point2D> assingnedVertices;
    
    for(Line * line : lines){
        //check vertex1
        Point2D vertex1 = line->getVertex1();
        int vertex1Id = getAssignedId(assingnedVertices , vertex1);
        if( vertex1Id == -1 ){
            // if not assinged then assign one id
            int nextId = int(assingnedVertices.size()) + 1;
            line->setVertex1Id(nextId);
            
            assingnedVertices.push_back(vertex1);
        }else{
            line->setVertex1Id(vertex1Id);
        }
        
        //do the same thing for vertex2
        Point2D vertex2 = line->getVertex2();
        int vertex2Id = getAssignedId(assingnedVertices , vertex2);
        
        if( vertex2Id == -1 ){
            // if not assinged then assign one id
            int nextId = int(assingnedVertices.size()) + 1;
            line->setVertex2Id(nextId);
            
            assingnedVertices.push_back(vertex2);
        }else{
            line->setVertex2Id(vertex2Id);
        }
    }
    
    
    for(Line* line : lines){
        cout <<"Class= "<< line->getParallelClassNum() << endl;
        cout << "Angle=="<<line->getAngle() << endl;
        cout << line->getVertex1Id() << "   " << line->getVertex2Id() << endl;
    }
    
    return assingnedVertices;
}

vector<Line *> getAllLinesOfClass(vector<Line *> lines , int classNum){
    vector<Line *> filteredList;
    for (Line *line : lines){
        if(line->getParallelClassNum() == classNum){
            filteredList.push_back(line);
        }
    }
    return filteredList;
}


/* make sure all the lines in a class points in same direction*/
void fixLineDirections(vector<Line *> lines , int numClasses){
    
    for(int i = 1 ; i <= numClasses ; i++){
        vector<Line *> filteredList = getAllLinesOfClass(lines, i);
        Line *lineToCompare = filteredList.at(0);
        
        if(filteredList.size() < 2){
            continue;
        }
        // compare first line with every line in the class
        // and make sure that dot product is positve
        for (int j = 1 ; j < filteredList.size() ; j++){
            Line *line2 = filteredList.at(j);
            line2->alignLineDirection(lineToCompare);
        }
    }
    
}

void createPMatrix( PMatrix * matrix , vector<Line*> lines , int numClasses){
    float** data = matrix->getData();
    int numRows = matrix->getNumRows();
    int numCols = matrix->getNumCols();
    
    // layout of the matrix is like this
    // each row will represent a line
    // each col will correspond to either class coefficient or vertex
    // first 'k' cols are for classes following n-1 are for vertices
    int currentRow = 0;
    for(Line * line : lines){
        // for each lines we will have equation of form
        // vertex1Z - vertex2Z - class1 * length = 0
        // hence coefficients will be 1 , -1 , -length
        // -length will go into line->classNum - 1
        // 1 will go into numClasses + vertexId - 2 (-2 because vertexClass is 1 based not 0 based)
        // -1 will go into numCLasses + vertex2Id - 2
        
        
        int vertex1Id = line->getVertex1Id();
        // lets place 1 into the matrix at its location
        if(vertex1Id != 1){
            data[currentRow][numClasses + vertex1Id - 2] = 1;
        }
        
        
        int vertex2Id = line->getVertex2Id();
        //lets place -1
        if(vertex2Id != -1){
            data[currentRow][numClasses + vertex2Id - 2] = -1;
        }
        
        // for -length
        float length = line->getLength();
        data[currentRow][line->getParallelClassNum() - 1] = -1 * length;
        
        
        currentRow ++;
    }
    
}


