#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(127,127,127);
            
    ofSetVerticalSync(true);

    gui.setup("test cv", 0, 0);
    gui.addPanel("control panel test", 4, false);
            
    //--------- PANEL 1
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    gui.addFpsChartPlotter();

    gui.addLabel("Loading images from disk"); 
    gui.addDrawableRect("image loaded from dir", &img, 320, 240);

    auto lister = gui.addFileLister( mFilePathStr.set("Image Lister", ""), "of_logos/" );
    lister->setDimensions(lister->getWidth(),200);

    gui.setWhichColumn(2);
    gui.addDrawableRect("video", &grabber, 200, 150);
    gui.addDrawableRect("colorCV", &colorCV, 200, 150);
    
    //use ofParameterGroup to create sets of sliders/parameters
    cvControls.setName("cv controls");
    cvControls.add(bInvert.set("invert", false));
    cvControls.add(threshold.set("threshold", 29.0, 1.0, 255.0));
    gui.add( cvControls );
    
    vector <string> names;
    names.push_back("abs diff");
    names.push_back("greater than");
    names.push_back("less than");
    gui.addTextDropDown(cvDiffMode.set("difference mode", 0), names);
    
    gui.addDrawableRect("grayscaleCV", &grayscaleCV, 200, 150);

    gui.setWhichColumn(3);
    vars.setName("app vars");
    vars.add( appFrameCount.set("frame count", 0) );
    vars.add( elapsedTime.set("elapsed time", 0.0) );
    
    gui.addVariableLister(vars);
    
    gui.addFilePicker( filePickerStr.set("PickedFile", "") );
    gui.addColorPicker( paramColor.set("PickedColor", ofColor(4,89,163, 255) ) );

    gui.addLabel("a label");
    gui.addLabel(testLabel.set("Test Label", "0"));
    
    gui.addMultiToggle( multiToggleParam.set("MultiTogg", 0), { "Option 1", "Option 2", "Option 3" } );
    
    status = "first frame";
    gui.setStatusMessage(status); 

    //SETTINGS AND EVENTS
    multiToggleParam.addListener(this, &ofApp::onToggleChange );
    mFilePathStr.addListener( this, &ofApp::onFilePathChange );
    filePickerStr.addListener( this, &ofApp::onFilePathChange );
    
    ofAddListener(gui.guiLoadEvent, this, &ofApp::onGuiLoad );
    ofAddListener(gui.guiSaveEvent, this, &ofApp::onGuiSave );

    //load from xml!
    gui.loadSettings("controlPanelSettings.xml");

    grabber.initGrabber(320, 240);

}

//--------------------------------------------------------------
void ofApp::update(){
    gui.update();
    
    grabber.update();
    if( grabber.isFrameNew() ) {
        colorCV.setFromPixels(grabber.getPixels());
        grayscaleCV = colorCV;
        grayscaleCV.threshold(threshold, bInvert);
    }
    
    elapsedTime = ofGetElapsedTimef();
    appFrameCount = ofGetFrameNum();

    testLabel = ofToString(ofGetFrameNum());//.getSelectedPath();
    
    status = "App running at " + ofToString(ofGetFrameRate());

}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor( paramColor );
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight() );
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::onGuiLoad( bool& ab ) {
    cout << "onGuiLoad : " << ab << " | " << ofGetFrameNum() << endl;
}

//--------------------------------------------------------------
void ofApp::onGuiSave( bool& ab ) {
    cout << "onGuiSave : " << ab << " | " << ofGetFrameNum() << endl;
}

//--------------------------------------------------------------
void ofApp::onFilePathChange( string& astr ) {
    cout << "onFilePathChange: " << astr << " | " << ofGetFrameNum() << endl;
    if( ofFile::doesFileExist(astr)) {
        string ext = ofToLower(ofFilePath::getFileExt(astr));
        if( ext == "png" || ext == "jpg" ) {
            img.load(astr);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::onToggleChange( int& aint ) {
    // get the multi toggle
    auto mtogg = gui.get< guiTypeMultiToggle >( multiToggleParam );
    if( mtogg ) {
        cout << "onToggleChange :: " << mtogg->getSelectedStringValue() << endl;
    }
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
