#include "guiTypePanel.h"
#include "ofxControlPanel.h"

//------------------------------------------------
guiTypePanel::guiTypePanel(){
	currentXPos     = 10;
	currentYPos     = 0;
    
	spacingAmntX    = 16;
	spacingAmntY    = 0;
	
	showOnly		= false;
	resetSelectedElement();
	
    disableStatusBar();
    enableStatusBar();
    //setStatus("Status", "Some debug message");
    
	columns.clear();
	columns.push_back(ofRectangle(currentXPos, currentYPos, 30, 20));
	col = 0;
}

//------------------------------------------------
void guiTypePanel::setup(string panelName){
	name = panelName;

	columns[0] = ofRectangle(currentXPos, currentYPos, 50, 20);

	//we don't want our panel flashing when we click :)
	bgColor.selected = bgColor.color;
	outlineColor.selected = outlineColor.color;
}

//-----------------------------------------------
void guiTypePanel::enableStatusBar(){
    showStatus = true;
}

//-----------------------------------------------
void guiTypePanel::disableStatusBar(){
    showStatus = false;
}

//-----------------------------------------------
void guiTypePanel::setStatus(ofParameter <string> & msg){
    statusGroup.clear();
    statusGroup.add(msg);
}

//-----------------------------------------------
void guiTypePanel::addColumn(float minWidth){
	float colX = columns.back().x + columns.back().width + spacingAmntX;
	columns.push_back(ofRectangle(colX, currentYPos, minWidth, 20));
}

//-----------------------------------------------
bool guiTypePanel::selectColumn(int which){
	col = ofClamp(which, 0, columns.size()-1);
	return true;
}

//-----------------------------------------------
void guiTypePanel::setElementSpacing(float spacingX, float spacingY){
	spacingAmntX = spacingX;
	spacingAmntY = spacingY;
}

//-----------------------------------------------.
bool guiTypePanel::checkHit(float x, float y, bool isRelative){
	
	if( boundingBox.inside(x, y) ){
		float xx = x - boundingBox.x;
		float yy = y - boundingBox.y;

		if( lockRect.inside(xx, yy) ){
			locked = !locked;
		}
	}
    
    elementInteracting = false;
    
    int prevElementInteracting = elementWithFocusIndex;
    whichElementInteracting = 0;
    elementWithFocusIndex = -1;
	
	if( hitArea.inside(x, y) ){
		state = SG_STATE_SELECTED;
		setSelected();

		if( !locked ){

			float offsetX = x - hitArea.x;
			float offsetY = y - hitArea.y;
            bool bHitSomething = false;
			for(unsigned int i = 0; i < children.size(); i++){
				bool result = children[i]->checkHit(offsetX, offsetY, isRelative);
				if(result){
                    bHitSomething = true;
					elementInteracting = true;
					whichElementInteracting = i;
                    elementWithFocusIndex = i;
					//return true;
                    break;
				}
			}
            
            if( bHitSomething ) {
//                cout << "prevElementInteracting: " << prevElementInteracting << " elementWithFocusIndex: " << elementWithFocusIndex << endl;
                if(prevElementInteracting > -1 && prevElementInteracting != elementWithFocusIndex ) {
                    if( prevElementInteracting < children.size() ) {
                        children[prevElementInteracting]->lostFocus();
                    }
                }
                
                return true;
            }
		}
	}
	
	return false;
}

//---------------------------------------------
void guiTypePanel::updateBoundingBox(){


}

//-----------------------------------------------.
void guiTypePanel::updateGui(float x, float y, bool firstHit, bool isRelative){
	if( state == SG_STATE_SELECTED){

		float offsetX = 0;
		float offsetY = 0;

		if( isRelative ){
			offsetX = x;
			offsetY = y;
		}else{
			offsetX = x - hitArea.x;
			offsetY = y - hitArea.y;
		}

		if( !locked ){

			for(unsigned int i = 0; i < children.size(); i++){
				children[i]->updateGui(offsetX, offsetY, firstHit, isRelative);
			}

		}
	}
}

