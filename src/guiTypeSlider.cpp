#include "guiTypeSlider.h"

//------------------------------------------------
guiTypeSlider::guiTypeSlider() {}

//------------------------------------------------
guiTypeSlider::~guiTypeSlider() {
    if(mTextInput) {
//        ofRemoveListener(mTextInput->textChangeEvent, this, &guiTypeSlider::onTextInputChange );
//        ofRemoveListener(mTextInput->textEnteredEvent, this, &guiTypeSlider::onTextInputEntered );
        mTextInput.reset();
    }
}

//------------------------------------------------
void guiTypeSlider::setup(){
    bShowDefaultValue = false;
    defaultValue = 0.0;
    
    if( value.getNumValues() == 0 ){
        return;
    }
    guiBaseObject::setupNamesFromParams(); 
	
    if( value.getMax() > value.getMin() ){
        bShowDefaultValue = true;
        defaultValue = value.getPct();
    }
    
    if( !mTextInput ) {
        mTextInput = make_shared<guiTypeTextInput>();
        mTextInput->setup(value.paramGroup.get(0));
        children.push_back(mTextInput);
//        mTextInput->setup(value.paramGroup);
//        ofAddListener(mTextInput->textChangeEvent, this, &guiTypeSlider::onTextInputChange );
//        ofAddListener(mTextInput->textEnteredEvent, this, &guiTypeSlider::onTextInputEntered );
    }
	
	setDimensions(getDefaultColumnWidth(), 14);
}

//---------------------------------------------
void guiTypeSlider::updateValue(){
    guiBaseObject::updateValue();
//    bShowXmlValue = false;
//    xmlValue = value.getPct();
}


//---------------------------------------------
void guiTypeSlider::updateBoundingBox(){
	hitArea.y = boundingBox.y;
	if( bShowText ){
		hitArea.height = boundingBox.height = displayText.getTextHeight() + 3.0;
        
        float textW = displayText.getTextWidth(varsString);
        float rightAlignVarsX = hitArea.width - textW;
        float rectW = textW+4.0;
        if(mTextInput) {
            // local to this hit Area
            mTextInput->hitArea = ofRectangle(hitArea.getWidth()-rectW-4, 2, rectW, hitArea.height-3);
        }
	}
}

//-------------------------------------------
void guiTypeSlider::onEnabledChanged() {
    if(mTextInput) {
        mTextInput->setEnabled(!isEnabled());
    }
    onRelayout();
}

//-------------------------------------------
void guiTypeSlider::onRelayout() {
    guiBaseObject::onRelayout();
    if(mTextInput) {
        mTextInput->onRelayout();
    }
}

//should be called on mousedown
//-------------------------------------------
bool guiTypeSlider::checkHit(float x, float y, bool isRelative) {
    bool bHit = guiBaseObject::checkHit( x,y,isRelative);
    if( !bHit ) {
        if(mTextInput) {
            mTextInput->setFocus(false);
        }
    }
    return bHit;
}

//-----------------------------------------------.
void guiTypeSlider::updateGui(float x, float y, bool firstHit, bool isRelative){
    if( value.getNumValues() == 0 ){
        return;
    }
    
    bool bUpdateSlider = true;
    if( mTextInput ) {
        if( mTextInput->hasFocus() ) {
            bUpdateSlider = false;
        }
    }

	if( state == SG_STATE_SELECTED ){
        if(bUpdateSlider) {
            if( !isRelative ) {
                float offsetX = x - hitArea.x;
                float offsetY = y - hitArea.y;
                bool bHitText = false;
                if( mTextInput->hitArea.inside(offsetX, offsetY)) {
                    bHitText = true;
                }
//                cout << "guiTypeSlider :: hit text: " << bHitText << " | " << ofGetFrameNum() << endl;
                float pct = ( x - ( hitArea.x ) ) / hitArea.width;
                if( bHitText ) {
                    if( !firstHit ) value.setValueAsPct( pct );
                } else {
                    value.setValueAsPct( pct );
                }
            }else if( !firstHit ){
                float pct = value.getPct();
                pct += (x * 0.02) / hitArea.width;
                value.setValueAsPct( pct );
            }
            
            if( fabs(value.getPct() - xmlValue) > 0.01 ){
                bShowXmlValue = true;
            }else{
                bShowXmlValue = false;
            }
        }
        
//        cout << " slider: " << name << " update gui | " << ofGetFrameNum() << endl;
//		notify();
	}
}

