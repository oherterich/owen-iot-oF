//
//  Particle.h
//  GridParticles
//
//  Created by Charlie Whitney on 9/23/13.
//
//

#pragma once 

#include "ofMain.h"

class Particle {
  public:
    Particle( ofVec2f position, ofImage* _img, float resolution );
  
    void update();
    void draw();
    void addForce( ofVec2f frc );
    void addAttractionForce( float radius, float strength );
    void addRepulsionForce( float px, float py,float radius, float strength );
    void lerpToColor(ofColor startColor, ofColor endColor, float amt);
    
    ofVec2f pos, vel, acc;
    ofVec2f initPos, loc;
    float damping;
    
    
    //
    float   radius;
    float   grayValue;
    ofColor c;
    float sinMod;
    
    ofImage* img;
    
    bool next;
};