#pragma once


#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTypeSlider : public guiBaseObject{

    public:

        //------------------------------------------------
        void setup();

		//---------------------------------------------
		virtual void updateBoundingBox();
    
        //-----------------------------------------------
        float getVerticalSpacing(){
            return 5.0;
        }

        virtual void updateValue();
		void updateGui(float x, float y, bool firstHit, bool isRelative = false);
		void render();

};
