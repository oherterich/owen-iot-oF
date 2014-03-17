//
//  Particle.cpp
//  GridParticles
//
//  Created by Charlie Whitney on 9/23/13.
//
//

#include "Particle.h"

Particle::Particle( ofVec2f position, ofImage* _img, float resolution ) {
    pos = position;
    initPos = pos;
    damping = 0.95;
    radius = resolution / 2;
    grayValue = 255;
//    radius = ofRandom(1.0, 5.0);
//    radius = cos( pos.y * 0.1 ) + sin( pos.x * 0.1 ) + 2;
//    radius = ( sin( pos.y * pos.x ) + 1.0f ) * 2.0f;
    
    img = _img;
    grayValue = img->getColor( initPos.x, initPos.y ).g;
    c.set(grayValue);
    sinMod = ofRandom(10000);
    
    next = false;
}

void Particle::addForce( ofVec2f frc ) {
    acc += frc;
}

void Particle::addAttractionForce( float radius, float strength ) {
    
    ofVec2f diff;
    diff = pos - loc;
    
    if (diff.length() > 1.0) {
        if ( diff.lengthSquared() < (radius * radius) ) {
            float pct = 1 - ( diff.lengthSquared() / (radius * radius) );
            diff.normalize();
            
            acc -= diff * pct * strength;
        }
    }
    else {
        pos = loc;
    }
}

void Particle::addRepulsionForce( float px, float py, float radius, float strength ) {
    ofVec2f loc;
    loc.set(px, py);
    
    ofVec2f diff;
    diff = pos - loc;
    
    if ( diff.lengthSquared() < (radius * radius) ) {
        float pct = 1 - ( diff.lengthSquared() / (radius * radius) );
        diff.normalize();
        
        acc += diff * pct * strength;
    }
    
}

void Particle::lerpToColor(ofColor startColor, ofColor endColor, float amt){
    c=startColor;
    c.lerp(endColor, amt);
}

void Particle::update(){
    
    vel += acc;
    pos += vel;
    
    acc.set(0);
    vel *= damping;
    
//    grayValue = img->getColor( initPos.x, initPos.y ).g;
//    c.set(grayValue);
    
    //radius = ((float)grayValue / 255.0) * 7.0;
    radius = 4;
}

void Particle::draw() {
//    ofSetColor( grayValue );
//    ofSetColor(255);
    float mod = sin(ofGetElapsedTimef()/10 + sinMod) * 10;
    
    ofSetColor(c + mod);
    ofCircle( pos, radius );
}