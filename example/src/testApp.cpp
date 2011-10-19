#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(127,127,127);
		
	ofSetVerticalSync(true);
	
	grabber.initGrabber(320, 240);

    motion.setup(grabber.width, grabber.height);
	bgExample.setup(grabber.width, grabber.height);
	
	camDraw.setup(&motion.gray, &motion.motionField);
	threshDraw.setup(&motion.motion, &motion.motionField);	

	ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 60, 200));
	ofxControlPanel::setTextColor(simpleColor(240, 50, 50, 255));
		
	gui.loadFont("MONACO.TTF", 8);		
	gui.setup("test cv", 0, 0, ofGetWidth(), 700);
	gui.addPanel("background subtraction example", 4, false);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));	
	gui.addPanel("motion example", 4, false);
	
	ofxControlPanel::setBackgroundColor(simpleColor(70, 70, 30, 200));	
	gui.addPanel("third panel", 4, false);

	ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 30, 200));	
		
	//--------- PANEL 1
	gui.setWhichPanel(0);
		
	gui.setWhichColumn(0);
	gui.addDrawableRect("video", &bgExample.gray, 200, 150);
	
	lister.listDir("images/of_logos/");
	gui.addFileLister("image lister", &lister, 200, 300);
	
	gui.setWhichColumn(1);
	gui.addDrawableRect("background", &bgExample.background, 200, 150);

	gui.enableIgnoreLayoutFlag();
	gui.addLogger("events logger", &logger, 410, 300);
	gui.disableIgnoreLayoutFlag();

	gui.setWhichColumn(2);
	gui.addDrawableRect("thresholded image", &bgExample.thresh, 200, 150);

	gui.setWhichColumn(3);
	gui.addToggle("grab background", "GRAB_BACKGROUND", 0);		

	gui.addButtonSlider("threshold", "BG_THRESHOLD", 29.0, 1.0, 255.0, false);		
	
	//some dummy vars we will update to show the variable lister object
	elapsedTime		= ofGetElapsedTimef();
	appFrameCount	= ofGetFrameNum();	
	appFrameRate	= ofGetFrameRate();
		
	vector <guiVariablePointer> vars;
	vars.push_back( guiVariablePointer("ellapsed time", &elapsedTime, GUI_VAR_FLOAT, 2) );
	vars.push_back( guiVariablePointer("ellapsed frames", &appFrameCount, GUI_VAR_INT) );
	vars.push_back( guiVariablePointer("app fps", &appFrameRate, GUI_VAR_FLOAT, 2) );

	vars.push_back( guiVariablePointer("mouse x", &mouseX, GUI_VAR_INT) );
	vars.push_back( guiVariablePointer("mouse y", &mouseY, GUI_VAR_INT) );

	gui.addVariableLister("app vars", vars);
	
	gui.addChartPlotter("some chart", guiStatVarPointer("app fps", &appFrameRate, GUI_VAR_FLOAT, true, 2), 200, 100, 200, 40, 80);
	
	vector <string> names;
	names.push_back("abs diff");
	names.push_back("greater than");
	names.push_back("less than");
	gui.addTextDropDown("difference mode", "DIFF_MODE", 0, names);
	gui.addLabel("a label");
	gui.addTextInput("text input space", "default text", 200 );
	
	//--------- PANEL 2
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
	
	//------- PANEL 3
	gui.setWhichPanel(2);
	gui.addCustomRect("custom implementation demo", &pointAdder, 500, 375);

	//SETTINGS AND EVENTS

	//load from xml!
	gui.loadSettings("controlPanelSettings.xml");

	//if you want to use events call this after you have added all your gui elements
	gui.setupEvents();
	gui.enableEvents();
	
//  -- SPECIFIC EVENTS -- this approach creates an event group and only sends you events for the elements you describe. 
//	vector <string> list;
//	list.push_back("FIELD_DRAW_SCALE");
//	list.push_back("DIFF_MODE");
//	gui.createEventGroup("TEST_GROUP", list);
//	ofAddListener(gui.getEventGroup("TEST_GROUP"), this, &testApp::eventsIn);

