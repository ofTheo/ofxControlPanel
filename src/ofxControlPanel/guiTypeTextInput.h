#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTypeTextInput : public guiBaseObject{

    public:

        guiTypeTextInput(){
		}

        //------------------------------------------------
        void setup(string textInputName, string defaultVal, int maxX, int maxY){
            value.addValueS(defaultVal);
            name = textInputName;
            updateText();
        }

		//--------------------------------------------
		void setFont(ofTrueTypeFont * fontPtr){
			displayText.setFont(fontPtr);
			valueText.setFont(fontPtr);
		}
		//-----------------------------------------------.
        void updateGui(float x, float y, bool firstHit, bool isRelative){
            
			if(!firstHit)return;
			
            if( state == SG_STATE_SELECTED){
                if( value.getValueI() == 0 ){
                    value.setValue(1, 0);
                }else{
                    value.setValue(0, 0);
                }
            }

			
			//if( state == SG_STATE_SELECTED){
                //float pct1 = ( x - ( hitArea.x ) ) / hitArea.width;
                //value.setValueAsPct( pct1, 0);
                //float pct2 = ( y - ( hitArea.y ) ) / hitArea.height;
                //value.setValueAsPct( pct2, 1);
			//	updateText();
            //}
        }

        //-----------------------------------------------.
        void render(){
            ofPushStyle();

                glPushMatrix();
                    
					guiBaseObject::renderText();

                    //draw the background
                    ofFill();
                    glColor4fv(bgColor.getColorF());
                    ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

					glColor4fv(textColor.getColorF());
					valueText.setText( value.getValueS());
					valueText.renderText(boundingBox.x + 2, boundingBox.y + (valueText.getTextSingleLineHeight()*2) + 3);
			
                    //draw the outline
					if( value.getValueI() ) glLineWidth(2.0);
					
					ofNoFill();
                    glColor4fv(outlineColor.getColorF());
                    ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
					
					glLineWidth(1.0);
			
                glPopMatrix();

            ofPopStyle();
        }
		
		guiTextBase valueText;

};


