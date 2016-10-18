//
//  DrawableObject.hpp
//  Project
//
//  Created by Gaurav Mishra on 28/09/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef DrawableObject_hpp
#define DrawableObject_hpp

#include <stdio.h>

class DrawableObject {
    
public:
    void virtual draw() = 0;
    int virtual getObjectType() = 0;
};

#endif /* DrawableObject_hpp */
