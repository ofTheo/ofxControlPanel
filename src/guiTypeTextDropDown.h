#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTypeTextDropDown : public guiBaseObject{

    public:

        //------------------------------------------------
        void setup(vector <string> boxNames);
        virtual void updateValue();
        virtual void update();
		virtual void updateText();
        virtual void release(float x, float y, bool isRelative);
        void updateGui(float x, float y, bool firstHit, bool isRelative);
        void render();
	
		string getSelectedBoxName() { return vecDropList[value.getValueI()]; }; 

		void notify();

		void hideDropDown() { bShowDropDown = false; state = SG_STATE_NORMAL; }
	
		vector <string> vecDropList;
		bool    bShowDropDown;
};

