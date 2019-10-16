#pragma once
#include "guiBaseObject.h"
#include "guiCustomImpl.h"

class guiTypeCustom : public guiBaseObject{
public:

    guiTypeCustom();
    virtual ~guiTypeCustom();

    void setup(string customName, guiCustomImpl * customIn, float panelWidth, float panelHeight);
    virtual bool saveSettings(string filename) override;
    virtual bool loadSettings(string filename) override;
    virtual void update();
    void updateGui(float x, float y, bool firstHit, bool isRelative); // mouse dragged //
    virtual void release(float x, float y, bool isRelative);
    virtual void mouseMoved(float x, float y);
    void render();
    
    virtual bool keyPressed(int key);
    virtual bool keyReleased(int key);

    guiCustomImpl * custom = NULL;
};
