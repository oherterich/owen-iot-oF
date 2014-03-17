//
//  Wind.cpp
//  twitterParticles
//
//  Created by Owen Herterich on 3/17/14.
//
//

#include "Wind.h"

Wind::Wind() {
    pos.set( -200, ofGetWindowHeight() / 2 );
    vel.set(0,0);
    
    damping = 0.95;
}

void Wind::addForce( ofVec2f frc ) {
    acc += frc;
}

void Wind::update() {
    vel += acc;
    pos += vel;
    
    vel *= damping;
    acc.set(0);
    
    if (pos.x > ofGetWindowWidth() + 200 ) {
        pos.x = -200;
    }
    
    if (pos.y > ofGetWindowHeight() / 2 + 100) {
        pos.y = ofGetWindowHeight() / 2 - 100;
    }
    
    if (pos.y < ofGetWindowHeight() / 2 - 100) {
        pos.y = ofGetWindowHeight() / 2 + 100;
    }
}

void Wind::draw() {
    
    ofSetColor(255,0,0);
    ofCircle( pos, 50 );
}