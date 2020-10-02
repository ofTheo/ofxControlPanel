#include "guiTypeToggle.h"
//#include "guiColor.h"
//#include "simpleColor.h"
#include "guiValue.h"

//------------------------------------------------
void guiTypeToggle::setup(){
    if( value.getNumValues() == 0 ){
        return;
    }
    guiBaseObject::setupNamesFromParams(); 

    setDimensions(14, 14);
}

//-----------------------------------------------
void guiTypeToggle::updateValue(){
    guiBaseObject::updateValue(); 
    if( value.getNumValues() == 0 ){
        return;
    }
    
	//CB
//	notify();
    #ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    mTextMesh.clear();
    #endif
}

//-----------------------------------------------.
void guiTypeToggle::updateGui(float x, float y, bool firstHit, bool isRelative){
    if( value.getNumValues() == 0 ){
        return;
    }
    
	if(!firstHit)return;

	if( state == SG_STATE_SELECTED ){
		if( value.getValueI() == 0 ){
			value.setValue(1);
		}else{
			value.setValue(0);
		}
        
		//CB
//		notify();
        #ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
        mTextMesh.clear();
        #endif
	}
}

////-----------------------------------------------
//void guiTypeToggle::notify(){
//    if( value.getNumValues() == 0 ){
//        return;
//    }
////    cout << "notify : Need to update, value changed | " << ofGetFrameNum() << endl;
//    #ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
//    mTextMesh.clear();
//    #endif
//
//	guiCallbackData cbVal;
//	cbVal.setup(xmlName, name);
//	cbVal.addValueF(value.getValueI());
//	ofNotifyEvent(guiEvent,cbVal,this);
//	//CB
//}

//---------------------------------------------
void guiTypeToggle::updateBoundingBox(){
	hitArea.y = boundingBox.y;
}

//-----------------------------------------------.
void guiTypeToggle::render(){
    if( value.getNumValues() == 0 ){
        return;
    }
    
    #ifdef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    ofPushStyle(); {

        //draw the background
        ofFill();
        ofSetColor(bgColor.getColor());
        ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

        if(value.getValueI()){
            ofFill();
            ofSetColor(fgColor.getColor());
            ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
        }
        
        //draw the outline
        ofNoFill();
        ofSetColor(outlineColor.getColor());
        ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
        
        if(bShowText){
            ofSetColor(textColor.getColor());
            displayText.renderText(hitArea.x + hitArea.width + 2, hitArea.y + displayText.getTextSingleLineHeight() - 2);
        }

    } ofPopStyle();
    #endif
}

//-----------------------------------------------
void guiTypeToggle::updateText(){
//    cout << "guiTypeToggle :: updateText : | " << ofGetFrameNum() << endl;
	displayText.setText( name );
	labelWidth = displayText.getTextWidth();
	updateBoundingBox(); 
}

#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
//-----------------------------------------------
void guiTypeToggle::addToRenderMesh( ofMesh& arenderMesh ) {
    addRectangleToMesh( arenderMesh, hitArea,  bgColor.getColor() );
    if( value.getValueI() ) {
        addRectangleToMesh( arenderMesh, hitArea, bShowXmlValue  ? xmlChangedColor.getColor() : fgColor.getColor() );
    }
}

//-----------------------------------------------
void guiTypeToggle::addToLinesRenderMesh( ofMesh& arenderMesh ) {
    addRectangleToLinesMesh( arenderMesh, hitArea, bShowXmlValue  ? xmlChangedColor.getColor() : outlineColor.getColor() );
}

//-----------------------------------------------
void guiTypeToggle::addToTextRenderMesh( ofMesh& arenderMesh ) {
    if( bShowText ) {
        if( mTextMesh.getNumVertices() == 0 ) {
            displayText.addStringToMesh( mTextMesh, displayText.textString, hitArea.x + hitArea.width + 4, hitArea.y, textColor.getColor() );
        }
//        arenderMesh.addVertices( mTextMesh.getVertices() );
//        arenderMesh.addTexCoords( mTextMesh.getTexCoords() );
        if(mTextMesh.getNumVertices() > 0 ) {
            arenderMesh.append( mTextMesh );
        }
    }
}
#endif









