#pragma once

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "ofxOpenCv.h"

class  ofApp: public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void eventsIn(guiCallbackData & data);
    
    
        ofxControlPanel gui;
        simpleFileLister lister;
    
        ofVideoGrabber  grabber;
        ofParameterGroup vars;
    
        ofxCvColorImage colorCV;
        ofxCvGrayscaleImage grayscaleCV;
    
        ofImage img; 
    
        ofParameter <float> elapsedTime;
        ofParameter <int> appFrameCount;
        ofParameter <float> appFrameRate;
    
        ofParameterGroup cvControls; 
        ofParameter <bool> bInvert; 
        ofParameter <int> threshold;
    
        ofParameter <string> testLabel;
        ofParameter <string> status; 
    
    
};
