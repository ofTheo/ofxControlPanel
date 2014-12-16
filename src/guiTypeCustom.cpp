#include "guiTypeCustom.h"

//------------------------------------------------
guiTypeCustom::guiTypeCustom(){
	custom = NULL;
	bgColor.setSelectedColor(0, 0, 0, 255);
	bgColor.setColor(0, 0, 0, 255);
}

//------------------------------------------------
guiTypeCustom::~guiTypeCustom(){
	custom = NULL;
}

//------------------------------------------------
void guiTypeCustom::setup(string customName, guiCustomImpl * customIn, float panelWidth, float panelHeight){
	custom = customIn;
	name = customName;
	updateText();

	setDimensions(panelWidth, panelHeight);
	if( custom != NULL )custom->boundingRect = hitArea;
}

//-----------------------------------------------
void guiTypeCustom::saveSettings(string filename){
	if( custom != NULL)custom->saveSettings(filename);
}

//-----------------------------------------------
void guiTypeCustom::loadSettings(string filename){
	if( custom != NULL)custom->loadSettings(filename);
}

//-----------------------------------------------
void guiTypeCustom::update(){
	updateText();
	if( custom != NULL)custom->boundingRect = hitArea;
	if( custom != NULL)custom->update();
}

//-----------------------------------------------.
void guiTypeCustom::updateGui(float x, float y, bool firstHit, bool isRelative){
	if( custom == NULL || state != SG_STATE_SELECTED)return;

	float realX = x;
	float realY = y;

	custom->mouseIsRelative = isRelative;

    int button = 0;
    if( isRelative ){
        button = 2;
    }

	if(firstHit){
		custom->mousePressed(realX, realY, button);
	}else{
		custom->mouseDragged(realX, realY, button);
	}
}

//should  be called on mouse up
//-------------------------------------------
void guiTypeCustom::release(float x, float y, bool isRelative){
	if( custom == NULL)return;

	state = SG_STATE_NORMAL;
	setNormal();
    
    int button = 0;
    if( isRelative ){
        button = 2;
    }
    
	custom->mouseReleased(x, y, button);
 }

//-----------------------------------------------.
void guiTypeCustom::mouseMoved(float x, float y) {
    if( custom == NULL)return;
    custom->mouseMoved( x, y);
}

//-----------------------------------------------.
bool guiTypeCustom::keyPressed(int key){
	if( custom == NULL)return false;
    return custom->keyPressed(key);
}

//-----------------------------------------------.
bool guiTypeCustom::keyReleased(int key){
	if( custom == NULL)return false;
    return custom->keyReleased(key);
}

//-----------------------------------------------.
void guiTypeCustom::render(){

	ofPushStyle();

		glPushMatrix();
			guiBaseObject::renderText();

			//draw the background
			ofFill();
			glColor4fv(bgColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			ofDisableAlphaBlending();

			ofSetColor(255, 255, 255);
			if( custom != NULL)custom->draw(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

		glPopMatrix();

	ofPopStyle();
}
