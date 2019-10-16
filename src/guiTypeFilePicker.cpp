//
//  guiTypeFilePicker.cpp
//  ofxControlPanel2
//
//  Created by Nick Hardeman on 10/4/19.
//

#include "guiTypeFilePicker.h"

//--------------------------------------------------------------
void guiTypeFilePicker::setup( ofParameter<string>& aparam ) {
    value.addValue(aparam);
    
    mParamRef.makeReferenceTo( aparam );
    guiBaseObject::setupNamesFromParams();
    
    setDimensions(getDefaultColumnWidth(), 34);
    
    updateText();
}

//every time we update the value of our text
void guiTypeFilePicker::updateText() {
        
    varsString = getVarsAsString();
    
    drawStr = name;//+": ";
    mFilename = "none";
    if( mParamRef.get() != "" ) {
        if(ofFile::doesFileExist(mParamRef.get())) {
            mFilename = ofFilePath::getBaseName(mParamRef.get())+"."+ofFilePath::getFileExt(mParamRef.get());
        }
    }
//    if( selection > -1 && mParamRef.get() != "") {
//        drawStr += lister->getSelectedName();
//    } else {
//        drawStr += "na";
//    }
    displayText.setText(drawStr);
    
    hitArea.x = boundingBox.getRight()-60;
    hitArea.width = 60.f;
    hitArea.height = 16;
//    hitArea.y = boundingBox.y + 16;

    //now update our bounding box
//    updateBoundingBox();
}

//-----------------------------------------------.
void guiTypeFilePicker::render(){
    ofPushStyle(); {

        ofPushMatrix(); {
            //draw the background
            ofFill();
//            ofSetColor(bgColor.getColor());
//            ofDrawRectangle(boundingBox.x, boundingBox.y+16, boundingBox.width, boundingBox.height-16);

            ofSetColor(textColor.getColor());
            guiBaseObject::renderText();

            ofFill();
            ofSetColor(fgColor.getColor());
            ofDrawRectangle( hitArea );
            ofSetColor(textColor.getColor());
            ofDrawBitmapString( "select", hitArea.getLeft() + 6, hitArea.getBottom()-4 );
            ofDrawBitmapString(mFilename, boundingBox.x, boundingBox.y+30);

            ofNoFill();
            ofSetColor(outlineColor.getColor());
            ofDrawRectangle(boundingBox.x, boundingBox.y+18, boundingBox.width, boundingBox.height-18);
//            ofDrawRectangle(hitArea.x , hitArea.y, sliderWidth, hitArea.height);

//            ofSetColor(textColor.getColor());
//            if(lister) {
//                drawRecords(hitArea.x+sliderWidth + 5, hitArea.y, boundingBox.width-(sliderWidth + 5), boundingBox.height);
//            }

        } ofPopMatrix();

    } ofPopStyle();
}

//-----------------------------------------------.
void guiTypeFilePicker::renderOnTop() {
    if( bShowToolTip ) {
        ofDrawBitmapStringHighlight("file: "+mParamRef.get(), boundingBox.x, boundingBox.y+8 );
    }
}

//-----------------------------------------------.
bool guiTypeFilePicker::checkHit(float x, float y, bool isRelative) {
    bool tbhit = guiBaseObject::checkHit(x, y, isRelative );
    
    if( state == SG_STATE_SELECTED ){
        bShowToolTip = false;
    } else {
        ofRectangle trect(boundingBox.x, boundingBox.y+18, boundingBox.width, boundingBox.height-18);
        if( trect.inside(x, y)) {
            bShowToolTip = !bShowToolTip;
        } else {
            bShowToolTip = false;
        }
    }
    
    return tbhit;
}

//-----------------------------------------------.
void guiTypeFilePicker::updateGui(float x, float y, bool firstHit, bool isRelative){
    
    if( state == SG_STATE_SELECTED ){
        bShowToolTip=false;
        // open a file dialog //
        auto fresult = ofSystemLoadDialog("Select a file");
        if( fresult.bSuccess ) {
            if( ofFile::doesFileExist(fresult.getPath())) {
                auto dpath = ofToDataPath("", true);
//                cout << "dpath: " << dpath << endl;
                mParamRef = ofFilePath::makeRelative( dpath, fresult.getPath() );
//                mParamRef = ofToDataPath( fresult.getPath() );
                updateText();
            }
        }
        state = SG_STATE_NORMAL;
        setNormal();
    }
}



