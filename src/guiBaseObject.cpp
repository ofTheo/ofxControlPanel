#include "guiBaseObject.h"



guiColor gTextColor;
guiColor gFgColor;
guiColor gBgColor;
guiColor gOutlineColor;
guiColor gTriDefaultColor;

bool initialColorsLoaded = false;

guiBaseObject::guiBaseObject(){
	readOnly			= false;
	bShowText			= true;
	locked				= false;
	bRemoveFromLayout	= false;
	state				= SG_STATE_NORMAL;

	numDecimalPlaces    = 2;
	storedTextWidth     = 0;

	if( !initialColorsLoaded ){
		gFgColor.setColor(120, 120, 120, 255);
//		gFgColor.setSelectedColor(100, 140, 220, 255);
//        gFgColor.setColor(4,89,163, 255);
        gFgColor.setSelectedColor(253,2,85, 255);
        gFgColor.setDisabledColor(70, 70, 70, 255);

		gOutlineColor.setColor(60, 60, 60, 255);
//		gOutlineColor.setSelectedColor(100, 140, 220, 255);
        gOutlineColor.setSelectedColor(4,89,163, 255);
        gOutlineColor.setDisabledColor(40, 40, 40, 255);

		gBgColor.setColor(30, 30, 30, 255);
		gBgColor.setSelectedColor(30, 30, 30, 255);
        gBgColor.setDisabledColor(20, 20, 20, 255);

		gTextColor.setColor(255, 255, 255, 255);
		gTextColor.setSelectedColor(255, 255, 255, 255);
        gTextColor.setDisabledColor( 180, 180, 180, 255 );
        
        gTriDefaultColor.setColor(253,2,85,255);
        gTriDefaultColor.setSelectedColor(4,89,163, 255);
        gTriDefaultColor.setDisabledColor(20, 20, 20, 255);
        
		initialColorsLoaded = true;
	}
	
	fgColor			= gFgColor;
	outlineColor	= gOutlineColor;				
	bgColor			= gBgColor;
	textColor		= gTextColor;
    triDefaultColor = gTriDefaultColor;
	
	//these need to be setable at some point
	fontSize     = 11;
	titleSpacing = 5;
}

//--------------------------------------------
void guiBaseObject::setFont(ofTrueTypeFont * fontPtr){
	displayText.setFont(fontPtr);
}

//should be called on mousedown
//-------------------------------------------
bool guiBaseObject::checkHit(float x, float y, bool isRelative){
	if(readOnly)return false;
	if( isInsideRect(x, y, hitArea) && isEnabled() ){
		state = SG_STATE_SELECTED;
		setSelected();
		updateGui(x, y, true, isRelative);

		float offsetX = x - hitArea.x;
		float offsetY = y - hitArea.y;

		for(unsigned int i = 0; i < children.size(); i++){
			children[i]->checkHit(offsetX, offsetY, isRelative);
		}
		return true;
	}else{
        state = SG_STATE_NORMAL; 
    }
	return false;
}

//this is the equivilant of mouse moved if the gui element has been selected
//this is empty as it really should be specified by
//the extending class (gui element).
//------------------------------------------------
void guiBaseObject::updateGui(float x, float y, bool firstHit, bool isRelative){

}

//------------------------------------------------
void guiBaseObject::mouseMoved(float x, float y) {
    
}

//-------------------------------------------
void guiBaseObject::setupNamesFromParams(){
    if( value.getNumValues() > 0 ){
        name    = value.paramGroup[0].getName();
        xmlName = value.paramGroup[0].getEscapedName();
        
        if( name == "" || xmlName == "" ){
            cout << "guiBaseObject::setupNames - name has not been set!!!" << endl;
        }
        
    }else{
        cout << "guiBaseObject::setupNames - error setting name and xmlName no params have been added!" << endl;
    }
}

//should  be called on mouse up
//-------------------------------------------
void guiBaseObject::release(float x, float y, bool isRelative){
    if( state == SG_STATE_SELECTED ){
    
        state = SG_STATE_NORMAL;
        setNormal();
        
        float offsetX = x - hitArea.x;
        float offsetY = y - hitArea.y;
        
        for(unsigned int i = 0; i < children.size(); i++){
            children[i]->release(offsetX, offsetY, isRelative);
        }
    }
 }

//-------------------------------------------
void guiBaseObject::lostFocus() {
    for(unsigned int i = 0; i < children.size(); i++){
        children[i]->lostFocus();
    }
}

