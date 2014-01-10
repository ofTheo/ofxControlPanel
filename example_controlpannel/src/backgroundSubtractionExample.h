/*
 *  backgroundSubtractionExample.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 31/03/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

typedef enum{
	BG_ABS,
	BG_GREATER,
	BG_LESS
}captureMode;

class backgroundSubtractionExample{

	public:

        //------------
        void setup(int width, int height);
        void update(unsigned char * pixelsIn, int width, int height );

        //-----------
        //here we add up all the directions from the
        //vector field and average them out to an overall
        //direction - this will be quite small so you will want
        //to scale it up.

        //-----------
        void draw(float x, float y);
		
        void setThreshold(int threshVal);
		void setDifferenceMode(int modeIn);
		void captureBackground();
		
        ofxCvColorImage color;
        ofxCvGrayscaleImage gray;

        ofxCvGrayscaleImage background;
        ofxCvGrayscaleImage thresh;

		captureMode mode;

		int numFrames;
        float threshAmnt;
	
};