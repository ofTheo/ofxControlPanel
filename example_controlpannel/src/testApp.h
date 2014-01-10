#pragma once

#include "ofMain.h"
#include "ofAddons.h"
#include "videoMotionExample.h"
#include "ofxControlPanel.h"
#include "backgroundSubtractionExample.h"

#include "customDrawer.h"

class testApp : public ofSimpleApp{

	public:
		
		void setup();
		void update();
		void draw();
		
		void eventsIn(guiCallbackData & data);
		void grabBackgroundEvent(guiCallbackData & data);
		
		void keyPressed  (int key);
		void keyReleased (int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		ofxControlPanel gui;
		simpleFileLister lister;
		simpleLogger logger;
		
		motionStats stats;
		motionDraw threshDraw;
		motionDraw camDraw;
		
		ofVideoGrabber grabber;
		backgroundSubtractionExample bgExample;
		customDrawer pointAdder;
		
		float elapsedTime;
		int appFrameCount;
		float appFrameRate;
		
		ofTrueTypeFont TTF;
		
        videoMotionExample motion;

};