//  -- PANEL EVENTS -- this approach gives you back an ofEvent for only the events from panel 0
//	ofAddListener(gui.getEventsForPanel(0), this, &testApp::eventsIn);

//  -- this gives you back an ofEvent for all events in this control panel object
	ofAddListener(gui.guiEvent, this, &testApp::eventsIn);

//  --EVENT FOR SINGLE GUI OBJECT
	ofAddListener(gui.createEventGroup("GRAB_BACKGROUND"), this, &testApp::grabBackgroundEvent);

}

// this is our callback function for the GRAB_BACKGROUND toggle - everytime it changes this gets fired
//--------------------------------------------------------------
void testApp::grabBackgroundEvent(guiCallbackData & data){
	
	//we use the event callback to capture the background - we then set the toggle value back to its previous value
	if( data.isElement( "GRAB_BACKGROUND" ) && data.getInt(0) == 1 ){
		bgExample.captureBackground();
		gui.setValueB("GRAB_BACKGROUND", false);
	}
}

//this captures all our control panel events - unless its setup differently in testApp::setup
//--------------------------------------------------------------
void testApp::eventsIn(guiCallbackData & data){

	//lets send all events to our logger
	if( !data.isElement( "events logger" ) ){
		string logStr = data.getXmlName();
		
		for(int k = 0; k < data.getNumValues(); k++){
			logStr += " - " + data.getString(k);
		}
		
		logger.log(OF_LOG_NOTICE, "event - %s", logStr.c_str());
	}
	
	// print to terminal if you want to 
	//this code prints out the name of the events coming in and all the variables passed
	printf("testApp::eventsIn - name is %s - \n", data.getXmlName().c_str());
	if( data.getDisplayName() != "" ){
		printf(" element name is %s \n", data.getDisplayName().c_str());
	}
	for(int k = 0; k < data.getNumValues(); k++){
		if( data.getType(k) == CB_VALUE_FLOAT ){
			printf("%i float  value = %f \n", k, data.getFloat(k));
		}
		else if( data.getType(k) == CB_VALUE_INT ){
			printf("%i int    value = %i \n", k, data.getInt(k));
		}
		else if( data.getType(k) == CB_VALUE_STRING ){
			printf("%i string value = %s \n", k, data.getString(k).c_str());
		}
	}
	
	printf("\n");
}

//--------------------------------------------------------------
void testApp::update(){
	//some dummy vars we will update to show the variable lister object
	elapsedTime		= ofGetElapsedTimef();
	appFrameCount	= ofGetFrameNum();	
	appFrameRate	= ofGetFrameRate();
	
	camDraw.setDrawScale(gui.getValueF("FIELD_DRAW_SCALE"));
	threshDraw.setDrawScale(gui.getValueF("FIELD_DRAW_SCALE"));
	
	grabber.update(); 

	if( grabber.isFrameNew() ){	
		if( gui.getSelectedPanel() == 0 ){
			bgExample.update(grabber.getPixels(), grabber.width, grabber.height);
		}else if( gui.getSelectedPanel() == 1 ){
			motion.update(grabber.getPixels(), grabber.width, grabber.height);
			stats.updateFromField(motion.motionField, 0.97);
		}
	}
	
	motion.setThreshold(gui.getValueI("MOTION_THRESHOLD"));
	motion.setFadeAmnt(gui.getValueF("MOTION_FADE"));
	
	bgExample.setDifferenceMode(gui.getValueI("DIFF_MODE"));
	bgExample.setThreshold(gui.getValueI("BG_THRESHOLD"));
	
	gui.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	gui.draw();
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	bool control_panel_ate_key = gui.keyPressed( key );

	if ( !control_panel_ate_key )
	{
		//if you need to adjust the camera properties
		if(key == 's'){
			grabber.videoSettings();
		}
	}	
	
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
