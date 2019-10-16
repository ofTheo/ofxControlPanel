#include "guiTypeRangeSlider.h"

//------------------------------------------------
void guiTypeRangeSlider::setup(){
    bShowDefaultValue = false;
    defaultValues[0] = 0.0;
    defaultValues[1] = 1.0;
    
    if( value.getNumValues() != 2 ){
        return;
    }
    
    if( name == "" ) {
        guiBaseObject::setupNamesFromParams();
    }
	
    if( value.getMax() > value.getMin() ){
        bShowDefaultValue = true;
    }
    defaultValues[0] = value.getPct(0);
    defaultValues[1] = value.getPct(1);
	
	setDimensions(getDefaultColumnWidth(), 14+16);
}

//-----------------------------------------------.
void guiTypeRangeSlider::updateGui(float x, float y, bool firstHit, bool isRelative){
    if( value.getNumValues() != 2 ) {
        return;
    }
    
    if( state == SG_STATE_SELECTED ){
        if( !isRelative ){
            float pct = ( x - ( hitArea.x ) ) / hitArea.width;
            float minX = hitArea.x + hitArea.width * value.getPct(0);
            float maxX = hitArea.x + hitArea.width * value.getPct(1);
            
            // determine if one should be selected //
            float diffMin = abs(minX-x);
            float diffMax = abs(maxX-x);
            
            mSelectedIndex = 0;
            if( diffMax < diffMin ) {
                mSelectedIndex = 1;
            }
            
            if( mSelectedIndex > -1 ) {
                value.setValueAsPct( pct, mSelectedIndex );
            }
            
//            value.setValueAsPct( pct );
        }else if( !firstHit ){
            
//            cout << "mSelectedIndex: " << mSelectedIndex << " | " << ofGetFrameNum() << endl;
            
            if( mSelectedIndex > -1 ) {
                float pct = value.getPct(mSelectedIndex);
                pct += (x * 0.02) / hitArea.width;
                
                value.setValueAsPct( pct, mSelectedIndex );
            }
            
//             cout << " guiTypeRangeSlider: !firstHit" << name << " update gui | " << ofGetFrameNum() << endl;
//            float pct = value.getPct();
//            pct += (x * 0.02) / hitArea.width;
//            value.setValueAsPct( pct );
        } else if( firstHit ) {
            float minX = hitArea.x + hitArea.width * value.getPct(0);
            float maxX = hitArea.x + hitArea.width * value.getPct(1);
            
            // determine if one should be selected //
            float diffMin = abs(minX-x);
            float diffMax = abs(maxX-x);
            
            mSelectedIndex = 0;
            if( diffMax < diffMin ) {
                mSelectedIndex = 1;
            }
        }
        
#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
        mTextMesh.clear();
#endif
        
        //        cout << " slider: " << name << " update gui | " << ofGetFrameNum() << endl;
//        notify();
    } else if( state == SG_STATE_NORMAL ) {
        mSelectedIndex = -1;
    }
    
    if( value.getPct(0) > value.getPct(1) ) {
        value.setValueAsPct(value.getPct(1)-0.05,0);
    }
}

//-----------------------------------------------
void guiTypeRangeSlider::updateBoundingBox() {
    hitArea.y = boundingBox.y+12;
    if( bShowText ){
//        hitArea.height = boundingBox.height = displayText.getTextHeight() + 3.0;
        hitArea.height = displayText.getTextHeight() + 3.0;
    }
}

//-----------------------------------------------
void guiTypeRangeSlider::setValue(float _value, int whichParam) {
    value.setValue(_value, whichParam);
}


//-----------------------------------------------.
void guiTypeRangeSlider::render(){
    if( value.getNumValues() != 2 ) {
        return;
    }

    #ifdef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    ofPushStyle(); {
        
        // draw the background
        ofFill();
        ofSetColor(bgColor.getColor());
        ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

        // draw the foreground
        ofSetColor(fgColor.getColor());
//        ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width * value.getPct(), hitArea.height);
        float minX = hitArea.x+hitArea.width*value.getPct(0);
        float maxX = hitArea.x+hitArea.width*value.getPct(1);
        ofDrawRectangle(minX, hitArea.y, maxX-minX, hitArea.height);

        ofColor color = textColor.getColor();
        ofSetColor(((float)color.r) * 0.75, ((float)color.g) * 0.75, ((float)color.b) * 0.75, color.a);
        displayText.renderString(name, hitArea.x , hitArea.y + displayText.getTextSingleLineHeight() );
        
        float textW = displayText.getTextWidth(varsString);
        float rightAlignVarsX = hitArea.width - textW;
        float rectW = textW+4.0;

        if( state == SG_STATE_SELECTED ){
            ofSetColor(0.0, 0.0, 0.0, 0.7 * 255.0);
        }else{
            ofSetColor(0.0, 0.0, 0.0,  0.7 * 255.0);
        }
        ofDrawRectangle(hitArea.getRight()-rectW, hitArea.y+2, rectW, hitArea.height-3);

        if( bShowDefaultValue ){
            float x = defaultValue * hitArea.getWidth();
            ofSetColor(1.0 * 255.0, 0.3 * 255.0, 0.2 * 255.0, 255);
            float flip = 1.0;
            if( defaultValue > 0.95 ){
                flip *= -1.0;
            }
            ofDrawTriangle(hitArea.x + x, hitArea.y, hitArea.x + x, hitArea.y + hitArea.height * 0.5, hitArea.x + x + hitArea.height * 0.5 *flip , hitArea.y);
        }

        ofSetColor(color);
        displayText.renderString(varsString, hitArea.x + rightAlignVarsX - 2.0, hitArea.y + displayText.getTextSingleLineHeight() );

        // draw the outline
        ofNoFill();
        ofSetColor(outlineColor.getColor());
        ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
    } ofPopStyle();
    #endif
}

