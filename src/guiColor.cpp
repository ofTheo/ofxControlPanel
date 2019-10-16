#include "guiColor.h"

//------------------------------------------------
guiColor::guiColor(){
	bUseSelected  = false;
	bUseDisabled    = false;
	colorMode       = 0;
}

//------------------------------------------------
void guiColor::setColor(int r, int g, int b, int a){
//	color.setColor(r, g, b, a);
    color.set( (float)r/255.f, (float)g/255.f, (float)b/255.f, (float)a/255.f );
}

//------------------------------------------------
void guiColor::setColor(int hexValue){
    color.setHex( hexValue );
//	color.setColor(hexValue);
}

//------------------------------------------------
void guiColor::setSelectedColor(int r, int g, int b, int a){
//	selected.setColor(r, g, b, a);
    selected.set( (float)r/255.f, (float)g/255.f, (float)b/255.f, (float)a/255.f );
	bUseSelected = true;
}

//------------------------------------------------
void guiColor::setSelectedColor(int hexValue){
//	selected.setColor(hexValue);
    selected.setHex(hexValue);
	bUseSelected = true;
}

//------------------------------------------------
void guiColor::setDisabledColor(int r, int g, int b, int a){
//	disabled.setColor(r, g, b, a);
    disabled.set( (float)r/255.f, (float)g/255.f, (float)b/255.f, (float)a/255.f );
	bUseDisabled = true;
}

//------------------------------------------------
void guiColor::setDisabledColor(int hexValue){
//	disabled.setColor(hexValue);
    disabled.setHex( hexValue );
	bUseDisabled = true;
}

//------------------------------------------------
void guiColor::setGuiColorMode(int whichColor){
	colorMode = whichColor;
}

////----------------------------------------------------------
//simpleColor& guiColor::getColor(){
//	if( colorMode == 1 && bUseSelected) return selected;
//	else if( colorMode == 2 && bUseDisabled ) return disabled;
//	else return color;
//}

//----------------------------------------------------------
ofFloatColor guiColor::getColor(){
    if( colorMode == 1 && bUseSelected) return selected;//.getColor();
    else if( colorMode == 2 && bUseDisabled) return disabled;//.getColor();
    else return color;//.getColor();
}

//----------------------------------------------------------
ofFloatColor guiColor::getSelectedColor(){
    return selected;//.getColor();
}

//----------------------------------------------------------
ofFloatColor guiColor::getNormalColor(){
    return color;//.getColor();
}
