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
    virtual void update() override;
    void updateGui(float x, float y, bool firstHit, bool isRelative) override; // mouse dragged //
    virtual void release(float x, float y, bool isRelative) override;
    virtual void mouseMoved(float x, float y) override;
    void render() override;
    
    virtual bool keyPressed(int key) override;
    virtual bool keyReleased(int key) override;

    guiCustomImpl * custom = NULL;
};
