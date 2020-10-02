#pragma once
#include "ofMain.h"
#include "ofxControlPanel.h"
#include "ofxOpenCv.h"

class  ofApp: public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void onGuiLoad( bool& ab );
    void onGuiSave( bool& ab );
    void onGuiSelectElement( shared_ptr<guiBaseObject>& aobj );
    void onFilePathChange( string& astr );
    void onToggleChange( int& aint );
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    
    ofxControlPanel gui;
    ofParameter<string> mFilePathStr;

    ofVideoGrabber  grabber;
    ofParameterGroup vars;

    ofxCvColorImage colorCV;
    ofxCvGrayscaleImage grayscaleCV;

    ofImage img;

    ofParameter <float> elapsedTime;
    ofParameter <int> appFrameCount;

    ofParameterGroup cvControls;
    ofParameter <bool> bInvert;
    ofParameter <int> threshold;
    
    ofParameter<string> filePickerStr;
    ofParameter<ofColor> paramColor;

    ofParameter <string> testLabel;
    ofParameter <string> status;
    ofParameter<int> cvDiffMode;
    
    ofParameter<int> multiToggleParam;
    
};
