/*
 *  guiTypeVairableLister.cpp
 *  ofxControlPanelDemo
 *
 *  Created by theo on 01/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "guiTypeVariableLister.h"
#include "guiValue.h"

//-----------------------------------------------.
void guiTypeVariableLister::setup(){
    cout << " setting name to " << value.paramGroup.getName() << endl;
	name = value.paramGroup.getName();
}

//-----------------------------------------------.
void guiTypeVariableLister::update(){
	updateBoundingBox();
}

//-----------------------------------------------
void guiTypeVariableLister::updateText(){
	
    drawStr = name + ":";
	displayText.setText(drawStr);

	//now update our bounding box
	updateBoundingBox();
}

//-----------------------------------------------.
void guiTypeVariableLister::updateBoundingBox(){

	minNameWidth = 20;
	minVarWidth  = 20;
	
	if( displayText.usingTTF() ){
		typeStartOffset = -4;
	}else{
		typeStartOffset = -2;
	}
					
	for(int i = 0; i < value.paramGroup.size(); i++){
		minNameWidth = MAX(minNameWidth, displayText.getTextWidth(value.paramGroup.getName(i)));
        
        string str = getVarAsString(i);
//        string str = "";
//        string ttype = value.getTypeAsString(i);
//
//        if( ttype == "float" ){
//            ofParameter <float> val = value.paramGroup.getFloat(i);
//            str = ofToString(val);
//        } else if( ttype == "int" ) {
//            ofParameter <int> val = value.paramGroup.getInt(i);
//            str = ofToString(val);
//        } else if(ttype == "bool" ) {
//            str = value.paramGroup.getBool(i) ? "true" : "false";
//        } else if( ttype == "string" ) {
//            str = value.paramGroup.getString(i);
//        }
        
		minVarWidth = MAX(minVarWidth, displayText.getTextWidth(str));
	}
	
	minNameWidth += 10;
	minVarWidth  += 5;
	
	hitArea.width  = MAX(hitArea.width, minNameWidth + minVarWidth);
	hitArea.height = MAX(20, value.paramGroup.size() * ( displayText.getTextSingleLineHeight() + 3));
	
    boundingBox.width = MAX(hitArea.width, getDefaultColumnWidth());
    
	if(bShowText){
		//we need to update out hitArea because the text will have moved the gui down
		hitArea.y = boundingBox.y + displayText.getTextHeight() + titleSpacing;
		boundingBox.height = hitArea.height + displayText.getTextHeight() + titleSpacing;
	}else{
		 //we need to update out hitArea because the text will have moved the gui down
		hitArea.y = boundingBox.y;
	}
}

//-----------------------------------------------.
void guiTypeVariableLister::render(){
	ofPushStyle();
		
			ofSetColor(textColor.getColor());				
			guiBaseObject::renderText();

			//draw the background


			ofFill();
			ofSetColor(bgColor.getColor());
			ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			float lineH = displayText.getTextSingleLineHeight();//hitArea.height / (float) MAX(1, value.paramGroup.size());
			
			float x		= hitArea.x;
			float y		= (hitArea.y + lineH) + typeStartOffset;
			float ly	= (hitArea.y + lineH);
			
			//draw the foreground
			for(int i = 0; i < value.paramGroup.size(); i++){
				
				ofSetColor(textColor.getColor());
				displayText.renderString(value.paramGroup.getName(i), x, y);
                
//                string str = "";
//                string ttype = value.getTypeAsString(i);
                
                string str = getVarAsString(i);
                
//                if( ttype == "float" ){
////                    ofParameter <float> val = value.paramGroup.getFloat(i);
//                    str = ofToString(value.paramGroup.getFloat(i));
//                } else if( ttype == "int" ) {
////                    ofParameter <int> val = value.paramGroup.getInt(i);
//                    str = ofToString(value.paramGroup.getInt(i));
//                } else if(ttype == "bool" ) {
//                    str = value.paramGroup.getBool(i) ? "true" : "false";
//                } else if( ttype == "string" ) {
//                    str = value.paramGroup.getString(i);
//                }
                
				displayText.renderString(str, x + minNameWidth, y);

				y  += lineH;
				ly += lineH;
			}
			
            //draw the outline
            ofNoFill();
            ofSetColor(outlineColor.getColor());
            ofDrawRectangle(boundingBox);
            
	ofPopStyle();
}
