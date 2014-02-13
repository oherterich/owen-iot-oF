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
    
    c.setHsb(35, 190, 190);
    
    size = ofRandom(12, 16);
    dampening = ofRandom(0.95, 0.97);
    
    font = _font;
    
    text = "A";
    
    theta = ofRandom(-180, 180);
    scale = 1;
    trans = 255.0;
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

void Particle::correctRotation() {
    if (theta <= 0 ) {
        theta++ ;
    }
    else if (theta >= 0 ) {
        theta-- ;
    }
}

void Particle::setBoundaries() {
    if (pos.x > ofGetWindowWidth() + 50) {
        pos.set(-50, ofRandomHeight());
    }
    
    if (pos.x < -50) {
        pos.set(ofGetWindowWidth() + 50, ofRandomHeight());
    }
}

void Particle::update() {
    vel += acc;
    pos += vel;
    
    correctRotation();
    
    float h = sin(ofGetElapsedTimef() * 0.02) * 100 + 100;
    c.setHsb(h, 210, 255);
    
    acc.set(0);
}

void Particle::draw() {
    ofSetColor(c, trans);
    ofPushMatrix();{
        ofTranslate(pos);
        ofRotate(theta);
        ofScale(scale, scale);
        font->drawString(text, 0, 0);
    }ofPopMatrix();
}