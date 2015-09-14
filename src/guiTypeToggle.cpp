#include "guiTypeToggle.h"
#include "guiColor.h"
#include "simpleColor.h"
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
    if( value.getNumValues() == 0 ){
        return;
    }
    
	//CB
	notify();
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
		notify();
	}
}

//-----------------------------------------------
void guiTypeToggle::notify(){
    if( value.getNumValues() == 0 ){
        return;
    }
    
	guiCallbackData cbVal;
	cbVal.setup(xmlName, name);
	cbVal.addValueF(value.getValueI());
	ofNotifyEvent(guiEvent,cbVal,this);
	//CB
}

//---------------------------------------------
void guiTypeToggle::updateBoundingBox(){
	hitArea.y = boundingBox.y;
}

//-----------------------------------------------.
void guiTypeToggle::render(){
    if( value.getNumValues() == 0 ){
        return;
    }
    
	ofPushStyle();

			//draw the background
			ofFill();
			glColor4fv(bgColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			if(value.getValueI()){
				ofFill();
				glColor4fv(fgColor.getColorF());
				ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
			}
			
			//draw the outline
			ofNoFill();
			glColor4fv(outlineColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
			
			if(bShowText){
				glColor4fv(textColor.getColorF());
				displayText.renderText(hitArea.x + hitArea.width + 2, hitArea.y + displayText.getTextSingleLineHeight() - 2);
			}

	ofPopStyle();
}

//-----------------------------------------------
void guiTypeToggle::updateText(){
	displayText.setText( name ); 
	labelWidth = displayText.getTextWidth();
	updateBoundingBox(); 
}
