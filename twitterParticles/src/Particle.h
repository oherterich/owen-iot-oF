//
//  Particle.h
//  twitterParticles
//
//  Created by Owen Herterich on 2/12/14.
//
//

#pragma once

#include "ofMain.h"

class Particle {
public:
    Particle( ofTrueTypeFont *_font );
    void update();
    void draw();
    void addForce( ofVec2f frc );
    void addAttractionForce( float radius, float strength );
    void addRepulsionForce( float px, float py, float radius, float strength );
    void addDampeningForce();
    
    ofVec2f pos, vel, acc;
    ofColor c;
    float size;
    float dampening;
    
    ofVec2f loc;
    
    ofTrueTypeFont *font;
    string text;
};
