/*
 *  guiTypeTextInput.cpp
 *  artvertiser
 *
 *  Created by damian on 25/10/10.
 *  Copyright 2010 frey damian@frey.co.nz. All rights reserved.
 *
 */


#include "guiTypeTextInput.h"


//------------------------------------------------
void guiTypeTextInput::setup(string textInputName, string defaultVal)
{
	valueText.setText( defaultVal );
	value.addValueB(false);
	name = textInputName;
	text_position = 0;
	changed = false;
	updateText();
}

bool guiTypeTextInput::keyPressed( int k )
{
	if ( isLocked() )
		return false;
	
	bool eaten = false;
	text_position = min(max(0, text_position), (int)valueText.textString.length() );
	if ( value.getValueB() )
	{
		eaten = true;
		// handle backspace
		if ( k == OF_KEY_BACKSPACE && valueText.textString.length() > 0 && text_position > 0 )
		{
			valueText.textString.erase( valueText.textString.begin()+(text_position-1) ) ;
			text_position--;
			changed = true;
		}
		// handle DEL
		else if ( k == OF_KEY_DEL && valueText.textString.length() > 0 && text_position < valueText.textString.length() )
		{
			valueText.textString.erase( valueText.textString.begin()+(text_position) );
		}
		// handle arrow keys, home, end
		else if ( k == OF_KEY_LEFT )
			text_position--;
		else if ( k == OF_KEY_RIGHT )
			text_position++;
		else if ( k == OF_KEY_HOME )
			text_position = 0;
		else if ( k == OF_KEY_END )
			text_position = valueText.textString.length();
		// everything else is text input
		else if ( k >= 32 /* ' ' */ && k <= 126 /* '~' */ )
		{
			char buf[3];
			sprintf( buf, "%c", (unsigned char)k );
			valueText.textString.insert( valueText.textString.begin()+text_position, k );
			text_position++;
			changed = true;
		}
	}
	return eaten;
}


void guiTypeTextInput::render()
{
	ofPushStyle();
	
	glPushMatrix();
	
	guiBaseObject::renderText();
	
	//draw the background
	ofFill();
	glColor4fv(bgColor.getColorF());
	ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
	
	glColor4fv(textColor.getColorF());
	valueText.renderText(boundingBox.x + 2, boundingBox.y + (valueText.getTextSingleLineHeight()*2) + 3);
	// draw blinking cursor
	if( !isLocked() && value.getValueB() && (ofGetElapsedTimeMillis()%500)>250)
	{
		text_position = min(max(0, text_position), (int)valueText.textString.length() );
		ofRect( hitArea.x+valueText.getTextWidth(valueText.textString.substr(0,text_position))+1, hitArea.y+1, 2, hitArea.height-2 );
	}
	
	ofNoFill();
	if( value.getValueB() ) 
		glColor4fv(outlineColor.getSelectedColorF());
	else
		glColor4fv(outlineColor.getNormalColorF());
	ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
	
	glLineWidth(1.0);
	
	glPopMatrix();
	
	ofPopStyle();
}


void guiTypeTextInput::release()
{
	// if we're releasing somewhere else
	if ( state != SG_STATE_SELECTED )
	{
		// hide
		value.setValue(false);
		changed = true;
	}
	state = SG_STATE_NORMAL;
}


void guiTypeTextInput::updateGui(float x, float y, bool firstHit, bool isRelative)
{
	
	if(!firstHit)
		return;
	
	if ( isLocked() )
		return;
	
	if( state == SG_STATE_SELECTED )
	{
		// activate
		value.setValue(true);
		// position cursor
		float rel_x = x - hitArea.x;
		float rel_y = y - hitArea.y;
		text_position = valueText.textString.length();
		for ( int i=0; i<valueText.textString.length(); i++ )
		{
			if ( rel_x-10 < valueText.getTextWidth(valueText.textString.substr(0,i)) )
			{
				text_position = i;
				break;
			}
		}
	}
	else
	{
		value.setValue(false);
	}
	
}

void guiTypeTextInput::setValueText( string new_text )
{
	value.setValue( false );
	valueText.setText( new_text );
	changed = false;
}

