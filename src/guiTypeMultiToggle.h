#pragma once
#include "guiBaseObject.h"

class guiTypeMultiToggle : public guiBaseObject{

    public:
        static const int boxSpacing = 2;
        static const int boxSize = 14;

        //------------------------------------------------
        void setup(vector <string> boxNames);
    
    virtual string getSelectedStringValue() override;

        //-----------------------------------------------
//		void notify();

        //-----------------------------------------------
//        virtual void updateValue();
        void updateGui(float x, float y, bool firstHit, bool isRelative) override;
        void render() override;
		
		vector <string> bNames;
};
