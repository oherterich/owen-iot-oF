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
    
    serialPort.enumerateDevices();
    serialPort.setup(0, 9600);
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    
    //Get wind speed
    if ( serialPort.available() ) {
        prevWindSpeed = windSpeed;
        
        windSpeed = serialPort.readByte();
        
        if (windSpeed < 0) {
            windSpeed = 0;
        }
        else if (windSpeed > 30) {
            windSpeed = 30;
        }
        if (abs(windSpeed - prevWindSpeed) > 5.0) {
            windSpeed = 0.0;
        }
        
        float forceY = ofNoise(wind.pos.y * 0.01, ofGetElapsedTimef() * 0.1) * PI * 2;
        
        wind.addForce( ofVec2f( ofNoise( wind.pos.x ) * windSpeed / 2, sin( forceY ) * windSpeed ) );
        
//        cout << ofNoise( wind.pos.x ) * windSpeed / 2 << " | " << sin( forceY ) * windSpeed / 2 << endl;
        
        
    }
    
    float newWindSpeed = ofMap(windSpeed, 0, 10, 0.0, 2.0);

    
    if ( newWindSpeed > 3.0 && timeSinceTweet > 3.0 ) {
        cout << "huh" << endl;
        newTweet();
    }
    
    //Update all of our particles
    for (int i = 0; i < randomParticles.size(); i++) {
        randomParticles[i].setBoundaries();
        randomParticles[i].update();
    }
    
    for (int i = 0; i < particleList.size(); i++) {
        particleList[i].addAttractionForce(1500, 0.3);
        particleList[i].addRepulsionForce(mouseX, mouseY, 50.0, 5.0);
        particleList[i].addRepulsionForce( wind.pos.x, wind.pos.y, 250, newWindSpeed);
        particleList[i].addDampeningForce();
        particleList[i].update();
    }

    
    wind.update();
    
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
    
    //wind.draw();
    
    ofSetColor(255);
    ofDrawBitmapString("Move mouse to push letters. Click to get new tweet.", ofPoint(20, 20));
    
    ofDrawBitmapString( ofToString(ofMap(windSpeed, 0, 30, 0, 20.0)), 50, 50);
    ofDrawBitmapString( ofToString( timeSinceTweet ), 50, 70);
    ofDrawBitmapString( ofToString( wind.pos.x ) + " " + ofToString( wind.pos.y ), 50, 90);
    
    float forceY = ofNoise(wind.pos.y * 0.005, ofGetElapsedTimef() * 0.1) * PI * 2;
    ofDrawBitmapString( ofToString( sin(forceY) ), 50, 110);

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
    
    particleLocation = particleStart;
    
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
        
        //            particleLocation.x += 30.0;
        //
        //            if (particleLocation.x >= ofGetWindowWidth() - 100) {
        //                particleLocation.y += 45.0;
        //                particleLocation.x = particleStart.x;
        //            }
    }
    
    for (int i = 0; i < particleList.size() - 1; i++) {
        
        int count = 1;
        
        if (particleList[i].text == " ") {
            
            while ( particleList[i+count].text != " " && (i + count) < particleList.size() - 1) {
                count++;
                cout << i << " : " << count << "   " << particleList[i+count].text << endl;
            }
            
            float letterWidth = count * (30);
            float restofLine = ofGetWindowWidth() - 100 - particleList[i].pos.x;
            
            if ( letterWidth > restofLine ) {
                cout << i << " : line break! " << count << endl;
            }
            else {
                cout << i << " : it's fine! " << count <<  endl;
            }
        }
    }
    
    for (int i = 0; i < randomParticles.size(); i++) {
        randomParticles[i].vel.set(ofRandom(-5.0, 5.0), 0);
        randomParticles[i].text = twitterText[activeTweet][ofRandom(twitterText[activeTweet].length())];
    }
    
    newTweetTime = ofGetElapsedTimef();
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
