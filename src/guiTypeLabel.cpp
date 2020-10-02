/*
 *  guiTypeLabel.cpp
 *  artvertiser
 *
 *  Created by damian on 10/10/10.
 *  Copyright 2010 frey damian@frey.co.nz. All rights reserved.
 *
 */


#include "guiTypeLabel.h"

//------------------------------------------------
void guiTypeLabel::setup(ofParameter<string> & label, bool highlight){
//    cout << " adding label with name " << label.getName() << " and value " << label << endl;
    textLabel.makeReferenceTo(label);
    
    guiBaseObject::setupNamesFromParams();
    
    bHighlight = highlight;
    prevString = "09m09asidjoasd890asdfasdsad";
    boundingBox = hitArea;
    internalUpdate();
}

//-----------------------------------------------.
void guiTypeLabel::internalUpdate(){
    string tstr = textLabel;//textLabel.getName();
//    if( str.length() ){
//        str += ": ";
//    }
    // only call setText if the text has changed //
    if( tstr != prevString ) {
        string str = textLabel.getName();
        if( str.length() ){
            str += ": ";
        }
        setText( str+tstr );
                
        prevString = tstr;
    }
    updateBoundingBox();
}

//-----------------------------------------------.
float guiTypeLabel::getVerticalSpacing(){
    if( bHighlight == false){
        return 0;
    }else{
        return 14;
    }
}

//-----------------------------------------------.
void guiTypeLabel::setText( string text ){
//	name = text;
    displayText.setText(text);
    hitArea.width = MAX(getDefaultColumnWidth(), displayText.getTextWidth(text) + 3);
//    cout << "guiTypeLabel :: calling setText: " << " | " << ofGetFrameNum() << endl;
    #ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    mTextMesh.clear();
    #endif
}

//-----------------------------------------------.
void guiTypeLabel::render(){
//    internalUpdate();
    
    #ifdef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    ofPushStyle(); {
        if( bHighlight ){
            ofSetColor(outlineColor.getColor() );
            ofDrawLine(boundingBox.x, boundingBox.y, boundingBox.x + boundingBox.width, boundingBox.y);
            ofDrawLine(boundingBox.x, boundingBox.y + boundingBox.height, boundingBox.x + boundingBox.width, boundingBox.y + boundingBox.height);
        }
    
        guiBaseObject::renderText();
    } ofPopStyle();
    #endif
}

#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
//-----------------------------------------------.
void guiTypeLabel::addToRenderMesh( ofMesh& arenderMesh ) {
    internalUpdate();
}

//-----------------------------------------------.
void guiTypeLabel::addToLinesRenderMesh( ofMesh& arenderMesh ) {
    if( bHighlight ){
        addLineToMesh( arenderMesh, boundingBox.x, boundingBox.y, boundingBox.getRight(), boundingBox.y, outlineColor.getColor() );
        addLineToMesh( arenderMesh, boundingBox.x, boundingBox.getBottom(), boundingBox.getRight(), boundingBox.getBottom(), outlineColor.getColor() );
    }
}

//-----------------------------------------------.
void guiTypeLabel::addToTextRenderMesh( ofMesh& arenderMesh ) {
    if( mTextMesh.getNumVertices() == 0 ) {
        mTextMesh.clear();
        displayText.addStringToMesh(mTextMesh, displayText.textString, boundingBox.x, boundingBox.y, textColor.getColor() );
    }
    
    if( mTextMesh.getNumVertices() > 0 ) {
        arenderMesh.append(mTextMesh);
    }
//    arenderMesh.addVertices( mTextMesh.getVertices() );
//    arenderMesh.addTexCoords( mTextMesh.getTexCoords() );
}
#endif






