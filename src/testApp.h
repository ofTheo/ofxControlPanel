#ifndef _TEST_APP
#define _TEST_APP


//#define OF_ADDON_USING_OFXOPENCV
//#define OF_ADDON_USING_OFXOBJLOADER
//#define OF_ADDON_USING_OFXDIRLIST
//#define OF_ADDON_USING_OFXVECTORMATH
//#define OF_ADDON_USING_OFXNETWORK
//#define OF_ADDON_USING_OFXVECTORGRAPHICS
//#define OF_ADDON_USING_OFXOSC
//#define OF_ADDON_USING_OFXTHREAD
//#define OF_ADDON_USING_OFXXMLSETTINGS

#include "ofMain.h"
#include "ofAddons.h"
#include "computerVision.h"
#include "ofxControlPanel.h"


class testApp : public ofSimpleApp{

	public:
		
		int iAvrg;
		float avrgArray[20];
		float xVec, yVec;
		
		void setup();
		void update();
		void draw();
		
		void eventsIn(guiCallbackData & data);
		
		void keyPressed  (int key);
		void keyReleased (int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		ofxControlPanel gui;
		
		motionStats stats;
		motionDraw threshDraw;
		motionDraw camDraw;
        computerVision vision;

};

#endif

