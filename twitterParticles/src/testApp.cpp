#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    font.loadFont("KlinicSlabLight.otf", 24);
    
    //Intial connection to twitter
    numOfTweets = 25;
    activeTweet = 0;
    
    //Token and Secret from Twitter dev
    string key = "BLjWmM1QxeVHjh3UuEaw";
    string secret = "m327ldIEiMnDT9hQ57VGZxYDZtx9jhJTaf9BNKYrs";

    client.setup(key,secret);
    
    if(client.isAuthorized())
    {
        // Get tweets.
        std::string s = client.exampleMethod("#yolo", numOfTweets);
        
        bool parsingSuccessful = json.parse(s);
        if (parsingSuccessful) {
            parseData();
        } else {
            cout  << "Failed to parse JSON" << endl;
        }
    }
    else
    {
        ofLogWarning("testApp::keyPressed") << "Not authorized yet.";
    }
    
    
    //Do all of the initial particle setup
    particleStart.set(100, ofGetWindowHeight() / 2 - 100);
    particleLocation = particleStart;
    
    for (int i = 0; i < 140; i++) {
        addParticle( i );
        particleLocation.x += 30.0;
        
        if (particleLocation.x >= ofGetWindowWidth() - 100) {
            particleLocation.y += 30.0;
            particleLocation.x = particleStart.x;
        }
    }

}

//--------------------------------------------------------------
void testApp::update(){
    for (int i = 0; i < particleList.size(); i++) {
        particleList[i].addAttractionForce(1500, 0.3);
        particleList[i].addRepulsionForce(mouseX, mouseY, 50.0, 5.0);
        particleList[i].addDampeningForce();
        particleList[i].update();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    for (int i = 0; i < particleList.size(); i++) {
        particleList[i].draw();
    }
}

void testApp::addParticle( int num ) {
    Particle p( &font );
    p.loc.set(particleLocation);
    p.text = twitterText[activeTweet][num];
    particleList.push_back(p);
}

void testApp::parseData() {
    for (int i = 0; i < json["statuses"].size(); i++) {
        //cout << json["statuses"][i]["text"] << endl;
        string t = json["statuses"][i]["text"].asString();
        twitterText.push_back( t );
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == ' '){
        //vector<string> result = ofSplitString(twitterText[0], " ");

//        for (int i = 0; i < result.size(); i++) {
//            cout << result[i] << endl;
//        }
    }
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
    if (activeTweet < numOfTweets) {
        activeTweet++;
    }
    else {
        activeTweet = 0;
    }
    
    for (int i = 0; i < particleList.size(); i++) {
        particleList[i].addForce( ofVec2f(ofRandom(-15.0, 15.0), ofRandom(-15.0, 15.0) ) );
        particleList[i].text = twitterText[activeTweet][i];
    }
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
