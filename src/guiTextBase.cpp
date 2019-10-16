#include "guiTextBase.h"
#include "ofxControlPanel.h"

//-------------------------------------------
guiTextBase::guiTextBase(){
	textString = "";
	bRealFont  = false;
	fontSize   = 13;
	numNewLines = 0;
	ourFont = NULL;
}

//we should also have an option to load a font once
//then just pass a reference to the ofTTF font object
//--------------------------------------------
 void guiTextBase::setFont(ofTrueTypeFont * fontPtr){
	if( fontPtr != NULL && fontPtr->isLoaded() ){
		ourFont = fontPtr;
		bRealFont = true;
	}else bRealFont = false;
}


//--------------------------------------------
 float guiTextBase::getTextWidth(string textIn){
	float textWidth = 0;

	if(bRealFont)textWidth = ourFont->stringWidth(textIn);
	else textWidth = 0.615 * (float)fontSize * textIn.size() ;

	return  textWidth;
}

//--------------------------------------------
 float guiTextBase::getTextWidth(){
	return  getTextWidth(textString);
}


//--------------------------------------------
 float guiTextBase::getTextHeight(){
	float textHeight = 0;

	if(bRealFont)
		textHeight = ourFont->stringHeight(textString);
	else
		textHeight = fontSize + fontSize * numNewLines;

	return textHeight;
}

//--------------------------------------------
 float guiTextBase::getTextSingleLineHeight(){
	float textHeight = 0;

	if(bRealFont)
		textHeight = ourFont->stringHeight("A");
	else
		textHeight = fontSize;

	return textHeight;
}

//-------------------------------------------
 void guiTextBase::setText(string textStr){
	textString = textStr;

	numNewLines = 0;
	for(unsigned int i = 0; i < textString.size(); i++){
		if( textString[i] == '\n' )numNewLines++;
	}

}

//--------------------------------------------
 void guiTextBase::renderText(float x, float y){

	if(bRealFont){
		ourFont->drawString(textString, x, y);
	}
	else {
		ofDrawBitmapString(textString, x, y);
	}
}

//--------------------------------------------
 void guiTextBase::renderString(string textToRender, float x, float y){

	if(bRealFont){
		ourFont->drawString(textToRender, x, y);
	}
	else ofDrawBitmapString(textToRender, x, y);
}

//--------------------------------------------
 float guiTextBase::stringHeight(string textToRender){
	float textHeight = 0;

	if(bRealFont)textHeight = ourFont->stringHeight(textToRender);
	else{
		int numLines = 0;
		for(unsigned int i = 0; i < textToRender.size(); i++){
			if( textToRender[i] == '\n' )numLines++;
		}
		textHeight = fontSize + fontSize * numLines;
	}
	return textHeight;
}

//--------------------------------------------
 float guiTextBase::stringWidth(string textToRender){

	float textWidth = 0;

	if(bRealFont)textWidth = ourFont->stringWidth(textToRender);
	else textWidth = 0.615 * (float)fontSize * textToRender.size() ;

	return  textWidth;
}

#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
//---------------------------------------------
void guiTextBase::addStringToMesh(ofMesh &amesh, string aString, float ax, float ay, ofFloatColor acolor ) {
    //    mutThis->setAlphaBitmapText(true);
    
    ofMesh charMesh;// = aBitmapFont.getMesh(aString, ax, ay, OF_BITMAPMODE_SCREEN, true );
    if( bRealFont ) {
        charMesh = ourFont->getStringMesh( aString, ax, ay+getTextSingleLineHeight(), true );
    } else {
        charMesh = ofxControlPanel::bitmapFont.getMesh(aString, ax, ay+getTextSingleLineHeight(), OF_BITMAPMODE_SCREEN, true );
    }
    
    charMesh.clearColors();
    vector<ofFloatColor> tempCs;
    tempCs.assign( charMesh.getNumVertices(), acolor );
    charMesh.addColors(tempCs);
    charMesh.enableColors();
    amesh.append( charMesh );
    
//    amesh.addVertices( charMesh.getVertices() );
//    amesh.addTexCoords( charMesh.getTexCoords() );
//    vector<ofFloatColor> tempCs;
//    tempCs.assign( charMesh.getNumVertices(), acolor );
//    amesh.addColors( tempCs );
    //    mutThis->bind( aBitmapFont.getTexture(),0);
    //    draw(charMesh,OF_MESH_FILL,false,true,false);
    //    mutThis->unbind(aBitmapFont.getTexture(),0);
    //    mutThis->setAlphaBitmapText(false);
}
#endif

