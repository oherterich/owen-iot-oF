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
    
    // kinect starts here ------------------------------------------------------
    
    kinect.setRegistration(true);
    
	kinect.init();
    kinect.open();
    
    colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	
	nearThreshold = 230;
	farThreshold = 70;
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// start from the front
	bDrawPointCloud = false;
    
    
    // kinect ends here ------------------------------------------------------
    
    
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
        
        for ( int j = 0; j < contourFinder.blobs.size(); j++ ) {
//            particleList[i].addRepulsionForce( contourFinder.blobs[j].boundingRect.x, contourFinder.blobs[j].boundingRect.y, contourFinder.blobs[j].boundingRect.width, 0.3 );

            particleList[i].addRepulsionForce( contourFinder.blobs[j].centroid.x, contourFinder.blobs[j].centroid.y, contourFinder.blobs[j].boundingRect.width, ofNoise( contourFinder.blobs[j].centroid.x ) );
        
        }

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
    
    
     // kinect starts here ------------------------------------------------------
    
    kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			grayThreshNear = grayImage;
			grayThreshFar = grayImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		} else {
			
			// or we do it ourselves - show people how they can work with the pixels
			unsigned char * pix = grayImage.getPixels();
			
			int numPixels = grayImage.getWidth() * grayImage.getHeight();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
		
		// update the cv images
		grayImage.flagImageChanged();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
	}
    
     // kinect ends here ------------------------------------------------------
    
    
    
    
    
    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    for( int i=0; i<particleList.size(); i++ ){
        particleList[i].draw();
    }
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(currentPic), ofPoint(600,10));
    
    
    
    //kinect
    
   // kinect.drawDepth(10, 10, 400, 300);
   // kinect.draw(420, 10, 400, 300);
    
    //grayImage.draw(10, 320, 400, 300);
    contourFinder.draw(0, 0, 500, 500);
    
    
    
    //kinect ends
    
    if (contourFinder.blobs.size() > 0) {
        ofDrawBitmapString( ofToString(contourFinder.blobs[0].boundingRect.x) + " | " + ofToString(contourFinder.blobs[0].boundingRect.y), 50, 50);
    }
    
    
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
    
    
    switch (key) {
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;
			
		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;
			
		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;
			
		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;
			
		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
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


//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}
