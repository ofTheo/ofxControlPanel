#include "guiType2DSlider.h"

//------------------------------------------------
guiType2DSlider::guiType2DSlider(){
	knobSize = 6.0;
}

//------------------------------------------------
void guiType2DSlider::setup(){
    if( value.getNumValues() != 2 ){
        return;
    }
    guiBaseObject::setupNamesFromParams();
	
	setDimensions(getDefaultColumnWidth(), getDefaultColumnWidth());
	updateText();
}

//---------------------------------------------
void guiType2DSlider::updateBoundingBox(){
	hitArea.y = boundingBox.y;
}

//-----------------------------------------------.
void guiType2DSlider::updateGui(float x, float y, bool firstHit, bool isRelative){
    if( value.getNumValues() != 2 ){
        return;
    }
    
	if( state == SG_STATE_SELECTED){
        
        if( !isRelative ){
            float pct1 = ( x - ( hitArea.x ) ) / hitArea.width;
            value.setValueAsPct( pct1, 0);
            float pct2 = ( y - ( hitArea.y ) ) / hitArea.height;
            value.setValueAsPct( pct2, 1);
        }else{
            float pct1 = value.getPct(0);
			pct1 += (x * 0.02) / hitArea.width;
			value.setValueAsPct( pct1, 0 );

            float pct2 = value.getPct(1);
			pct2 += (y * 0.02) / hitArea.height;
			value.setValueAsPct( pct2, 1 );
        }

		updateText();
		//CB
		notify();
	}
}

//-----------------------------------------------
void guiType2DSlider::notify(){
    if( value.getNumValues() != 2 ){
        return;
    }
    
	guiCallbackData cbVal;
	cbVal.setup(xmlName, name);
	cbVal.addValueF(value.getValueF(0));
	cbVal.addValueF(value.getValueF(1));
	ofNotifyEvent(guiEvent,cbVal,this);
}

//-----------------------------------------------
void guiType2DSlider::setKnobSize(float _knobSize){
	knobSize = _knobSize;
}

//-----------------------------------------------
void guiType2DSlider::setValue(float _value, int whichParam) {
	value.setValue(_value, whichParam);
}

//-----------------------------------------------.
void guiType2DSlider::updateValue(){
   //CB
   notify();
}

//-----------------------------------------------.
void guiType2DSlider::render(){
    if( value.getNumValues() != 2 ){
        return;
    }
    
	ofPushStyle();

			//draw the background
			ofFill();
			glColor4fv(bgColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			//draw the foreground

			float boxWidth  = knobSize;
			float boxHeight = knobSize;

			float bx = hitArea.x + (boxWidth * 0.5) + ( value.getPct(0) * (hitArea.width - (boxWidth)) );
			float by = hitArea.y + (boxHeight * 0.5) + ( value.getPct(1) * (hitArea.height - (boxHeight)) );
			
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, 0xCCCC);
			glColor4fv(outlineColor.getColorF());
			ofLine(bx, hitArea.y, bx, hitArea.y + hitArea.height);
			ofLine(hitArea.x, by, hitArea.x + hitArea.width, by);
			glDisable(GL_LINE_STIPPLE);

			glColor4fv(fgColor.getColorF());
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofCircle(bx, by, boxHeight/2);
			ofSetRectMode(OF_RECTMODE_CORNER);
			
			//draw the outline
			ofNoFill();
			glColor4fv(outlineColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			glColor4fv(textColor.getColorF());			
			displayText.renderString( name + "\n" + varsString, hitArea.x, hitArea.y + displayText.getTextSingleLineHeight() );

	ofPopStyle();
}

