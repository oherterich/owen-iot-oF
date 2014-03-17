#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    numImages = 12;
    
    for( int i = 0; i < numImages; i++ ) {
        ofImage img;
        img.loadImage("pic_" + ofToString(i) + ".png");
        
        imgList.push_back(img);
    }
    
    resolution = 10.0;
    imgSize = 500.0;
    
    int xRes = floor( imgSize / resolution );
    int yRes = floor( imgSize / resolution );
    
    for( int y=0; y<yRes; y++ ){
        for( int x=0; x<xRes; x++ ){
            addParticle( x, y );
        }
    }
    
    randomParticle = 0;
    changeSpeed = 10;
    currentPic = 0;
    
    timeBetween = 2.0;
    picTimeStamp = 0.0;
    
    repulsePos.set(-500, -500);
    
    ofBackground(0);
}

void testApp::addParticle(float x, float y) {
    float xPos = ( x + 0.5f ) * resolution;
    float yPos = ( y + 0.5f ) * resolution;

    Particle p = Particle( ofVec2f( xPos, yPos ), &imgList[0], resolution );
    p.loc.set( xPos, yPos);
    
    particleList.push_back( p );
}

//--------------------------------------------------------------
void testApp::update(){
    for( int i=0; i<particleList.size(); i++ ){
        if ( particleList[i].next ) {
            float grayValue = particleList[i].img->getColor( particleList[i].initPos.x, particleList[i].initPos.y ).g;
            ofColor newColor = ofColor(grayValue);
            
            particleList[i].lerpToColor(particleList[i].c, newColor, 0.02);
        }
        particleList[i].update();
        particleList[i].addAttractionForce(1500, 0.3);
        particleList[i].addRepulsionForce(mouseX, mouseY, 500.0, 0.3);

    }
    
    if (ofGetElapsedTimef() - picTimeStamp > timeBetween) {
        if (picked.size() <= particleList.size() - changeSpeed) {
            
            for (int i = 0; i < changeSpeed; i++) {
                changePic();
            }
            
        }
        else {
            
            if (currentPic < numImages - 1) {
                currentPic++;
                picTimeStamp = ofGetElapsedTimef();
            }
            else {
                currentPic = 0;
                picTimeStamp = ofGetElapsedTimef();
            }
            
            picked.clear();
            
        }
    }
    
    if (repulsePos.x < ofGetWindowWidth() + 500 && repulsePos.y < ofGetWindowHeight() + 500) {
        repulsePos.x+=2;
        repulsePos.y+=2;
    }
    else {
        repulsePos.set(-500,-500);
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    for( int i=0; i<particleList.size(); i++ ){
        particleList[i].draw();
    }
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(currentPic), ofPoint(600,10));
    
}

void testApp::changePic() {
    getRandom();
    if (currentPic < numImages - 1) {
        particleList[randomParticle].img = &imgList[currentPic+1];
        particleList[randomParticle].next = true;
    }
    else {
        particleList[randomParticle].img = &imgList[0];
    }
    
    int p = randomParticle;
    picked.push_back(p);
}

void testApp::getRandom() {
    randomParticle = ofRandom(0, particleList.size());
    for (int i = 0; i < picked.size(); i++) {
        if (randomParticle == picked[i]) {
            getRandom();
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