//should  be called on key press
//-------------------------------------------
bool guiBaseObject::keyPressed(int k){
	for(unsigned int i = 0; i < children.size(); i++){
		bool eaten = children[i]->keyPressed(k);
		if ( eaten ){
			return true;
        }
	}
	return false;
}

bool guiBaseObject::keyReleased(int k){
	for(unsigned int i = 0; i < children.size(); i++){
		bool eaten = children[i]->keyReleased(k);
        if ( eaten ){
			return true;
        }
	}
	return false;
}


//these are here for the custom control types
//we notify all elements about these actions
bool guiBaseObject::saveSettings(string filename){ return true;}
bool guiBaseObject::loadSettings(string filename){ return true;}

void guiBaseObject::lock() {
    
	locked = true;
}

void guiBaseObject::unlock(){
	locked = false;
}

bool guiBaseObject::isLocked(){
	return locked;
}

//------------------------------------------------
void guiBaseObject::setEnabled( bool ab ) {
    if(ab) {
        if( !bEnabled ) {
            setNormal();
            onEnabledChanged();
        }
    } else {
        if( bEnabled ) {
            setLocked();
            onEnabledChanged();
        }
    }
    bEnabled = ab;
}

//------------------------------------------------
bool guiBaseObject::isEnabled() {
    return bEnabled;
}

//------------------------------------------------
void guiBaseObject::setShowText(bool showText){
	bShowText = showText;
}

//-----------------------------------------------
void guiBaseObject::setPosition(float x, float y){
	boundingBox.x = x;
	boundingBox.y = y;
	hitArea.x     = x;
	hitArea.y     = y;
}

//------------------------------------------------
void guiBaseObject::setDimensions(float width, float height){
	hitArea.width       = width;
	hitArea.height      = height;
	boundingBox.width   = width;
	boundingBox.height  = height;
}

//-----------------------------------------------
float guiBaseObject::getPosX(){
	return boundingBox.x;
}

//------------------------------------------------
float guiBaseObject::getPosY(){
	return boundingBox.y;
}

//------------------------------------------------
float guiBaseObject::getWidth(){
	return boundingBox.width;

}

//-----------------------------------------------
float guiBaseObject::getHeight(){
	 return boundingBox.height;
}

//-----------------------------------------------
float guiBaseObject::getHorizontalSpacing(){
    return 16.0;
}

//-----------------------------------------------
float guiBaseObject::getVerticalSpacing(){
    return 10.0;
}

//-------------------------------------------
float guiBaseObject::getDefaultColumnWidth(){
    return 260;
}

//-----------------------------------------------
void guiBaseObject::update(){
	updateText();
}

////-----------------------------------------------
//void guiBaseObject::notify(){
//	guiCallbackData cbVal;
//	cbVal.setup(xmlName, name);
//    
//    for(int i = 0; i < value.getNumValues(); i++){
//    
//        string type = value.getTypeAsString(i);
//        if( type == "float" || type == "" ){
//            cbVal.addValueF(value.getValueF(i));
//        }
//        else if( type == "int" || type == "bool" ){
//            cbVal.addValueI(value.getValueI(i));
//        }
//        else if( type == "string" ){
//            cbVal.addValueS(value.getValueAsString(i));
//        }
//    
//    }
//    
//	ofNotifyEvent(guiEvent,cbVal,this);
//	//CB
//}

//-----------------------------------------------
void guiBaseObject::checkPrescison(){
	//here we check the range of floating point values and change number of decimal places based on range
	float range =fabs( value.getMax() - value.getMin() );
	if( range < 0.001 ){
		numDecimalPlaces  = 6;
	}else if( range < 0.01){
		numDecimalPlaces = 5;
	}else if( range < 0.1 ){
		numDecimalPlaces = 4;
	}else if( range < 1){
		numDecimalPlaces = 3;
	}else if( range < 10){
		numDecimalPlaces = 2;
	}else{
		numDecimalPlaces = 1;
	}
}

//        //-----------------------------------------------
//        void setIsRelative( bool bIsRelative ){
//            isRelative = bIsRelative;
//        }

//-----------------------------------------------
string guiBaseObject::getVarsAsString(){
	string str = "";

	for(int i = 0; i < value.getNumValues(); i++){
		if( i > 0 ) str += " ";
        
        str += getVarAsString(i);
        
//        string type = value.getTypeAsString(i);
//
//        if( type == "float" ){
//            checkPrescison();
//            str += ofToString(value.getValueF(i), numDecimalPlaces);
//        }
//        else if( type == "int" ){
//            str += ofToString(value.getValueI(i), 0);
//        }
//        else if( type == "bool" ){
//            str += ofToString(value.getValueB(i), 0);
//        }
//        else if( type == "string" ){
//            str += ofToString(value.getValueAsString(i));
//        }
    }
	
	return str;
}

