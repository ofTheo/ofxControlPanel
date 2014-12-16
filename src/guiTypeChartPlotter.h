/*
 *  guiTypeChartPlotter.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 01/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"
#include "guiVariablePointer.h"

//class guiStatVarPointer : public guiVariablePointer{
//	public:
//		guiStatVarPointer(){
//			displayName = "none";
//			ptr			= NULL;
//			dataType	= GUI_VAR_FLOAT;		
//			precision   = 7;
//			autoUpdate  = true;		
//		}
//
//		guiStatVarPointer( string displayNameIn, void * varPtr, guiVarType theDataType, bool autoUpdateGraph, float floatPrecision = 7){
//			displayName = displayNameIn;
//			ptr			= varPtr;
//			dataType	= theDataType;		
//			precision   = floatPrecision;
//			autoUpdate  = autoUpdateGraph;
//		} 
//	
//		void setup( string displayNameIn, void * varPtr, guiVarType theDataType, bool autoUpdateGraph, float floatPrecision = 7){
//			displayName = displayNameIn;
//			ptr			= varPtr;
//			dataType	= theDataType;		
//			precision   = floatPrecision;
//			autoUpdate  = autoUpdateGraph;
//		} 
//		
//		void updateManually(){
//			bDoUpdate = true;
//		}
//
//		bool autoUpdate;
//		bool bDoUpdate;
//};

class guiTypeChartPlotter : public guiBaseObject{

    public:

        guiTypeChartPlotter(){
			minVal = 0.0;
			maxVal = 1.0;
			maxNum = 100;
		}

        void setup(float minValY, float maxValY){
            if( value.getNumValues() == 0 ){
                return;
            }
        
			name		   = value.paramGroup[0].getName();
			
			hitArea.width  = getDefaultColumnWidth();
			hitArea.height = getDefaultColumnWidth()/2;
            
            maxNum         = (int)hitArea.width;
			
			minVal         = minValY;
			maxVal		   = maxValY;
		}
    
        //-----------------------------------------------
        float getVerticalSpacing(){
            return 10.0;
        }

        //-----------------------------------------------
        void updateText(){
            
            drawStr = name + ": " + ofToString(value.getValueF(), 3);
            displayText.setText(drawStr);

            //now update our bounding box
            updateBoundingBox();
            
            boundingBox.width = MAX(hitArea.width, getDefaultColumnWidth());
        }
		
		//-----------------------------------------------
		void update(){
            if( value.getNumValues() == 0 ){
                return;
            }
            
			updateText();
			
            float valIn = value.getValueF(0);
            
            valueHistory.push_back( valIn );
            if( valueHistory.size() > maxNum ){
                valueHistory.erase(valueHistory.begin(), valueHistory.begin()+1);
            }
				
			
		}
		
		//-----------------------------------------------
		void render(){
            if( value.getNumValues() == 0 ){
                return;
            }
            
			ofPushStyle();
				
					glColor4fv(textColor.getColorF());				
					guiBaseObject::renderText();

					//draw the background
					ofFill();
					glColor4fv(bgColor.getColorF());
					ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
					
					float x = hitArea.x;
					float y = hitArea.y + hitArea.height;
					
					if( valueHistory.size() ){
						glColor4fv(fgColor.getSelectedColorF());
					
						ofNoFill();
						ofBeginShape();
						for(int i = 0; i < valueHistory.size(); i++){
							float xx = ofMap(i, 0, maxNum, x, x + hitArea.width, true);
							float yy = ofMap(valueHistory[i], minVal, maxVal, y, y - hitArea.height, true);
							
							ofVertex(xx, yy);
						}
						ofEndShape(false);
					}
					
					ofFill();

					glColor4fv(textColor.getColorF());				
					displayText.renderString("max: "+ofToString(maxVal, 0), x + 2, hitArea.y + displayText.getTextSingleLineHeight()+2);
					displayText.renderString("min: "+ofToString(minVal, 0), x + 2, y - 8);
					
					//draw the outline
					ofNoFill();
					glColor4fv(outlineColor.getColorF());
					ofRect(boundingBox);

			ofPopStyle();
		}
		
		float minVal, maxVal;
		vector <float> valueHistory;
		int maxNum;
		
		
//        
//		void updateGui(float x, float y, bool firstHit, bool isRelative);
//        void setKnobSize(float _knobSize);
//        
//		virtual void setValue(float _value, int whichParam);
//        virtual void updateValue();
//		
//		virtual void notify();	
//		
//        void render();

};
