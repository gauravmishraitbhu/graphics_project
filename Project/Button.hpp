//
//  Button.hpp
//  Project
//
//  Created by Gaurav Mishra on 17/10/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef Button_hpp
#define Button_hpp

#include <stdio.h>
#include "DrawableObject.hpp"
#include <iostream>
using namespace std;
class Button : public DrawableObject{
    
private:
    
    /* start and end coordinates of Line segment */
    int _startX , _startY;
    int _width , _height;
    int _highlighted;
    int _fontX , _fontY;
    char* _label;
    int _state = 0 ;
    
    /* callback function */
    void (*clickCallback)();
    
    
public:
    Button(char* label){
        _startX = 0;
        _startY = 0;
        _width = 100;
        _height = 100;
        _highlighted = 0;
        _label = label;
        _state = 0;
        
    }
    
    Button(char* label , int startX , int startY){
        _startX = startX;
        _startY = startY;
        _width = 100;
        _height = 100;
        _highlighted = 0;
        _label = label;
        _state = 0;
    }
    
    
    Button(char* label , int startX , int startY , int width , int height){
        _startX = startX;
        _startY = startY;
        _width = width;
        _height = height;
        _highlighted = 0;
        _label = label;
        _state = 0;
    }
    
    void draw();
    void addClickCallback( void (*f)(void) ){
        clickCallback = f;
    }
    
    int buttonClickTest(int x , int y);
    void setState(int state){
        _state = state;
    }
    
    
    void setHighlighted(int highlight);
    
    int isHighlighted(){
        if(_highlighted){
            return 1;
        }else{
            return 0;
        }
    }
    
    void clicked();
    
    int getObjectType();
    
};

#endif /* Button_hpp */