#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
//---------------------------------------------
void guiTypeRangeSlider::addToRenderMesh( ofMesh& arenderMesh ) {
    addRectangleToMesh( arenderMesh, hitArea, bgColor.getColor() );
    float minX = hitArea.x+hitArea.width*value.getPct(0);
    float maxX = hitArea.x+hitArea.width*value.getPct(1);
    addRectangleToMesh( arenderMesh, ofRectangle( minX, hitArea.y, maxX-minX, hitArea.height ), fgColor.getColor() );
    if( bShowDefaultValue ){
        float x = defaultValues[0] * hitArea.getWidth();
        float flip = 1.0;
        if( defaultValues[0] > 0.95 ) {
            flip *= -1.0;
        }
        addTriangleToMesh( arenderMesh,
                          hitArea.x + x, hitArea.y,
                          hitArea.x + x, hitArea.y + hitArea.height * 0.5,
                          hitArea.x + x + hitArea.height * 0.5 *flip, hitArea.y,
                          triDefaultColor.getColor()
                          );
        
        x = defaultValues[1] * hitArea.getWidth();
        flip = 1.0;
        if( defaultValues[1] > 0.95 ) {
            flip *= -1.0;
        }
        addTriangleToMesh( arenderMesh,
                          hitArea.x + x, hitArea.y,
                          hitArea.x + x, hitArea.y + hitArea.height * 0.5,
                          hitArea.x + x + hitArea.height * 0.5 *flip, hitArea.y,
                          triDefaultColor.getColor()
                          );
    }
    
    // min value //
    float textW = displayText.getTextWidth( getVarAsString(0));
//    float rightAlignVarsX = hitArea.width - textW;
    float rectW = textW+4.0;
    addRectangleToMesh(arenderMesh, ofRectangle(hitArea.x+4, hitArea.y+2, rectW, hitArea.height-3), ofFloatColor(0.0,0.0,0.0, 0.7));
    
    // max value //
    textW = displayText.getTextWidth( getVarAsString(1));
    float rightAlignVarsX = hitArea.width - textW;
    rectW = textW+4.0;
    addRectangleToMesh(arenderMesh, ofRectangle(hitArea.getRight()-rectW, hitArea.y+2, rectW, hitArea.height-3), ofFloatColor(0.0,0.0,0.0, 0.7));
}

//---------------------------------------------
void guiTypeRangeSlider::addToLinesRenderMesh( ofMesh& arenderMesh ) {
    addRectangleToLinesMesh( arenderMesh, hitArea, outlineColor.getColor() );
}

//---------------------------------------------
void guiTypeRangeSlider::addToTextRenderMesh( ofMesh& arenderMesh ) {
//    addStringToMesh(ofMesh &amesh, ofBitmapFont& aBitmapFont, string aString, float ax, float ay )
//    name, hitArea.x , hitArea.y + displayText.getTextSingleLineHeight()
    if( mTextMesh.getNumVertices() == 0 ) {
        mTextMesh.clear();
        
        displayText.addStringToMesh(mTextMesh, name, boundingBox.x, boundingBox.y-6, textColor.getColor() );
        
        float textW = displayText.getTextWidth( getVarAsString(0));
        displayText.addStringToMesh( mTextMesh, getVarAsString(0), hitArea.x + 4, hitArea.y, textColor.getColor() );
//
        textW = displayText.getTextWidth( getVarAsString(1));
        float rightAlignVarsX = hitArea.width - textW;
        displayText.addStringToMesh( mTextMesh, getVarAsString(1), hitArea.x + rightAlignVarsX - 2.0, hitArea.y, textColor.getColor() );
    }
    
    if( mTextMesh.getNumVertices() > 0 ) {
        arenderMesh.append(mTextMesh);
    }
    
//    arenderMesh.addVertices( mTextMesh.getVertices() );
//    arenderMesh.addTexCoords( mTextMesh.getTexCoords() );
}
#endif