//----------------------------------------------
void guiTypePanel::mouseMoved(float x, float y, bool isRelative) {
//
//    if( state == SG_STATE_SELECTED){
    
		float offsetX = 0;
		float offsetY = 0;
        
		if( isRelative ){
			offsetX = x;
			offsetY = y;
		}else{
			offsetX = x - hitArea.x;
			offsetY = y - hitArea.y;
		}
        
		if( !locked ){
            
			for(unsigned int i = 0; i < children.size(); i++){
//                cout << "ofxControlPanel :: mouseMoved : state = " << state << " | " << ofGetElapsedTimef() << endl;
				children[i]->mouseMoved( offsetX, offsetY );
			}
            
		}
//	}
}

//we should actually be checking our child heights
//every frame to see if the panel needs to adjust layout
//for now we only check heights when elements are first added
//----------------------------------------------
void guiTypePanel::update(){
	updateText();

	lockRect.x          = boundingBox.width - (LOCK_WIDTH + spacingAmntX + LOCK_BORDER);
	lockRect.y          = spacingAmntY - LOCK_BORDER;
	lockRect.width      = LOCK_WIDTH + LOCK_BORDER * 2;
	lockRect.height     = LOCK_HEIGHT + LOCK_BORDER * 2;

	for(unsigned int i = 0; i < children.size(); i++){
		children[i]->update();
	}

	for(unsigned int i = 0; i < whichColumn.size(); i++){
		if( children[i]->boundingBox.x != columns[whichColumn[i]].x ){
			float amntToShiftX = columns[whichColumn[i]].x - children[i]->boundingBox.x;

			children[i]->hitArea.x     += amntToShiftX;
			children[i]->boundingBox.x += amntToShiftX;
		}
	}
}

//-----------------------------------------------
void guiTypePanel::resetSelectedElement(){
	elementInteracting = false;
	whichElementInteracting = 0;
}

//-----------------------------------------------
void guiTypePanel::setShowOnlySelectedElement(bool showOnlySelected){
	showOnly = showOnlySelected;
}


//-----------------------------------------------
void guiTypePanel::addSpace( int height ) {
	columns[col].y += height;
}

//-----------------------------------------------
void guiTypePanel::addElement( shared_ptr<guiBaseObject> element ){
    if( !element ) return;
	element->updateText();
    
    if(columns[col].y + element->getHeight() + 30.0 >= getHeight()){
    
        int nextCol = col+1;
        if( nextCol >= columns.size() ){
            addColumn(30);
        }
        col = nextCol; 
    }
    
	//update the current position for the next element
	columns[col].y += spacingAmntY + element->getVerticalSpacing();

	element->setPosition(columns[col].x, columns[col].y);
    
    columns[col].y += element->getHeight();

	whichColumn.push_back(col);

	//add the element to the panel list
	children.push_back( element );

	float checkWidth = element->getWidth();
		
	if(checkWidth >= columns[col].width && !element->bRemoveFromLayout ){
		float amnt = checkWidth - columns[col].width;
		columns[col].width += amnt;

		for(unsigned int i = col+1; i < columns.size(); i++){
			columns[i].x += amnt;
		}
		
	}

	//see if we need to resize!
	//checkResize(element);
}

//-----------------------------------------------
void guiTypePanel::removeElement( shared_ptr<guiBaseObject> element ) {
    if( !element ) return;
    bool found = false;
    for( int i=0; i < children.size(); i++ ){
        if( children[i] && children[i].get() == element.get() ){
            // remove from children
            children.erase( children.begin()+i );
            // adjust column height
            columns[whichColumn[i]].y -= element->getHeight() + spacingAmntY + element->getVerticalSpacing();
            // remove from whichColumn
            whichColumn.erase( whichColumn.begin()+i );
            found = true;
            break;
        }
    }
}

//-----------------------------------------------
bool guiTypePanel::containsElement( shared_ptr<guiBaseObject> element ) {
    if( !element ) return;
    vector < shared_ptr<guiBaseObject> >::iterator elementIter = std::find( children.begin(), children.end(), element );
    return ( elementIter != children.end() );
}

//-----------------------------------------------
bool guiTypePanel::containsElement( string xmlName ){
    for ( int i=0; i < children.size(); i++ ){
        if( children[i] && children[i]->xmlName == xmlName ){
            return true;
        }
    }
    return false;
}

