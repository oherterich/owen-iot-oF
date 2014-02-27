#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(0);
    
    font.loadFont("KlinicSlabBook.otf", 24);
    
    //Intial connection to twitter
    numOfTweets = 10;
    activeTweet = 0;
    newTweetTime = 0;
    
    //Token and Secret from Twitter dev
    string key = "BLjWmM1QxeVHjh3UuEaw";
    string secret = "m327ldIEiMnDT9hQ57VGZxYDZtx9jhJTaf9BNKYrs";

    client.setup(key,secret);
    
    if(client.isAuthorized())
    {
        // Get tweets.
        std::string s = client.exampleMethod("#technology", numOfTweets);
        
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
    particleStart.set(100, ofGetWindowHeight() / 2 - 50);
    particleLocation = particleStart;
    pushForce = 25.0;
    
    //This adds the main letter particles
    for (int i = 0; i < 140; i++) {
        addParticle( i );
        
        if (i <= twitterText[activeTweet].length()) {
            particleList[i].text = twitterText[activeTweet][i];
        }
        else {
            particleList[i].text = " ";
        }
        
        particleLocation.x += 30.0;
        
        if (particleLocation.x >= ofGetWindowWidth() - 100) {
            particleLocation.y += 45.0;
            particleLocation.x = particleStart.x;
        }
    }
    
    //This adds all of the small letter particles in the background
    for (int i = 0; i < 500; i++) {
        Particle p( &font );
        p.vel.set(ofRandom(-5.0, 5.0), 0);
        p.text = twitterText[activeTweet][ofRandom(twitterText[activeTweet].length())];
        p.scale = ofRandom(0.3, 0.4);
        p.theta = 0;
        p.trans = 80;
        
        randomParticles.push_back( p );
    }

}

//--------------------------------------------------------------
void testApp::update(){
    
    //Update all of our particles
    for (int i = 0; i < randomParticles.size(); i++) {
        randomParticles[i].setBoundaries();
        randomParticles[i].update();
    }
    
    for (int i = 0; i < particleList.size(); i++) {
        particleList[i].addAttractionForce(1500, 0.3);
        particleList[i].addRepulsionForce(mouseX, mouseY, 50.0, 5.0);
        particleList[i].addDampeningForce();
        particleList[i].update();
    }
    
    timeSinceTweet = ofGetElapsedTimef() - newTweetTime;
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //Background color changes according to sin wave...smooth.
    bkgrnd.setHsb( sin(ofGetElapsedTimef() * 0.2) * 10 + 160, sin(ofGetElapsedTimef() * 0.2) * 10 + 100, sin(ofGetElapsedTimef() * 0.2) * 10 + 40);
    ofBackground(bkgrnd);
    
    //Draw all of our particles
    for (int i = 0; i < randomParticles.size(); i++) {
        randomParticles[i].draw();
    }

    for (int i = 0; i < particleList.size(); i++) {
        particleList[i].draw();
    }
    
    ofSetColor(255);
    ofDrawBitmapString("Move mouse to push letters. Click to get new tweet.", ofPoint(20, 20));
    

}

//Function that adds a particle to our vector.
void testApp::addParticle( int num ) {
    Particle p( &font );
    p.loc.set(particleLocation);
    p.text = twitterText[activeTweet][num];
    particleList.push_back(p);
}

//This function gets the text from each tweet by parsing our ofxJSONElement
void testApp::parseData() {
    for (int i = 0; i < json["statuses"].size(); i++) {
        //cout << json["statuses"][i]["text"] << endl;
        string t = json["statuses"][i]["text"].asString();
        cout << t << endl;
        twitterText.push_back( t );
    }
}

//Get a new tweet
void testApp::newTweet() {
    //We need to check to see if we've reached the end of our array
    if (activeTweet < numOfTweets) {
        activeTweet++;
        
        //Let's also make sure the tweet isn't blank.
        if (twitterText[activeTweet].length() < 1) {
            activeTweet++;
        }
    }
    else {
        activeTweet = 0;
    }
    
    //If we have a new tweet, make the particles go crazy.
    for (int i = 0; i < particleList.size(); i++) {
        particleList[i].addForce( ofVec2f(ofRandom(-pushForce, pushForce), ofRandom(-pushForce, pushForce) ) );
        particleList[i].theta = ofRandom(-180, 180);
        if (i <= twitterText[activeTweet].length()) {
            particleList[i].text = twitterText[activeTweet][i];
        }
        else {
            particleList[i].text = " ";
        }
    }
    
    for (int i = 0; i < randomParticles.size(); i++) {
        randomParticles[i].vel.set(ofRandom(-5.0, 5.0), 0);
        randomParticles[i].text = twitterText[activeTweet][ofRandom(twitterText[activeTweet].length())];
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
    //Click the mouse, get a new tweet.
    newTweet();
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
