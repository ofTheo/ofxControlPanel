#pragma once

#include "ofMain.h"

class guiValue{

    public:
        guiValue();
        ~guiValue();
    
       //------------------------------------------------
        bool addValue( ofAbstractParameter & param);

      //------------------------------------------------
        bool setValue(float val, int which = 0);

        //------------------------------------------------
        bool setValueAsPct(float percent, int which = 0);
    
        string getTypeAsString(int which = 0);

       //------------------------------------------------
        float getValueF(int which = 0);

       //------------------------------------------------
        float getValueI(int which = 0);

       //------------------------------------------------
        float getValueB(int which = 0);

       //------------------------------------------------
        string getValueAsString(int which = 0);

        //------------------------------------------------
        float getMin(int which = 0);

        //------------------------------------------------
        float getMax(int which = 0);

        //------------------------------------------------
		void setMin(float newMin, int which = 0);
		
        //------------------------------------------------						
		void setMax(float newMax, int which = 0);

        //------------------------------------------------
        float getPct(int which = 0);

        //------------------------------------------------
        int getNumValues();

        ofParameterGroup paramGroup;

    protected:

        //-----------------------------------------------
        void updatePct();

        //--------------
        //--------------
    
    
};

