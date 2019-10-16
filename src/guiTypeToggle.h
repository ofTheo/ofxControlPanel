#pragma once
	
#include "guiBaseObject.h"

class guiTypeToggle : public guiBaseObject{
public:

    void setup();
    virtual void updateValue();
    virtual void updateText();
    void updateGui(float x, float y, bool firstHit, bool isRelative);
    void render();
//		void notify();

    float getWidth() { return guiBaseObject::getWidth()+labelWidth; }
    
    int labelWidth;
    void updateBoundingBox();
    
    #ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    virtual void addToRenderMesh( ofMesh& arenderMesh );
    virtual void addToLinesRenderMesh( ofMesh& arenderMesh );
    virtual void addToTextRenderMesh( ofMesh& arenderMesh );
    ofMesh mTextMesh;
    #endif
};
