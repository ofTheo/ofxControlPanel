#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(127,127,127);
            
    ofSetVerticalSync(true);

    gui.setup("test cv", 0, 0, ofGetWidth(), 700);
    gui.addPanel("control panel test", 4, false);
            
    //--------- PANEL 1
    gui.setWhichPanel(0);
            
    gui.setWhichColumn(0);
    gui.addDrawableRect("video", &grabber, 200, 150);

    gui.addLabel("Loading images from disk"); 
    gui.addDrawableRect("image loaded from dir", &img, 200, 150);

    lister.listDir("of_logos/");
    gui.addFileLister("image lister", &lister, 200, 100);

    gui.setWhichColumn(1);
    gui.addDrawableRect("colorCV", &colorCV, 200, 150);

    gui.setWhichColumn(2);
    gui.addDrawableRect("grayscaleCV", &grayscaleCV, 200, 150);

    gui.setWhichColumn(3);
    
    cvControls.setName("cv controls");
    cvControls.add(bInvert.set("invert", false));
    cvControls.add(threshold.set("threshold", 29.0, 1.0, 255.0));
    
    gui.addGroup( cvControls ); 
    
    vars.setName("app vars");
    vars.add( appFrameCount.set("frame count", 0) );
    vars.add( appFrameRate.set("frame rate", 60.0) );
    vars.add( elapsedTime.set("elapsed time", 0.0) );
    
    gui.addVariableLister(vars);

    gui.addChartPlotter(appFrameRate, 30, 80);

    vector <string> names;
    names.push_back("abs diff");
    names.push_back("greater than");
    names.push_back("less than");
    gui.addTextDropDown("difference mode", 0, names);

    testLabel.set("currentPath", lister.getSelectedPath());
    gui.addLabel("a label");
    gui.addLabel(testLabel);
    
    status = "first frame";
    gui.setStatusMessage(status); 

    //SETTINGS AND EVENTS

    //load from xml!
    gui.loadSettings("controlPanelSettings.xml");

    //if you want to use events call this after you have added all your gui elements
    gui.setupEvents();
    gui.enableEvents();

    //  -- SPECIFIC EVENTS -- this approach creates an event group and only sends you events for the elements you describe. 
    //        vector <string> list;
    //        list.push_back("FIELD_DRAW_SCALE");
    //        list.push_back("DIFF_MODE");
    //        gui.createEventGroup("TEST_GROUP", list);
    //        ofAddListener(gui.getEventGroup("TEST_GROUP"), this, &ofApp::eventsIn);

    //  -- PANEL EVENTS -- this approach gives you back an ofEvent for only the events from panel 0
    //        ofAddListener(gui.getEventsForPanel(0), this, &ofApp::eventsIn);

    //  -- this gives you back an ofEvent for all events in this control panel object
    ofAddListener(gui.guiEvent, this, &ofApp::eventsIn);

    grabber.initGrabber(320, 240);

}

//--------------------------------------------------------------
void ofApp::update(){
    gui.update();
    
    grabber.update();
    colorCV.setFromPixels(grabber.getPixelsRef());
    grayscaleCV = colorCV;
    grayscaleCV.threshold(gui.getValueI("cv_controls:threshold"), bInvert);
    
    appFrameRate = ofGetFrameRate();
    elapsedTime = ofGetElapsedTimef();
    appFrameCount = ofGetFrameNum();

    testLabel = lister.getSelectedPath();
    
    status = "App running at " + ofToString(ofGetFrameRate());

}

//--------------------------------------------------------------
void ofApp::eventsIn(guiCallbackData & data){
     // print to terminal if you want to 
        //this code prints out the name of the events coming in and all the variables passed
        printf("ofApp::eventsIn - name is %s - \n", data.getXmlName().c_str());
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
    
        if( data.getXmlName() == "IMAGE_LISTER" ){
            img.loadImage(data.getString(1));
        }
}


//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}