//-----------------------------------------------.
void guiTypeSlider::render(){
    if( value.getNumValues() == 0 ){
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
        ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width * value.getPct(), hitArea.height);

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
void guiTypeSlider::addToRenderMesh( ofMesh& arenderMesh ) {
    addRectangleToMesh( arenderMesh, hitArea, bgColor.getColor() );
    
    addRectangleToMesh( arenderMesh, ofRectangle( hitArea.x, hitArea.y, hitArea.width*value.getPct(), hitArea.height ), (bShowXmlValue && state != SG_STATE_SELECTED)  ? xmlChangedColor.getColor() : fgColor.getColor() );
    if( bShowXmlValue ){
        
        float x = xmlValue * hitArea.getWidth();
        float flip = 1.0;
        if( xmlValue > 0.95 ){
            flip *= -1.0;
        }
        addTriangleToMesh( arenderMesh,
                          hitArea.x + x, hitArea.y,
                          hitArea.x + x + 2, hitArea.y,
                          hitArea.x + x + 2, hitArea.y + hitArea.height,
                          xmlChangedColor.getColor()
                          );
        addTriangleToMesh( arenderMesh,
                          hitArea.x + x + 2, hitArea.y + hitArea.height,
                          hitArea.x + x, hitArea.y + hitArea.height,
                          hitArea.x + x, hitArea.y,
                          xmlChangedColor.getColor()
                          );
        
    }
    if( bShowDefaultValue ){
        float x = defaultValue * hitArea.getWidth();
        float flip = 1.0;
        if( defaultValue > 0.95 ){
            flip *= -1.0;
        }
        addTriangleToMesh( arenderMesh,
                          hitArea.x + x, hitArea.y,
                          hitArea.x + x, hitArea.y + hitArea.height * 0.5,
                          hitArea.x + x + hitArea.height * 0.5 *flip, hitArea.y,
//                          ofColor(1.0 * 255.0, 0.3 * 255.0, 0.2 * 255.0, 255)
                          triDefaultColor.getColor()
                          );
    }
        
    if(mTextInput) {
        mTextInput->addToRenderMesh( arenderMesh, hitArea.x, hitArea.y );
    }
//    float textW = displayText.getTextWidth(varsString);
//    float rightAlignVarsX = hitArea.width - textW;
//    float rectW = textW+4.0;
//    addRectangleToMesh(arenderMesh, ofRectangle(hitArea.getRight()-rectW, hitArea.y+2, rectW, hitArea.height-3), ofFloatColor(0.0,0.0,0.0, 0.7));
}

//---------------------------------------------
void guiTypeSlider::addToLinesRenderMesh( ofMesh& arenderMesh ) {
    addRectangleToLinesMesh( arenderMesh, hitArea, outlineColor.getColor() );
//    if(mTextInput) {
//        mTextInput->addToLinesRenderMesh( arenderMesh, hitArea.x, hitArea.y );
//    }
}

//---------------------------------------------
void guiTypeSlider::addToTextRenderMesh( ofMesh& arenderMesh ) {
//    addStringToMesh(ofMesh &amesh, ofBitmapFont& aBitmapFont, string aString, float ax, float ay )
//    name, hitArea.x , hitArea.y + displayText.getTextSingleLineHeight()
    if( mTextMesh.getNumVertices() == 0 ) {
        mTextMesh.clear();
        displayText.addStringToMesh(mTextMesh, name, hitArea.x+2, hitArea.y, textColor.getColor() );
    //    addStringToMesh( arenderMesh, afont, name, hitArea.x, hitArea.y + displayText.getTextSingleLineHeight(), textColor.getColor() );
    }
    
    if( mTextInput ) {
        mTextInput->addToTextRenderMesh( arenderMesh, hitArea.x, hitArea.y-2 );
    }
    
    if( mTextMesh.getNumVertices() > 0 ) {
        arenderMesh.append(mTextMesh);
    }
    
//    arenderMesh.addVertices( mTextMesh.getVertices() );
//    arenderMesh.addTexCoords( mTextMesh.getTexCoords() );
}
#endif