//-----------------------------------------------
string guiBaseObject::getVarAsString(int awhich) {
    if( awhich < value.getNumValues() ) {
        string type = value.getTypeAsString(awhich);
        
        if( type == "float" ){
            checkPrescison();
            return ofToString(value.getValueF(awhich), numDecimalPlaces);
        }
        else if( type == "int" ){
            return ofToString(value.getValueI(awhich), 0);
        }
        else if( type == "bool" ){
            return ofToString(value.getValueB(awhich), 0);
        }
        else if( type == "string" ){
            return ofToString(value.getValueAsString(awhich));
        }
        
    }
    return "default";
}

//every time we update the value of our text
//-----------------------------------------------
void guiBaseObject::updateText(){
	
	varsString = getVarsAsString();
	drawStr = name;
    if( varsString.length() ){
        drawStr += varsString;
    }

	displayText.setText(drawStr);

	//now update our bounding box
	updateBoundingBox();
}

//---------------------------------------------
void guiBaseObject::updateBoundingBox(){
	if(bShowText){
		//we need to update out hitArea because the text will have moved the gui down
		hitArea.y = boundingBox.y + displayText.getTextHeight() + titleSpacing;
		boundingBox.height = hitArea.height + displayText.getTextHeight() + titleSpacing;
	}else{
		 //we need to update out hitArea because the text will have moved the gui down
		hitArea.y = boundingBox.y;
	}
}

//---------------------------------------------
void guiBaseObject::renderText(float offsetX, float offsetY){
	if(!bShowText) return;

	ofSetColor(textColor.getColor());
	displayText.renderText(boundingBox.x + offsetX, boundingBox.y + displayText.getTextSingleLineHeight() + offsetY);
}

//this also needs to be specified by the extending class
//--------------------------------------------
void guiBaseObject::render(){

}

 //-------------------------------------------
 void guiBaseObject::setSelected(){
    fgColor.setGuiColorMode(1);
    bgColor.setGuiColorMode(1);
    outlineColor.setGuiColorMode(1);
    textColor.setGuiColorMode(1);
    triDefaultColor.setGuiColorMode(1);
 }

 //-------------------------------------------
 void guiBaseObject::setNormal(){
	fgColor.setGuiColorMode(0);
	bgColor.setGuiColorMode(0);
	outlineColor.setGuiColorMode(0);
	textColor.setGuiColorMode(0);
    triDefaultColor.setGuiColorMode(0);
 }

//-------------------------------------------
void guiBaseObject::setLocked() {
    fgColor.setGuiColorMode(2);
    bgColor.setGuiColorMode(2);
    outlineColor.setGuiColorMode(2);
    textColor.setGuiColorMode(2);
    triDefaultColor.setGuiColorMode(2);
}

 //-------------------------------------------
 void guiBaseObject::setForegroundColor( int norR, int norG, int norB, int norA = 255){
	fgColor.setColor(norR, norG, norB, norA);
 }

 //-------------------------------------------
 void guiBaseObject::setForegroundSelectColor(int selR, int selG, int selB, int selA = 255){
	fgColor.setSelectedColor(selR, selG, selB, selA);
 }

 //-------------------------------------------
 void guiBaseObject::setBackgroundColor( int norR, int norG, int norB, int norA = 255){
	bgColor.setColor(norR, norG, norB, norA);
 }

 //-------------------------------------------
 void guiBaseObject::setBackgroundSelectColor(int selR, int selG, int selB, int selA = 255){
	bgColor.setSelectedColor(selR, selG, selB, selA);
 }

 //-------------------------------------------
 void guiBaseObject::setOutlineColor( int norR, int norG, int norB, int norA = 255){
	outlineColor.setColor(norR, norG, norB, norA);
 }

 //-------------------------------------------
 void guiBaseObject::setOutlineSelectColor(int selR, int selG, int selB, int selA = 255){
	outlineColor.setSelectedColor(selR, selG, selB, selA);
 }

 //-------------------------------------------
 void guiBaseObject::setTextColor( int norR, int norG, int norB, int norA = 255){
	textColor.setColor(norR, norG, norB, norA);
 }

 //-------------------------------------------
 void guiBaseObject::setTextSelectColor(int selR, int selG, int selB, int selA = 255){
	textColor.setSelectedColor(selR, selG, selB, selA);
 }
 
 //-------------------------------------------
 void guiBaseObject::setValue(float _value, int whichParam) {
	 value.setValue(_value,whichParam);
 }

 //-------------------------------------------
 void guiBaseObject::updateValue() {
     for( int i = 0; i < children.size(); i++ ) {
         children[i]->updateValue();
     }
}


