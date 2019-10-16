#pragma once
#include "guiBaseObject.h"
#include "guiTypeTextInput.h"

class guiTypeSlider : public guiBaseObject{
public:
    
    guiTypeSlider();
    ~guiTypeSlider();

    void setup();
    virtual void updateBoundingBox() override;

    //-----------------------------------------------
    virtual float getVerticalSpacing() override {
        return 5.0;
    }
    
//    virtual void updateText() override;
    virtual bool checkHit(float x, float y, bool isRelative) override;
    void updateGui(float x, float y, bool firstHit, bool isRelative = false) override;
    void render() override;
    
    
    
#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    virtual void addToRenderMesh( ofMesh& arenderMesh ) override;
    virtual void addToLinesRenderMesh( ofMesh& arenderMesh ) override;
    virtual void addToTextRenderMesh( ofMesh& arenderMesh ) override;
    
    ofMesh mTextMesh;
#endif
    
    bool bShowDefaultValue = false;
    float defaultValue = 0.0;
    
    shared_ptr<guiTypeTextInput> mTextInput;
};
