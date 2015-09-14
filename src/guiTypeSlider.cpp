#include "guiTypeSlider.h"

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
			ofSetColor(bgColor.getColor());
			ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			//draw the foreground
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

			//draw the outline
			ofNoFill();
			ofSetColor(outlineColor.getColor());
			ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
	ofPopStyle();
}
