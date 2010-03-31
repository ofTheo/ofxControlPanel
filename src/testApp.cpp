#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(127,127,127);
	//ofSetFrameRate(60);
	//vision.loadVideo("video/motion2.mov");
    vision.setupCamera(0, 320, 240);
	
	camDraw.setup(&vision.gray, &vision.motionField);
	threshDraw.setup(&vision.thresh, &vision.motionField);
			
	gui.setup("test cv", 0, 0, ofGetWidth(), 700);
	gui.addPanel("first panel", 4, false);
	gui.addPanel("second panel", 1, false);
	gui.addPanel("third panel", 1, false);
	
	gui.setWhichPanel(0);
	
	vector <string> names;
	names.push_back("top");
	names.push_back("left");
	names.push_back("right");
	names.push_back("down");
	
	gui.setWhichColumn(0);
	gui.addDrawableRect("video and motion", &camDraw, 200, 150);

	gui.setWhichColumn(1);
	gui.addDrawableRect("motion diff", &threshDraw, 200, 150);
	
	gui.setWhichColumn(2);
	stats.setDrawScale(2000.0);
	gui.addDrawableRect("average motion", &stats, 200, 150);

	gui.setWhichColumn(3);
	gui.addSlider("field draw scale", "FIELD_DRAW_SCALE", 1.0, 1.0, 10.0, false);	
	gui.addTextDropDown("direction", "DIRECTION", 0, names);

	//if you want to use events call this after you have added all your gui elements
	gui.setupEvents();
	
	vector <string> list;
	list.push_back("FIELD_DRAW_SCALE");
	list.push_back("DIRECTION");
	gui.createEventGroup("TEST_GROUP", list);
	
	ofAddListener(gui.getEventsForPanel(0), this, &testApp::eventsIn);

}

//--------------------------------------------------------------
void testApp::eventsIn(guiCallbackData & data){

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

//    vision.setThreshold(thresh);
//    vision.setFadeAmnt(fade);
	
	camDraw.setDrawScale(gui.getValueF("FIELD_DRAW_SCALE"));
	threshDraw.setDrawScale(gui.getValueF("FIELD_DRAW_SCALE"));
	
    vision.update();
	
	stats.updateFromField(vision.motionField, 0.97);
	
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
        vision.camera.videoSettings();
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
