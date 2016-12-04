//
//  Face.h
//  Project
//
//  Created by Gaurav Mishra on 04/12/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef Face_h
#define Face_h

class Face{
public:
    void virtual draw() = 0;
    void virtual warp(int minX , int minY , int minZ , double rangeX , double rangeY , double rangeZ) = 0;
};

#endif /* Face_h */
