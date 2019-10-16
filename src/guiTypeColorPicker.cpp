//
//  guiTypeColorPicker.cpp
//  ofxControlPanel2
//
//  Created by Nick Hardeman on 10/16/19.
//

#include "guiTypeColorPicker.h"

guiTypeColorPicker::guiTypeColorPicker() {
    
}

guiTypeColorPicker::~guiTypeColorPicker() {
    children.clear();
    
    if( mTextInputs.size() ) {
        mParamRef.removeListener(this, &guiTypeColorPicker::_onColorParamChange );
    }
    
    for( int i = 0; i < mTextInputs.size(); i++ ) {
        ofRemoveListener( mTextInputs[i]->textEnteredEvent, this, &guiTypeColorPicker::_onColorComponentInputChange );
    }
    
    mTextInputs.clear();
    
//    for( int i = 0; i < mComponentParams.size(); i++ ) {
//        mComponentParams[i].removeListener(this, &guiTypeColorPicker::_onColorComponentChange);
//    }
    mComponentParams.clear();
}

//--------------------------------------------------------------
void guiTypeColorPicker::setup( ofParameter<ofColor>& acolor ) {
    value.addValue(acolor);
    mParamRef.makeReferenceTo(acolor);
    
    guiBaseObject::setupNamesFromParams();
    
    mParamRef.addListener(this, &guiTypeColorPicker::_onColorParamChange );
    
    ofColor tcolor = acolor;
    mComponentParams.clear();
    for( int i = 0; i < 4; i++ ) {
        ofParameter<int> pai;
        pai = (int)tcolor[i];
        pai.setMin(0);
        pai.setMax(255);
//        pai.addListener(this, &guiTypeColorPicker::_onColorComponentChange );
        mComponentParams.push_back(pai);
        
        auto ti = make_shared<guiTypeTextInput>();
        ti->setup(pai);
        ofAddListener( ti->textEnteredEvent, this, &guiTypeColorPicker::_onColorComponentInputChange );
        mTextInputs.push_back(ti);
        children.push_back(ti);
    }
    
    setDimensions(getDefaultColumnWidth(), 34);
}

//every time we update the value of our text
//-----------------------------------------------
void guiTypeColorPicker::updateText() {
    
    varsString = getVarsAsString();
    drawStr = name;
//    if( varsString.length() ){
//        drawStr += varsString;
//    }

    displayText.setText(drawStr);

    //now update our bounding box
    updateBoundingBox();
}

//---------------------------------------------
void guiTypeColorPicker::updateBoundingBox() {
    hitArea.y = boundingBox.y + displayText.getTextHeight() + 4.0;
    if( bShowText ){
        hitArea.height = displayText.getTextHeight() + 3.0;
        mColorRect.y = hitArea.y;
        mColorRect.height = hitArea.height;
        mColorRect.x = hitArea.x;
        mColorRect.width = 48;
        
        
        float tw = displayText.getTextWidth("255")+4;
        float sx = mColorRect.width + 16;
        float availW = boundingBox.width-sx;
        float spacing = (availW - (4.f*tw)) / 3.f;
        
        for(int i = 0; i < mTextInputs.size(); i++ ) {
            // local to this hit Area
            mTextInputs[i]->hitArea = ofRectangle( sx, 2, tw, hitArea.height - 2 );
            sx += spacing + tw;
        }
    }
}

//-------------------------------------------
bool guiTypeColorPicker::checkHit(float x, float y, bool isRelative) {
    bool bHit = guiBaseObject::checkHit( x,y,isRelative);
    if( !bHit ) {
        for( auto& ti : mTextInputs ) {
            ti->setFocus(false);
        }
    }
    return bHit;
}

//---------------------------------------------
void guiTypeColorPicker::addToRenderMesh( ofMesh& arenderMesh ) {
    addRectangleToMesh( arenderMesh, hitArea, bgColor.getColor() );
    addRectangleToMesh( arenderMesh, mColorRect, mParamRef.get() );
    
    for( auto& ti : mTextInputs ) {
        ti->addToRenderMesh( arenderMesh, hitArea.x, hitArea.y );
    }
//    float textW = displayText.getTextWidth(varsString);
//    float rightAlignVarsX = hitArea.width - textW;
//    float rectW = textW+4.0;
//    addRectangleToMesh(arenderMesh, ofRectangle(hitArea.getRight()-rectW, hitArea.y+2, rectW, hitArea.height-3), ofFloatColor(0.0,0.0,0.0, 0.7));
}

//---------------------------------------------
void guiTypeColorPicker::addToLinesRenderMesh( ofMesh& arenderMesh ) {
    addRectangleToLinesMesh( arenderMesh, hitArea, outlineColor.getColor() );
}

//---------------------------------------------
void guiTypeColorPicker::addToTextRenderMesh( ofMesh& arenderMesh ) {
//    addStringToMesh(ofMesh &amesh, ofBitmapFont& aBitmapFont, string aString, float ax, float ay )
//    name, hitArea.x , hitArea.y + displayText.getTextSingleLineHeight()
    if( mTextMesh.getNumVertices() == 0 ) {
        mTextMesh.clear();
        displayText.addStringToMesh(mTextMesh, name, boundingBox.x+2, boundingBox.y, textColor.getColor() );
    //    addStringToMesh( arenderMesh, afont, name, hitArea.x, hitArea.y + displayText.getTextSingleLineHeight(), textColor.getColor() );
    }
    
    for( auto& ti : mTextInputs ) {
        ti->addToTextRenderMesh( arenderMesh, hitArea.x, hitArea.y-2 );
    }
    
    if( mTextMesh.getNumVertices() > 0 ) {
        arenderMesh.append(mTextMesh);
    }
    
//    arenderMesh.addVertices( mTextMesh.getVertices() );
//    arenderMesh.addTexCoords( mTextMesh.getTexCoords() );
}

//---------------------------------------------
void guiTypeColorPicker::_onColorParamChange( ofColor& av ) {
    for( int i = 0; i < mComponentParams.size(); i++ ) {
        mComponentParams[i] = av[i];
    }
}

//---------------------------------------------
void guiTypeColorPicker::_onColorComponentInputChange( string& s ) {
    if( mComponentParams.size() < 4 ) return;
    ofColor tcolor;
    tcolor.r = mComponentParams[0];
    tcolor.g = mComponentParams[1];
    tcolor.b = mComponentParams[2];
    tcolor.a = mComponentParams[3];
    mParamRef = tcolor;
}
