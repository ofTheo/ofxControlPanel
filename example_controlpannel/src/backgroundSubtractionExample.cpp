/*
 *  backgroundSubtractionExample.cpp
 *  ofxControlPanelDemo
 *
 *  Created by theo on 31/03/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "backgroundSubtractionExample.h"

//------------
void backgroundSubtractionExample::setup(int width, int height){

	color.allocate(width, height);
	gray.allocate(width, height);
	thresh.allocate(width, height);
	background.allocate(width, height);

	threshAmnt  = 29;
	numFrames	= 0;
}

//-----------
void backgroundSubtractionExample::update(unsigned char * pixelsIn, int width, int height ){
	color.setFromPixels(pixelsIn, width, height);

	//convert the color image to grayscale
	gray = color;
	
	if( numFrames < 20 ){
		background = gray;
	}	

	if( mode == BG_ABS ){
		//do absolute diff between the prev frame and current frame
		//all pixels that are different will show up as non-black
		thresh.absDiff(gray, background);
	}else if( mode == BG_GREATER ){
		thresh = gray;
		thresh -= background;
	}else if( mode == BG_LESS ){
		thresh = background;
		thresh -= gray;	
	}
	
	//threshold to 0 or 255 value
	thresh.threshold(threshAmnt);
	
	numFrames++;
}

//-----------
void backgroundSubtractionExample::draw(float x, float y){
	ofSetColor(0xFFFFFF);
	gray.draw(x, y);
	background.draw(x + gray.width,  y);
	thresh.draw(x, y + gray.height);
}

//-----------
void backgroundSubtractionExample::setThreshold(int threshVal){
	threshAmnt = (int)threshVal;
}

//-----------
void backgroundSubtractionExample::captureBackground(){
	background = gray;
}

//-----------
void backgroundSubtractionExample::setDifferenceMode(int modeIn){
	mode = (captureMode)modeIn;
}



