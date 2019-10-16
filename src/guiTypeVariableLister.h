/*
 *  guiTypeVairableLister.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 01/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "guiBaseObject.h"
//#include "guiVariablePointer.h"

class guiTypeVariableLister : public guiBaseObject{
public:

    void setup();
    void update();

    //-----------------------------------------------
    float getVerticalSpacing(){
        return 10.0;
    }

    void updateText();
    void updateBoundingBox();
    void render();
                    
    float minNameWidth = 24, minVarWidth=24;
    float typeStartOffset = 0;
};
