#pragma once
#include "guiBaseObject.h"

class guiTypeRangeSlider : public guiBaseObject {
public:

    void setup();
    
    void updateGui(float x, float y, bool firstHit, bool isRelative = false);
    void updateBoundingBox();
    
    virtual void setValue(float _value, int whichParam);
    
    void render();
    
#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    void addToRenderMesh( ofMesh& arenderMesh );
    void addToLinesRenderMesh( ofMesh& arenderMesh );
    virtual void addToTextRenderMesh( ofMesh& arenderMesh );
    
    ofMesh mTextMesh;
#endif
    
    bool bShowDefaultValue = false;
    
protected:
    int mSelectedIndex = -1;
    float defaultValues[2];
};
