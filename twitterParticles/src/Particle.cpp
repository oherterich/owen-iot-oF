//
//  Particle.cpp
//  twitterParticles
//
//  Created by Owen Herterich on 2/12/14.
//
//

#include "Particle.h"

Particle::Particle( ofTrueTypeFont *_font ) {
    pos.set(ofRandomWidth(), ofRandomHeight());
    vel.set(ofRandom(-5, 5), ofRandom(-5, 5));
    acc.set(0);
    
    c.setHsb(ofRandom(0, 30), 180, ofRandom(220,240));
    
    size = ofRandom(12, 16);
    dampening = ofRandom(0.95, 0.97);
    
    font = _font;
    
    text = "A";
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

void Particle::addDampeningForce() {
    vel *= dampening;
}

void Particle::update() {
    vel += acc;
    pos += vel;
    
    acc.set(0);
}

void Particle::draw() {
    ofSetColor(c);
    //ofCircle(pos, size);
    font->drawString(text, pos.x, pos.y);
}