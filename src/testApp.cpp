#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(127,127,127);

	grabber.initGrabber(320, 240);

	//ofSetFrameRate(60);
	//motion.loadVideo("video/motion2.mov");
    motion.setup(grabber.width, grabber.height);
	bgExample.setup(grabber.width, grabber.height);
	
	camDraw.setup(&motion.gray, &motion.motionField);
	threshDraw.setup(&motion.motion, &motion.motionField);
			
	gui.setup("test cv", 0, 0, ofGetWidth(), 700);
	gui.addPanel("background subtraction example", 4, false);
	gui.addPanel("motion example", 4, false);
	gui.addPanel("third panel", 4, false);
	
	gui.setWhichPanel(0);
	gui.setWhichColumn(0);
	
//	gui.setWhichColumn(0);
//	gui.addDrawableRect("video", &motion.gray, 200, 150);
//	gui.addDrawableRect("background", &motion.background, 200, 150);
//	
	gui.setWhichColumn(0);
	gui.addDrawableRect("video", &bgExample.gray, 200, 150);

	gui.setWhichColumn(1);
	gui.addDrawableRect("background", &bgExample.background, 200, 150);

	gui.setWhichColumn(2);
	gui.addDrawableRect("thresholded image", &bgExample.thresh, 200, 150);

	gui.setWhichColumn(3);
	gui.addToggle("grab background", "GRAB_BACKGROUND", 0);		

	gui.addSlider("threshold", "BG_THRESHOLD", 29.0, 1.0, 255.0, false);		
	
	vector <string> names;
	names.push_back("abs diff");
	names.push_back("greater than");
	names.push_back("less than");
	gui.addTextDropDown("difference mode", "DIFF_MODE", 0, names);
	
	gui.setWhichPanel(1);
	
	gui.setWhichColumn(0);
	gui.addDrawableRect("video and motion", &camDraw, 200, 150);

	gui.setWhichColumn(1);
	gui.addDrawableRect("motion diff", &threshDraw, 200, 150);
	
	gui.setWhichColumn(2);
	stats.setDrawScale(2000.0);
	gui.addDrawableRect("average motion", &stats, 200, 150);

	gui.setWhichColumn(3);
	gui.addSlider("motion threshold", "MOTION_THRESHOLD", 29.0, 1.0, 255.0, false);	
	gui.addSlider("motion fade amnt", "MOTION_FADE", 0.67, 0.0, 1.0, false);	
	gui.addSlider("field draw scale", "FIELD_DRAW_SCALE", 1.0, 1.0, 10.0, false);	

	//load from xml!
	gui.loadSettings("controlPanelSettings.xml");

	//if you want to use events call this after you have added all your gui elements
	gui.setupEvents();
	gui.enableEvents();
	
//	vector <string> list;
//	list.push_back("FIELD_DRAW_SCALE");
//	list.push_back("DIFF_MODE");
//	gui.createEventGroup("TEST_GROUP", list);
//	
//	ofAddListener(gui.getEventsForPanel(0), this, &testApp::eventsIn);

	ofAddListener(gui.guiEvent, this, &testApp::eventsIn);

}

//--------------------------------------------------------------
void testApp::eventsIn(guiCallbackData & data){

	if( data.groupName == "GRAB_BACKGROUND" && data.getFloat(0.0) == 1.0 ){
		bgExample.captureBackground();
		gui.setValueB("GRAB_BACKGROUND", false);
	}

	printf("testApp::eventsIn - name is %s - \n", data.groupName.c_str());
	if( data.elementName != "" ){
		printf(" element name is %s \n", data.elementName.c_str());
	}
	if( data.fVal.size() ){
		for(int i = 0; i < data.fVal.size(); i++){
			printf(" float value [%i] = %f \n", i, data.fVal[i]);
		}
	}
	if( data.iVal.size() ){
		for(int i = 0; i < data.iVal.size(); i++){
			printf(" int value [%i] = %i \n", i, data.iVal[i]);
		}
	}	
	if( data.sVal.size() ){
		for(int i = 0; i < data.sVal.size(); i++){
			printf(" string value [%i] = %s \n", i, data.sVal[i].c_str());
		}
	}
	printf("\n");
}

//--------------------------------------------------------------
void testApp::update(){

//    motion.setThreshold(thresh);
//    motion.setFadeAmnt(fade);
	
	camDraw.setDrawScale(gui.getValueF("FIELD_DRAW_SCALE"));
	threshDraw.setDrawScale(gui.getValueF("FIELD_DRAW_SCALE"));
	
	grabber.update(); 

	if( grabber.isFrameNew() ){	
		if( gui.getSelectedPanel() == 0 ){
			bgExample.update(grabber.getPixels(), grabber.width, grabber.height);
		}else if( gui.getSelectedPanel() == 1 ){
			motion.update(grabber.getPixels(), grabber.width, grabber.height);
		}
	}
	
	motion.setThreshold(gui.getValueI("MOTION_THRESHOLD"));
	motion.setFadeAmnt(gui.getValueF("MOTION_FADE"));
	
	bgExample.setDifferenceMode(gui.getValueI("DIFF_MODE"));
	bgExample.setThreshold(gui.getValueI("BG_THRESHOLD"));
	
	stats.updateFromField(motion.motionField, 0.97);
	
	gui.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	gui.draw();
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    //if you need to adjust the camera properties
    if(key == 's'){
        grabber.videoSettings();
    }
	
//	gui.keyPressed(key);
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	gui.mousePressed(x, y, button);

}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	gui.mouseReleased();
}
