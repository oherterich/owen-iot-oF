#pragma once

#include "ofMain.h"
#include "Particle.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        void addParticle(float x, float y);
        vector<Particle>    particleList;
    
    int numImages;
    int resolution;
    int imgSize;
    vector<ofImage> imgList;
    
    void changePic();
    void getRandom();
    int changeSpeed;
    int currentPic;
    
    vector<int> picked;
    int randomParticle;
    
    float timeBetween;
    float picTimeStamp;
    
    ofVec2f repulsePos;
};
