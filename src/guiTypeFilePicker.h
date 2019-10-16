//
//  guiTypeFilePicker.h
//  ofxControlPanel2
//
//  Created by Nick Hardeman on 10/4/19.
//

#pragma once
#include "guiBaseObject.h"

class guiTypeFilePicker : public guiBaseObject {
public:
    
    void setup( ofParameter<string>& aparam );
    virtual void updateText() override;
    void render() override;
    virtual void renderOnTop() override;
    
    virtual bool checkHit(float x, float y, bool isRelative) override;
    void updateGui(float x, float y, bool firstHit, bool isRelative) override;
    
protected:
    ofParameter<string> mParamRef;
    string mFilename = "";
    bool bShowToolTip = false;
};
