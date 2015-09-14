#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiType2DSlider : public guiBaseObject{

    public:

        guiType2DSlider();

        void setup();
        
		void updateGui(float x, float y, bool firstHit, bool isRelative);
        void setKnobSize(float _knobSize);
        
		void updateBoundingBox();
		
		virtual void setValue(float _value, int whichParam);
        virtual void updateValue();
		
		virtual void notify();	
		
        void render();

        float knobSize;

};
