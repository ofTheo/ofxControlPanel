#pragma once
#include "guiBaseObject.h"

class guiTypeTextDropDown : public guiBaseObject{
public:

    void setup(vector <string> boxNames);
    void update( vector<string> boxNames );
//    virtual void updateValue();
    virtual string getSelectedStringValue() override;
    virtual void update() override;
    virtual void updateText() override;
    virtual void release(float x, float y, bool isRelative) override;
    void updateGui(float x, float y, bool firstHit, bool isRelative) override;

    void renderInternal();

    void render() override;
    void renderOnTop() override;

    void hideDropDown() { bShowDropDown = false; state = SG_STATE_NORMAL; }

    vector <string> vecDropList;
    bool    bShowDropDown;
    
#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    void addToRenderMesh( ofMesh& arenderMesh ) override;
    void addToLinesRenderMesh( ofMesh& arenderMesh ) override;
    virtual void addToTextRenderMesh( ofMesh& arenderMesh ) override;
    
    int boxHeight = 15;
    
//    ofMesh mTextMesh;
#endif
};

