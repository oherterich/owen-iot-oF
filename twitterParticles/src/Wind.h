//
//  Wind.h
//  twitterParticles
//
//  Created by Owen Herterich on 3/17/14.
//
//

#pragma once

#include "ofMain.h"

class Wind {
public:
    Wind();
    void update();
    void draw();
    void addForce ( ofVec2f frc );
    
    ofVec2f pos, vel, acc;
    float damping;
};
