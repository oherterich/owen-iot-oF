#pragma once

#include "ofMain.h"
#include "ofxJSONElement.h"
#include "Particle.h"
#include "ofxExampleTwitterClient.h"

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
    
    void addParticle( int num );
    void parseData();
    
    vector<Particle> particleList;
    
    ofVec2f particleLocation;
    ofVec2f particleStart;
    
    ofTrueTypeFont font;
    
    ofxExampleTwitterClient client;
    
    ofxJSONElement json;
    
    vector<string> twitterText;
    int numOfTweets;
    int activeTweet;
		
};
