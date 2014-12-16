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
    guiBaseObject::setupNamesFromParams(); 

    textLabel.makeReferenceTo(label);
    bHighlight = highlight;
    
    internalUpdate();
    boundingBox = hitArea;
}

//-----------------------------------------------.
void guiTypeLabel::internalUpdate(){
    string str = textLabel.getName();
    if( str.length() ){
        str += ": ";
    }
        
    setText(  str );
}

//-----------------------------------------------.
float guiTypeLabel::getVerticalSpacing(){
    if( bHighlight == false){
        return 0.0;
    }else{
        return 14;
    }
}

//-----------------------------------------------.
void guiTypeLabel::setText( string text ){
	name = text;
    hitArea.width = MAX(getDefaultColumnWidth(), displayText.getTextWidth(text) + 3);
}

//-----------------------------------------------.
void guiTypeLabel::render(){
    internalUpdate();
    
	ofPushStyle();
    
        if( bHighlight ){
            glColor4fv(outlineColor.getColorF());
            ofLine(boundingBox.x, boundingBox.y, boundingBox.x + boundingBox.width, boundingBox.y);
            ofLine(boundingBox.x, boundingBox.y + boundingBox.height, boundingBox.x + boundingBox.width, boundingBox.y + boundingBox.height);
        }
    
        guiBaseObject::renderText();
	ofPopStyle();
}