//---------------------------------------------
void guiBaseObject::addRectangleToMesh( ofMesh& amesh, ofRectangle arect, ofFloatColor acolor ) {
    int tVoffset = amesh.getNumVertices();
    vector<glm::vec3> rverts;
    rverts.resize(4);
    rverts[0] = glm::vec3( arect.x, arect.y, 0 );
    rverts[1] = glm::vec3( arect.getRight(), arect.y, 0 );
    rverts[2] = glm::vec3( arect.getRight(), arect.getBottom(), 0 );
    rverts[3] = glm::vec3( arect.x, arect.getBottom(), 0 );
    
    vector<ofFloatColor> rcolors;
    rcolors.assign( 4, acolor );
    
    vector<ofIndexType> rindices;
    rindices.resize( 6 );
    rindices[0] = tVoffset + 0;
    rindices[1] = tVoffset + 1;
    rindices[2] = tVoffset + 2;
    
    rindices[3] = tVoffset + 0;
    rindices[4] = tVoffset + 2;
    rindices[5] = tVoffset + 3;
    
    amesh.addVertices( rverts );
    amesh.addColors( rcolors );
    amesh.addIndices( rindices );
}

//---------------------------------------------
void guiBaseObject::addRectangleToLinesMesh( ofMesh& amesh, ofRectangle arect, ofFloatColor acolor ) {
    int tVoffset = amesh.getNumVertices();
    vector<glm::vec3> rverts;
    rverts.resize(4);
    rverts[0] = glm::vec3( arect.x, arect.y, 0 );
    rverts[1] = glm::vec3( arect.getRight(), arect.y, 0 );
    rverts[2] = glm::vec3( arect.getRight(), arect.getBottom(), 0 );
    rverts[3] = glm::vec3( arect.x, arect.getBottom(), 0 );
    
    vector<ofFloatColor> rcolors;
    rcolors.assign( 4, acolor );
    
    vector<ofIndexType> rindices;
    rindices.resize( 8 );
    // top line
    rindices[0] = tVoffset + 0;
    rindices[1] = tVoffset + 1;
    
    // right side
    rindices[2] = tVoffset + 1;
    rindices[3] = tVoffset + 2;
    
    // bottom
    rindices[4] = tVoffset + 2;
    rindices[5] = tVoffset + 3;
    
    // left
    rindices[6] = tVoffset + 3;
    rindices[7] = tVoffset + 0;
    
    amesh.addVertices( rverts );
    amesh.addColors( rcolors );
    amesh.addIndices( rindices );
}

//---------------------------------------------
void guiBaseObject::addTriangleToMesh( ofMesh& amesh, float a1x, float a1y, float a2x, float a2y, float a3x, float a3y, ofFloatColor acolor ) {
    int tVoffset = amesh.getNumVertices();
    vector<glm::vec3> rverts;
    rverts.resize(3);
    rverts[0] = glm::vec3( a1x, a1y, 0 );
    rverts[1]= glm::vec3( a2x, a2y , 0);
    rverts[2]= glm::vec3( a3x, a3y , 0);
    
    vector<ofFloatColor> rcolors;
    rcolors.assign( 3, acolor );
    
    vector<ofIndexType> rindices;
    rindices.resize( 3 );
    rindices[0] = tVoffset + 0;
    rindices[1] = tVoffset + 1;
    rindices[2] = tVoffset + 2;
    
    amesh.addVertices( rverts );
    amesh.addColors( rcolors );
    amesh.addIndices( rindices );
}

//---------------------------------------------
void guiBaseObject::addLineToMesh( ofMesh& amesh, float a1x, float a1y, float a2x, float a2y, ofFloatColor acolor ) {
    int tVoffset = amesh.getNumVertices();
    vector<glm::vec3> rverts;
    rverts.resize(2);
    rverts[0] = glm::vec3( a1x, a1y, 0 );
    rverts[1] = glm::vec3( a2x, a2y, 0 );
    
    vector<ofFloatColor> rcolors;
    rcolors.assign( 2, acolor );
    
    vector<ofIndexType> rindices;
    rindices.resize( 2 );
    rindices[0] = tVoffset + 0;
    rindices[1] = tVoffset + 1;
    
    amesh.addVertices( rverts );
    amesh.addColors( rcolors );
    amesh.addIndices( rindices );
    
}



