#pragma once
#include "guiBaseObject.h"

class guiTypeRangeSlider : public guiBaseObject {
public:

    void setup();
    
    void updateGui(float x, float y, bool firstHit, bool isRelative = false) override;
    void updateBoundingBox() override;
    virtual void onEnabledChanged() override;
    
    virtual void setValue(float _value, int whichParam) override;
    
    void render() override;
    
#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    void addToRenderMesh( ofMesh& arenderMesh ) override;
    void addToLinesRenderMesh( ofMesh& arenderMesh ) override;
    virtual void addToTextRenderMesh( ofMesh& arenderMesh ) override;
    
//    ofMesh mTextMesh;
#endif
    
    bool bShowDefaultValue = false;
    
protected:
    int mSelectedIndex = -1;
    float defaultValues[2];
};
