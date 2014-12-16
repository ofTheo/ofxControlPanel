#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"
#include "guiCustomImpl.h"

class guiTypeCustom : public guiBaseObject{

     public:

        guiTypeCustom();
        virtual ~guiTypeCustom();
    
        void setup(string customName, guiCustomImpl * customIn, float panelWidth, float panelHeight);
        virtual void saveSettings(string filename);
        virtual void loadSettings(string filename);
        virtual void update();
        void updateGui(float x, float y, bool firstHit, bool isRelative);
        virtual void release(float x, float y, bool isRelative);
        virtual void mouseMoved(float x, float y);
        void render();
    
    
    
        virtual bool keyPressed(int key);
        virtual bool keyReleased(int key);

        guiCustomImpl * custom;
};
