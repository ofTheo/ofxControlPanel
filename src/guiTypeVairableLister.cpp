/*
 *  guiTypeVairableLister.cpp
 *  ofxControlPanelDemo
 *
 *  Created by theo on 01/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "guiTypeVairableLister.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

//-----------------------------------------------.
guiTypeVairableLister::guiTypeVairableLister(){
	typeStartOffset = 0;
    
}

//-----------------------------------------------.
void guiTypeVairableLister::setup(){
    cout << " setting name to " << value.paramGroup.getName() << endl;
	name = value.paramGroup.getName();
}

//-----------------------------------------------.
void guiTypeVairableLister::update(){			
	updateBoundingBox();
}

//-----------------------------------------------
void guiTypeVairableLister::updateText(){
	
    drawStr = name + ":";
	displayText.setText(drawStr);

	//now update our bounding box
	updateBoundingBox();
}

//-----------------------------------------------.
void guiTypeVairableLister::updateBoundingBox(){

	minNameWidth = 20;
	minVarWidth  = 20;
	
	if( displayText.usingTTF() ){
		typeStartOffset = -4;
	}else{
		typeStartOffset = -2;
	}
					
	for(int i = 0; i < value.paramGroup.size(); i++){
		minNameWidth = MAX(minNameWidth, displayText.getTextWidth(value.paramGroup.getName(i)));
        
        string str = "";
        
        if( value.getTypeAsString(i) == "float" ){
            ofParameter <float> val = value.paramGroup.getFloat(i);
            str = ofToString(val);
        }else{
            ofParameter <int> val = value.paramGroup.getInt(i);
            str = ofToString(val);
        }
        
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
void guiTypeVairableLister::render(){
	ofPushStyle();
		
			glColor4fv(textColor.getColorF());				
			guiBaseObject::renderText();

			//draw the background


			ofFill();
			glColor4fv(bgColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			float lineH = displayText.getTextSingleLineHeight();//hitArea.height / (float) MAX(1, value.paramGroup.size());
			
			float x		= hitArea.x;
			float y		= (hitArea.y + lineH) + typeStartOffset;
			float ly	= (hitArea.y + lineH);
			
			//draw the foreground
			for(int i = 0; i < value.paramGroup.size(); i++){
				
				glColor4fv(textColor.getColorF());
				displayText.renderString(value.paramGroup.getName(i), x, y);
                
                string str = "";
                
                if( value.getTypeAsString(i) == "float" ){
                    ofParameter <float> val = value.paramGroup.getFloat(i);
                    str = ofToString(val);
                }else{
                    ofParameter <int> val = value.paramGroup.getInt(i);
                    str = ofToString(val);
                }
                
				displayText.renderString(str, x + minNameWidth, y);

				y  += lineH;
				ly += lineH;
			}
			
            //draw the outline
            ofNoFill();
            glColor4fv(outlineColor.getColorF());
            ofRect(boundingBox);
            
	ofPopStyle();
}