//-----------------------------------------------
shared_ptr<guiBaseObject> guiTypePanel::getElement( string xmlName ){
    for ( int i=0; i<children.size(); i++ ){
        if( children[i] && children[i]->xmlName == xmlName ){
            return children[i];
        }
    }
    return NULL;
}

//-----------------------------------------------.
void guiTypePanel::drawLocked(){
    ofPushMatrix(); {
		ofFill();
		ofTranslate(lockRect.x, lockRect.y, 0);

//		ofSetColor(200, 0, 0, bgColor.getColor()[3]*255.0);
        ofSetColor( fgColor.getSelectedColor() );
		ofDrawRectangle(0, 0, lockRect.width, lockRect.height);

		ofTranslate(LOCK_BORDER, LOCK_BORDER, 0);

		ofSetColor(255, 255, 255);
		ofDrawEllipse(LOCK_WIDTH/2, LOCK_HEIGHT/2, LOCK_WIDTH * 0.8, LOCK_HEIGHT * 0.9);

//		ofSetColor(200, 0, 0);
        ofSetColor( fgColor.getSelectedColor() );
		ofDrawEllipse(LOCK_WIDTH/2, LOCK_HEIGHT/2, LOCK_WIDTH * 0.8 * 0.6, LOCK_HEIGHT * 0.9 * 0.6);

		ofSetColor(255, 255, 255);
		ofDrawRectangle(0, LOCK_HEIGHT/2, LOCK_WIDTH, LOCK_HEIGHT/2);
    } ofPopMatrix();
}

//-----------------------------------------------.
void guiTypePanel::drawUnlocked(){
	ofPushMatrix();
		ofFill();
		ofTranslate(lockRect.x, lockRect.y, 0);

		ofSetColor(0, 0, 0, bgColor.getColor()[3]*255.0);
		ofDrawRectangle(0, 0, lockRect.width, lockRect.height);

		ofTranslate(LOCK_BORDER, LOCK_BORDER, 0);
		ofSetColor(255, 255, 255);
		ofDrawEllipse(LOCK_WIDTH/2, LOCK_HEIGHT * 0.4, LOCK_WIDTH * 0.8, LOCK_HEIGHT * 0.9);

		ofSetColor(0, 0, 0);
		ofDrawEllipse(LOCK_WIDTH/2, LOCK_HEIGHT * 0.44, LOCK_WIDTH * 0.8 * 0.6, LOCK_HEIGHT * 0.9 * 0.6);

		ofSetColor(255, 255, 255);
		ofDrawRectangle(0, LOCK_HEIGHT/2, LOCK_WIDTH, LOCK_HEIGHT/2);

		ofSetColor(0, 0, 0);
		ofDrawRectangle(0, LOCK_HEIGHT * 0.5 - LOCK_HEIGHT * 0.25 , LOCK_WIDTH * 0.35, LOCK_HEIGHT * 0.25);

	ofPopMatrix();
}

