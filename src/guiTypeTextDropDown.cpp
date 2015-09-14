#include "guiTypeTextDropDown.h"

static int boxHeight = 15;

//------------------------------------------------
void guiTypeTextDropDown::setup(vector <string> boxNames){
	if( value.getNumValues() == 0 ){
        return;
    }

    guiBaseObject::setupNamesFromParams();
    vecDropList = boxNames;
    
    boxHeight = MAX(7, displayText.getTextHeight() + 3);
	setDimensions(getDefaultColumnWidth(), boxHeight);
	bShowDropDown = false;	
}

//-----------------------------------------------
void guiTypeTextDropDown::updateValue(){
	if( value.getNumValues() == 0 ){
        return;
    }

	//CB
	notify();	
}

//-----------------------------------------------.
void guiTypeTextDropDown::update(){
	if( value.getNumValues() == 0 ){
        return;
    }

	if ( value.getMax() != vecDropList.size()-1 ){
		// set new max
		value.setMax( vecDropList.size()-1 );
	}
	
	boxHeight = MAX(15, displayText.getTextHeight());
	hitArea.height = boundingBox.height = boxHeight;
	
	int minSize = 0;
	for(int i = 0; i < vecDropList.size(); i++){
		minSize = MAX(minSize, displayText.getTextWidth(vecDropList[i]));
	}
	
	boundingBox.width = MAX(boundingBox.width, minSize + boxHeight + 5);
	hitArea			= boundingBox;
	
	//setShowText(false);
	updateText();
	if(bShowDropDown) {
		hitArea.height = boundingBox.height = boxHeight * vecDropList.size();
	} else {
		hitArea.height = boundingBox.height = boxHeight;
		state = SG_STATE_NORMAL;
		setNormal();
	}
}

//-----------------------------------------------
void guiTypeTextDropDown::release(float x, float y, bool isRelative){
	if( value.getNumValues() == 0 ){
        return;
    }

	if(state != SG_STATE_SELECTED) {
		bShowDropDown = false;
	}
	state = SG_STATE_NORMAL;
 }

//-----------------------------------------------
void guiTypeTextDropDown::notify(){
	if( value.getNumValues() == 0 ){
        return;
    }
    
	guiCallbackData cbVal;
	cbVal.setup(xmlName, name);
	cbVal.addValueI(value.getValueI());
	if( value.getValueI() < vecDropList.size() ){
		cbVal.addValueS(vecDropList[value.getValueI()]);
	}
	ofNotifyEvent(guiEvent,cbVal,this);
}

//-----------------------------------------------.
void guiTypeTextDropDown::updateGui(float x, float y, bool firstHit, bool isRelative) {
	if( value.getNumValues() == 0 ){
        return;
    }
    
	if(!firstHit)return;

	if( state == SG_STATE_SELECTED){
		float relX = x - hitArea.x;
		float relY = y - hitArea.y;

		if(bShowDropDown) {
			for(unsigned int i = 0; i < vecDropList.size(); i++){
				ofRectangle tmpRect(0, i * (boxHeight), boundingBox.width, boxHeight);
				if( isInsideRect(relX, relY, tmpRect) ){
					value.setValue(i, 0);
					bShowDropDown = false;
					//CB
					notify();
					break;
				}
			}
		} else {
			ofRectangle tmpRect(0, 0, boundingBox.width, boxHeight);
			if( isInsideRect(relX, relY, tmpRect) ){
				bShowDropDown = true;
			}
		}
	}
}

void guiTypeTextDropDown::updateText(){
	if( value.getNumValues() == 0 ){
        return;
    }
    
	// don't append the selected number
	displayText.setText( name );
	updateBoundingBox();
}

//-----------------------------------------------.
void guiTypeTextDropDown::render(){
	if( value.getNumValues() == 0 ){
        return;
    }
    
	ofPushStyle();
	guiBaseObject::renderText();

		//draw the background
		ofFill();
		glColor4fv(bgColor.getNormalColorF());
		ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

		float arrowX = hitArea.x + boundingBox.width - boxHeight*0.5;
		float arrowY = hitArea.y;

		if(bShowDropDown)
		{

			for(int i = 0; i < (int) vecDropList.size(); i++)
			{
				float bx = hitArea.x;
				float by = hitArea.y + i * (boxHeight);

				if(value.getValueI() == i){
					glColor4fv(fgColor.getSelectedColorF());
				}else{
					glColor4fv(fgColor.getNormalColorF());
				}

				ofFill();

				ofRect(bx, by,  boundingBox.width, boxHeight);

				ofNoFill();
				glColor4fv(outlineColor.getColorF());
				ofRect(bx, by,  boundingBox.width, boxHeight);

				if(i==0) {				
					ofFill();
					glColor4fv(outlineColor.getColorF());
					ofRect(arrowX, arrowY, boxHeight*0.5, boxHeight*0.5);
				}

				glColor4fv(textColor.getColorF());

				displayText.renderString(vecDropList[i], bx + 2, by + boxHeight -4);

			}

		} else {
			float bx = hitArea.x;
			float by = hitArea.y;

			ofFill();
			glColor4fv(bgColor.getColorF());
			ofRect(bx, by,  boundingBox.width, boxHeight);

			ofNoFill();
			glColor4fv(outlineColor.getColorF());
			ofRect(bx, by,  boundingBox.width, boxHeight);

			ofFill();
			glColor4fv(outlineColor.getColorF());
			//ofTriangle(bx + boundingBox.width - 7, by + boxHeight, bx + boundingBox.width - 14, by,bx + boundingBox.width, by);
			ofRect(arrowX, arrowY, boxHeight*0.5, boxHeight*0.5);
		
			glColor4fv(textColor.getColorF());
            if(value.getValueI() < vecDropList.size()) {
                displayText.renderString(vecDropList[value.getValueI()], bx + 2, by + boxHeight -4);
            }

		}

	ofPopStyle();
}
