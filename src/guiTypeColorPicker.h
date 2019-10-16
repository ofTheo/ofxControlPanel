//
//  guiTypeColorPicker.h
//  ofxControlPanel2
//
//  Created by Nick Hardeman on 10/16/19.
//

#pragma once
#include "guiBaseObject.h"
#include "guiTypeTextInput.h"

class guiTypeColorPicker : public guiBaseObject {
public:
    
    guiTypeColorPicker();
    ~guiTypeColorPicker();
    
    void setup( ofParameter<ofColor>& acolor );
    virtual void updateText() override;
    virtual void updateBoundingBox() override;
    virtual bool checkHit(float x, float y, bool isRelative) override;
    
    virtual void addToRenderMesh( ofMesh& arenderMesh ) override;
    virtual void addToLinesRenderMesh( ofMesh& arenderMesh ) override;
    virtual void addToTextRenderMesh( ofMesh& arenderMesh ) override;
    
    ofMesh mTextMesh;
    
protected:
    void _onColorParamChange( ofColor& av );
    void _onColorComponentInputChange( string& s );
    // lets make some params to track each rgba value //
    vector< ofParameter<int> > mComponentParams;
    vector< shared_ptr<guiTypeTextInput> > mTextInputs;
    ofRectangle mColorRect;
    ofParameter<ofColor> mParamRef;
};
