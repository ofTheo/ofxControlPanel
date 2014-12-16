#include "guiTypeSlider.h"

//------------------------------------------------
void guiTypeSlider::setup(){
    if( value.getNumValues() == 0 ){
        return;
    }
    guiBaseObject::setupNamesFromParams(); 
	
	setDimensions(getDefaultColumnWidth(), 14);
}

//-----------------------------------------------
void guiTypeSlider::updateValue(){
    if( value.getNumValues() == 0 ){
        return;
    }

	notify();
}

//---------------------------------------------
void guiTypeSlider::updateBoundingBox(){
	hitArea.y = boundingBox.y;
	if( bShowText ){
		hitArea.height = boundingBox.height = displayText.getTextHeight() + 3.0;
	}
}

//-----------------------------------------------.
void guiTypeSlider::updateGui(float x, float y, bool firstHit, bool isRelative){
    if( value.getNumValues() == 0 ){
        return;
    }

	if( state == SG_STATE_SELECTED ){
		if( !isRelative ){
			float pct = ( x - ( hitArea.x ) ) / hitArea.width;
			value.setValueAsPct( pct );
		}else if( !firstHit ){
			float pct = value.getPct();
			pct += (x * 0.02) / hitArea.width;
			value.setValueAsPct( pct );
		}
		notify();
	}
}

//-----------------------------------------------.
void guiTypeSlider::render(){
    if( value.getNumValues() == 0 ){
        return;
    }

	ofPushStyle();

			//draw the background
			ofFill();
			glColor4fv(bgColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			//draw the foreground
			glColor4fv(fgColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width * value.getPct(), hitArea.height);
    
            float * color = textColor.getColorF();
			glColor4f(color[0]*0.75, color[1]*0.75, color[2]*0.75, color[3]);
			displayText.renderString(name, hitArea.x , hitArea.y + displayText.getTextSingleLineHeight() );
			
            float textW = displayText.getTextWidth(varsString);
			float rightAlignVarsX = hitArea.width - textW;
            float rectW = textW+4.0;
    
            if( state == SG_STATE_SELECTED ){
                glColor4f(0.0, 0.0, 0.0, 0.7);
            }else{
                glColor4f(0.0, 0.0, 0.0, 0.4);
            }
			ofRect(hitArea.getRight()-rectW, hitArea.y+2, rectW, hitArea.height-3);

			glColor4fv(color);
			displayText.renderString(varsString, hitArea.x + rightAlignVarsX - 2.0, hitArea.y + displayText.getTextSingleLineHeight() );

			//draw the outline
			ofNoFill();
			glColor4fv(outlineColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
	ofPopStyle();
}
