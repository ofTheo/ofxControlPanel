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
	void setup(string textInputName, string defaultVal);

	
	//--------------------------------------------
	void setFont(ofTrueTypeFont * fontPtr){
		displayText.setFont(fontPtr);
		valueText.setFont(fontPtr);
	}

	//--------------------------------------------
	void release();

	//-----------------------------------------------.
	void updateGui(float x, float y, bool firstHit, bool isRelative);

	
	//-----------------------------------------------.
	void render();
	
	// return true if we eat the key
	bool keyPressed( int k );


	/// deal with the actual value text being edited
	bool valueTextHasChanged() { return changed; }
	void clearValueTextChangedFlag() { changed = false; }
	string getValueText() { return valueText.textString; }
	void setValueText( string new_text );
	
private:
	
	bool changed;
	int text_position;
	guiTextBase valueText;
	

};


