/*
 *  guiTypeChartPlotter.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 01/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "guiBaseObject.h"

class guiTypeChartPlotter : public guiBaseObject {
public:

    guiTypeChartPlotter(){
        minVal = 0.0;
        maxVal = 1.0;
        maxNum = 100;
    }

    virtual void setup(float minValY, float maxValY);
    
    float getVerticalSpacing();
    void updateText();
		
    virtual void update();
    void render();
    
#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    void addToRenderMesh( ofMesh& arenderMesh );
    void addToLinesRenderMesh( ofMesh& arenderMesh );
    virtual void addToTextRenderMesh( ofMesh& arenderMesh );
    
//    ofMesh mTextMesh;
#endif
		
    float minVal, maxVal;
    vector <float> valueHistory;
    int maxNum;

};

class guiTypeFPSChartPlotter : public guiTypeChartPlotter {
public:
    
    virtual void setup(float minValY, float maxValY) override {
        mFramerate.set("Framerate", 60.f );
        value.addValue(mFramerate);
        guiTypeChartPlotter::setup(minValY, maxValY);
    }
    virtual void update() override {
        mFramerate = ofGetFrameRate();
        guiTypeChartPlotter::update();
    }
    
protected:
    ofParameter<float> mFramerate;
};