//-----------------------------------------------.
void guiTypePanel::render(){
	ofPushStyle();

		if( showOnly && elementInteracting ){
            
			ofPushMatrix();
				ofTranslate(hitArea.x, hitArea.y, 0);
				for(unsigned int i = 0; i < children.size(); i++){
					int elementToRender = children.size()-(1+i);
					if( elementToRender == whichElementInteracting && children[elementToRender] ){
						children[elementToRender]->render();
					}
				}
			ofPopMatrix();
					
		}else{
//		cout << "Drawing panel: " << name << " | " << ofGetFrameNum() << endl;
            
            //    mutThis->setAlphaBitmapText(true);
//            ofMesh charMesh = aBitmapFont.getMesh(aString, ax, ay, OF_BITMAPMODE_SCREEN, false );
//            amesh.addVertices( charMesh.getVertices() );
//            amesh.addTexCoords( charMesh.getTexCoords() );
            //    mutThis->bind( aBitmapFont.getTexture(),0);
            //    draw(charMesh,OF_MESH_FILL,false,true,false);
            //    mutThis->unbind(aBitmapFont.getTexture(),0);
            //    mutThis->setAlphaBitmapText(false);
            #ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
            mLinesMesh.setMode( OF_PRIMITIVE_LINES );
            mLinesMesh.clear();
            mRenderMesh.clear();
            mTextMesh.clear();
            #endif
            
            ofPushMatrix(); {
                ofTranslate(boundingBox.x, boundingBox.y, 0);
							
				//draw the background
				ofFill();
				ofSetColor(bgColor.getColor());
				ofDrawRectangle(0, 0, boundingBox.width-2, boundingBox.height);

				//draw the outline
//				ofNoFill();
//				ofSetColor(outlineColor.getColor());
//				ofRect(0, 0, boundingBox.width, boundingBox.height);

//                if( locked ){
//                    drawLocked();
//                }else{
//                    drawUnlocked();
//                }
					
            } ofPopMatrix();
			//renderText();

            ofPushMatrix(); {
				ofTranslate(hitArea.x, hitArea.y, 0);
                
                #ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
                for(unsigned int i = 0; i < children.size(); i++){
                    children[i]->addToRenderMesh(mRenderMesh);
                    children[i]->addToLinesRenderMesh(mLinesMesh);
                    children[i]->addToTextRenderMesh(mTextMesh);
                }
                mRenderMesh.draw();
                #endif
				for(unsigned int i = 0; i < children.size(); i++){
					children[i]->render();
				}
                #ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
                mLinesMesh.draw();
            
                if( displayText.usingTTF() ) {
                    displayText.ourFont->getFontTexture().bind();
                } else {
                    ofxControlPanel::bitmapFont.getTexture().bind();
                }
                ofSetColor( textColor.getColor() );
                mTextMesh.draw();
                if( displayText.usingTTF() ) {
                    displayText.ourFont->getFontTexture().unbind();
                } else {
                    ofxControlPanel::bitmapFont.getTexture().unbind();
                }
                #endif
                
                for(unsigned int i = 0; i < children.size(); i++){
                    children[i]->renderOnTop();
                }
                
            } ofPopMatrix();
            
            if( locked ){
                ofSetColor( 10, 10, 10, 100 );
                ofDrawRectangle( boundingBox );
            }
            
            ofPushMatrix(); {
                ofTranslate(boundingBox.x, boundingBox.y, 0);
                if( locked ){
                    drawLocked();
                }else{
                    drawUnlocked();
                }
                
            } ofPopMatrix();
            
            if( locked ) {
                ofPushMatrix(); {
                    ofTranslate(boundingBox.getCenter());
                    
//                    float bw = 90;
                    float tw = 28;
//                    ofSetColor( 10, 100 );
//                    ofDrawRectangle(-bw/2, -bw/2, bw, bw );
                    ofSetColor( 140 );
                    ofDrawCircle( 0., -tw/2, tw*0.45 );
                    ofSetColor( 10 );
                    ofDrawCircle( 0., -tw/2, tw*0.25 );
                    
                    ofSetColor( 140 );
                    ofDrawRectangle(-tw/2, -tw/2, tw, tw );
                    
                } ofPopMatrix();
            }

        }
	ofPopStyle();
}

void guiTypePanel::renderStatus(){

    if( bShowText && statusGroup.size() > 0 ){
        ofPushMatrix();
        
        float boxHeight = displayText.getTextHeight() * 1.2;
        ofTranslate(boundingBox.x, boundingBox.getBottom()-boxHeight, 0);
    
        ofSetColor(bgColor.getColor());
        ofDrawRectangle(0, 0, boundingBox.width, boxHeight);
        ofNoFill();
        ofSetColor(outlineColor.getColor());
        ofDrawRectangle(0, 0, boundingBox.width, boxHeight);

        string statusParam = statusGroup.getString(0);
        
        //handle newlines
        int pos = statusParam.find('\n');
        if( pos != string::npos ){
            statusParam.erase(statusParam.begin() + pos, statusParam.end() );
        }
        
        ofSetColor(textColor.getColor());
        displayText.renderString("Status: " + statusParam, 3, boxHeight-3);

        ofPopMatrix();
    }
}
