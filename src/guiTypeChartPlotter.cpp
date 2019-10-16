/*
 *  guiTypeChartPlotter.cpp
 *  ofxControlPanelDemo
 *
 *  Created by theo on 01/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "guiTypeChartPlotter.h"

//-----------------------------------------------
void guiTypeChartPlotter::setup(float minValY, float maxValY){
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
float guiTypeChartPlotter::getVerticalSpacing(){
    return 10.0;
}

//-----------------------------------------------
void guiTypeChartPlotter::updateText(){
    
    drawStr = name + ": " + ofToString(value.getValueF(), 3);
    displayText.setText(drawStr);
    
    //now update our bounding box
    updateBoundingBox();
    
    boundingBox.width = MAX(hitArea.width, getDefaultColumnWidth());
}

//-----------------------------------------------
void guiTypeChartPlotter::update(){
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
void guiTypeChartPlotter::render(){
    if( value.getNumValues() == 0 ){
        return;
    }
    
#ifdef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    ofPushStyle(); {
        
        ofSetColor(textColor.getColor());
        guiBaseObject::renderText();
        
        //draw the background
        ofFill();
        ofSetColor(bgColor.getColor());
        ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
        
        float x = hitArea.x;
        float y = hitArea.y + hitArea.height;
        
        if( valueHistory.size() ){
            ofSetColor(fgColor.getSelectedColor());
            
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
        
        ofSetColor(textColor.getColor());
        displayText.renderString("max: "+ofToString(maxVal, 0), x + 2, hitArea.y + displayText.getTextSingleLineHeight()+2);
        displayText.renderString("min: "+ofToString(minVal, 0), x + 2, y - 8);
        
        //draw the outline
        ofNoFill();
        ofSetColor(outlineColor.getColor());
        ofDrawRectangle(boundingBox);
        
    } ofPopStyle();
#endif
}

#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
//-----------------------------------------------
void guiTypeChartPlotter::addToRenderMesh( ofMesh& arenderMesh ) {
    addRectangleToMesh( arenderMesh, hitArea, bgColor.getColor() );
}

//-----------------------------------------------
void guiTypeChartPlotter::addToLinesRenderMesh( ofMesh& arenderMesh ) {
    addRectangleToLinesMesh( arenderMesh, boundingBox, outlineColor.getColor() );
    
    float x = hitArea.x;
    float y = hitArea.y + hitArea.height;
    
    
    if( valueHistory.size() > 1 ){
        
        int voffset = arenderMesh.getNumVertices();
        
        ofFloatColor tcolor = fgColor.getSelectedColor();
        
        ofVec3f curr;
        for(int i = 0; i < valueHistory.size(); i++){
            float xx = ofMap(i, 0, maxNum, x, x + hitArea.width, true);
            float yy = ofMap(valueHistory[i], minVal, maxVal, y, y - hitArea.height, true);
            curr.set( xx, yy );
            arenderMesh.addVertex(curr);
            arenderMesh.addColor(tcolor);
        }
        
        for(int i = 0; i < valueHistory.size()-1; i++){
            arenderMesh.addIndex( voffset + i );
            arenderMesh.addIndex( voffset + i+1 );
        }
    }
    
}

//-----------------------------------------------
void guiTypeChartPlotter::addToTextRenderMesh( ofMesh& arenderMesh ) {
    displayText.addStringToMesh( arenderMesh, displayText.textString, boundingBox.x + 2, boundingBox.y+displayText.getTextSingleLineHeight(), textColor.getColor() );
    
    float x = hitArea.x;
    float y = hitArea.y + hitArea.height;
    displayText.addStringToMesh( arenderMesh, "max: "+ofToString(maxVal, 0), x+2, hitArea.y+displayText.getTextSingleLineHeight()+2, textColor.getColor() );
    displayText.addStringToMesh( arenderMesh, "min: "+ofToString(minVal, 0), x+2, y-displayText.getTextSingleLineHeight()-8, textColor.getColor() );
}

#endif