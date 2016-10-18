//
//  Button.cpp
//  Project
//
//  Created by Gaurav Mishra on 17/10/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#include "Button.hpp"
#include "Utils.hpp"
#include <GLUT/GLUT.h>
#include <iostream>
using namespace std;

void Font(void *font,char *text,int x,int y)
{
    glRasterPos2i(x, y);
    
    while( *text != '\0' )
    {
        glutBitmapCharacter( font, *text );
        ++text;
    }
}

void Button::setHighlighted(int highlight){

    _highlighted = highlight;
}

void Button::draw(){

    if (this->_highlighted)
        glColor3f(0.7f,0.7f,0.8f);
    else
        glColor3f(0.6f,0.6f,0.6f);
    
    /*
     *	draw background for the button.
     */
    glBegin(GL_QUADS);
    glVertex2i( _startX  , _startY );
    glVertex2i( _startX     , _startY + _height );
    glVertex2i( _startX + _width, _startY + _height );
    glVertex2i( _startX + _width , _startY );
    glEnd();
    
    /*
     *	Draw an outline around the button with width 3
     */
    glLineWidth(3);
    
    /*
     *	The colours for the outline are reversed when the button.
     */
    if (_state)
        glColor3f(0.4f,0.4f,0.4f);
    else
        glColor3f(0.8f,0.8f,0.8f);
    
    glBegin(GL_LINE_STRIP);
    glVertex2i( _startX + _width , _startY );
    glVertex2i( _startX  , _startY );
    glVertex2i( _startX  , _startY + _height );
    glEnd();
    
    if (_state)
        glColor3f(0.8f,0.8f,0.8f);
    else
        glColor3f(0.4f,0.4f,0.4f);
    
    glBegin(GL_LINE_STRIP);
    glVertex2i( _startX  , _startY + _height );
    glVertex2i( _startX + _width, _startY + _height);
    glVertex2i( _startX + _width, _startY );
    glEnd();
    
    glLineWidth(1);
    
    
    /*
     *	Calculate the x and y coords for the text string in order to center it.
     */
    _fontX = _startX + (_width - 50) / 2 ;
    _fontY = _startY + (_startY + 10 )/2;
    
    /*
     *	if the button is pressed, make it look as though the string has been pushed
     *	down. It's just a visual thing to help with the overall look....
     */
    if (_state) {
        _fontX+=2;
        _fontY+=2;
    }
    
    /*
     *	If the cursor is currently over the button we offset the text string and draw a shadow
     */
    if(_highlighted)
    {
        glColor3f(0,0,0);
        Font(GLUT_BITMAP_HELVETICA_10,_label,_fontX,_fontY);
        _fontX--;
        _fontY--;
    }
    
    glColor3f(1,1,1);
    Font(GLUT_BITMAP_HELVETICA_10,_label,_fontX,_fontY);
}

int Button::buttonClickTest(int x , int y){
    if(x > _startX && x < _startX+_width && y > _startY && y < _startY + _height){
        return 1;
    }else{
        return 0;
    }
}

void Button :: clicked(){
    clickCallback();
}

int Button::getObjectType(){
    return OBJECT_TYPE_BUTTON;
}

