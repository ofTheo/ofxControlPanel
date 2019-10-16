#include "ofxControlPanel.h"
#include "ofEvents.h"

float ofxControlPanel::borderWidth = 0;
float ofxControlPanel::topSpacing  = 15;
float ofxControlPanel::tabWidth = 35;
float ofxControlPanel::tabHeight = 10;

ofBitmapFont ofxControlPanel::bitmapFont;

vector <ofxControlPanel *> ofxControlPanel::globalPanelList;

//----------------------------
bool ofxControlPanel::shouldRemoveGuiBaseObject( const shared_ptr<guiBaseObject> aObj ) {
    return !aObj;//return (aObj == NULL);
}

//----------------------------
string checkXmlName(string friendlyName, string xmlName){
	if( xmlName == "" ){
		xmlName = friendlyName;
		ofStringReplace(xmlName, " ", "_");
	}
	return xmlName;
}

//----------------------------
ofxControlPanel::ofxControlPanel(){
    dragging        = false;
    minimize        = false;
    selectedPanel   = 0;
    currentPanel    = 0;
    bUseTTFFont     = false;
    usingXml        = true;
    saveDown        = false;
    saveAsDown      = false;
    loadDown        = false;
    restoreDown     = false;
    incrementSave   = false;
    hidden          = false;
    bDraggable      = true;
//	eventsEnabled	= false;
//	bEventsSetup	= false;
	bIgnoreLayout	= false;
	bInvisible		= false;
	lastFrameDrawn	= 0;
	hitSomething	= false;
	elementSelected = false;
	
    currentXmlFile = "";
    incrementSaveName = "";
    xmlObjects.clear();
    
    bHasKeyboardEvents = false;
	
	ofxControlPanel::globalPanelList.push_back(this);
}

//-----------------------------
ofxControlPanel::~ofxControlPanel(){

    xmlObjects.clear();
    
//	for(int i = 0; i < customEvents.size(); i++){
//		if( customEvents[i] != NULL ){
//            ofRemoveListener(guiObjects[i]->guiEvent, this, &ofxControlPanel::eventsIn);
//			delete customEvents[i];
//			customEvents[i] = NULL;
//		}
//	}
//	customEvents.clear();

//    for(unsigned int i = 0; i < guiObjects.size(); i++){
//        if( guiObjects[i] != NULL ){
//			delete guiObjects[i];
//			guiObjects[i] = NULL;
//		}
//    }
	guiObjects.clear();
	
	for(int i = 0; i < ofxControlPanel::globalPanelList.size(); i++){
		if( ofxControlPanel::globalPanelList[i] != NULL && ofxControlPanel::globalPanelList[i]->name == name ){			
			ofxControlPanel::globalPanelList.erase( ofxControlPanel::globalPanelList.begin()+i, ofxControlPanel::globalPanelList.begin()+i+1);
			break;
		}
	}
	
    for(unsigned int i = 0; i < internalParams.size(); i++){
        if( internalParams[i] != NULL ){
			delete internalParams[i];
			internalParams[i] = NULL;
		}
    }
	internalParams.clear();
    
    disableKeyboardEvents();
}

//-----------------------------
ofxControlPanel * ofxControlPanel::getPanelInstance(string panelName){
	for(int i = 0; i < ofxControlPanel::globalPanelList.size(); i++){
		if( ofxControlPanel::globalPanelList[i] != NULL && ofxControlPanel::globalPanelList[i]->name == panelName ){
			return ofxControlPanel::globalPanelList[i];
		}
	}
	return NULL;
}	
		
//-----------------------------
void ofxControlPanel::setup(string controlPanelName, float panelX, float panelY, float width, float height, bool doSaveRestore, bool locked){
			
	bDoSaveRestore = doSaveRestore;
	name = controlPanelName;

	setPosition(panelX, panelY);
	setDimensions(width, height);
	setShowText(true);

	fgColor			= gFgColor;
	outlineColor	= gOutlineColor;				
	bgColor			= gBgColor;
	textColor		= gTextColor;	 
	
	ofAddListener(ofEvents().mousePressed, this, &ofxControlPanel::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxControlPanel::mouseReleased);
	ofAddListener(ofEvents().mouseDragged, this, &ofxControlPanel::mouseDragged);
    ofAddListener(ofEvents().mouseMoved, this, &ofxControlPanel::mouseMoved);
	enableKeyboardEvents();
    
	updateBoundingBox();
    
    if( panels.size() == 0 ){
		addPanel(controlPanelName, 6, locked);
	} 

}

//-----------------------------
void ofxControlPanel::loadFont(string fontName, int fontsize ){
    guiTTFFont.load(fontName, fontsize);
    bool okay = guiTTFFont.isLoaded();
	guiBaseObject::setFont(&guiTTFFont);

    if(okay){
		printf("font loaded okay!\n");
        bUseTTFFont = true;
        for(unsigned int i = 0; i < guiObjects.size(); i++){
            guiObjects[i]->setFont(&guiTTFFont);
        }
    }else{
		printf("ahhhhhh why does my font no work!\n");
	}
}

//---------------------------------------------

void ofxControlPanel::setSize( int new_width, int new_height ){
//    printf("ofxControlPanel::setSize\n");
    
	// set width
	setDimensions( new_width, new_height );
	// set underlying panel widths
	for ( int i=0; i<panels.size(); i++ )
	{
		panels[i]->setDimensions( (boundingBox.width - borderWidth*2) -1, boundingBox.height - topSpacing*3);
	}
}

// keyboard events
//---------------------------------------------
void ofxControlPanel::enableKeyboardEvents() {
    if(!bHasKeyboardEvents) {
        ofAddListener(ofEvents().keyPressed, this, &ofxControlPanel::keyPressed);
        ofAddListener(ofEvents().keyReleased, this, &ofxControlPanel::keyReleased);
    }
    bHasKeyboardEvents = true;
}

//---------------------------------------------
void ofxControlPanel::disableKeyboardEvents() {
    if(bHasKeyboardEvents) {
        ofRemoveListener(ofEvents().keyPressed, this, &ofxControlPanel::keyPressed);
        ofRemoveListener(ofEvents().keyReleased, this, &ofxControlPanel::keyReleased);
    }
    bHasKeyboardEvents = false;
}

//---------------------------------------------
bool ofxControlPanel::hasPanel( string aPanelName ) {
    for(int i = 0; i < (int) panels.size(); i++){
        if( panels[i]->name == aPanelName) {
            return true;
        }
    }
    return false;
}

//---------------------------------------------
weak_ptr<guiTypePanel> ofxControlPanel::addPanel(string panelName, int numColumns, bool locked){
//    guiTypePanel * panelPtr = new guiTypePanel();
    auto panelPtr = make_shared<guiTypePanel>();
    panelPtr->setup(panelName);
    
    
    float yShift = topSpacing * 2;
    panelPtr->setPosition(borderWidth, 0);
    
    float panelH = boundingBox.height-yShift;
//    cout << " bounding box height is " << boundingBox.getHeight() << " yShift is " << yShift << " panelH is " << panelH << endl;
    
    panelPtr->setDimensions(boundingBox.width, panelH);
    
    if( locked )panelPtr->lock();
    else panelPtr->unlock();

    if( numColumns > 1 ){
        for(int i = 1; i < numColumns; i++){
            panelPtr->addColumn(30);
        }
    }

    panels.push_back(panelPtr);
    panelTabs.push_back(ofRectangle());

    guiObjects.push_back(panelPtr);
    if( bUseTTFFont ){
        panelPtr->setFont(&guiTTFFont);
    }
	
	setWhichPanel(panels.size()-1);

    return panelPtr;
}

//---------------------------------------------
void ofxControlPanel::removePanel( string panelName ) {
    for(int i = 0; i < (int) panels.size(); i++){
        if( panels[i]->name == panelName ){
            for(int j = 0; j < panels[i]->children.size(); j++ ) {
                
                //xmlObjects
                
                for(int k = 0; k < guiObjects.size(); k++ ) {
                    if(panels[i]->children[j] == xmlObjects[k].guiObj ) {
//                        guiObjects[k] = NULL;
                        xmlObjects.erase( xmlObjects.begin() + k );
                        break;
                    }
                }
                
                for(int k = 0; k < guiObjects.size(); k++ ) {
                    if(panels[i]->children[j] == guiObjects[k] ) {
                        guiObjects[k] = NULL;
                        break;
                    }
                }
                
                //delete panels[i]->children[j];
                //panels[i]->children[j] = NULL;
            }
            panels[i]->children.clear();
            
            panelTabs.erase( panelTabs.begin()+i );
            
            for(int j = 0; j < guiObjects.size(); j++ ) {
                if(guiObjects[j]) {
                    if(guiObjects[j] == panels[i] ) {
//                        delete  guiObjects[j];
                        //guiObjects[j] = NULL;
                        guiObjects[j].reset();
                        guiObjects.erase( guiObjects.begin()+j );
                        break;
                    }
                }
            }
            
            if( currentPanel == i ) {
                setWhichPanel( panels.size()-1 );
            }
            
            
            //delete panels[i];
            //panels[i] = NULL;
            panels[i].reset();
            
            panels.erase( panels.begin() + i );
            
            
            break;
        }
    }
    ofRemove( guiObjects, shouldRemoveGuiBaseObject );
}

// ############################################################## //
// ##
// ##      Control where things are added to
// ##
// ############################################################## //


//---------------------------------------------
void ofxControlPanel::setWhichPanel(int whichPanel){
    if( whichPanel < 0 || whichPanel >= (int) panels.size() )return;
    currentPanel = whichPanel;
    setWhichColumn(0);
}

//---------------------------------------------
void ofxControlPanel::lock() {
    for( int i = 0; i < panels.size(); i++ ) {
        panels[i]->lock();
    }
}

//---------------------------------------------
void ofxControlPanel::unlock() {
    for( int i = 0; i < panels.size(); i++ ) {
        panels[i]->unlock();
    }
}

//---------------------------------------------
void ofxControlPanel::setWhichPanel(string panelName){
    for(int i = 0; i < (int) panels.size(); i++){
        if( panels[i]->name == panelName){
            setWhichPanel(i);
            setWhichColumn(0);
            return;
        }
    }
}

//---------------------------------------------
void ofxControlPanel::setWhichColumn(int column){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return;
    panels[currentPanel]->selectColumn(column);
}

//---------------------------------------------
int ofxControlPanel::getSelectedPanel(){
	return selectedPanel;
}

//---------------------------------------------
void ofxControlPanel::setSelectedPanel(int whichPanel){
	if( whichPanel >= 0 && whichPanel < panels.size()){
        
        int lastSelectedPanel = selectedPanel;
        selectedPanel = whichPanel;
        if( lastSelectedPanel != selectedPanel ){
            bNewPanelSelected = true;
            panels[lastSelectedPanel]->lostFocus();
        } else {
            bNewPanelSelected = false;
        }
	}
}

//---------------------------------------------
void ofxControlPanel::setEnabled( ofParameterGroup& agroup, bool ab ) {
    if( agroup.getName().size() < 2 ) return;
    if( agroup.size() < 1 ) return;
//    cout << "ofxControlPanel : setEnabled : " << agroup.getName() << endl;
    for( int i = 0; i < xmlObjects.size(); i++ ) {
        if(xmlObjects[i].guiObj && xmlObjects[i].guiObj->value.paramGroup.size() ) {
            // try to find the pointer for the param, so it's not just name based
//            cout << "ofxControlPanel : setEnabled : " << agroup.getName() << " trying to match: " << xmlObjects[i].guiObj->value.paramGroup.getName() << endl;
            if( agroup.getName() == xmlObjects[i].guiObj->value.paramGroup.getName() ) {
//                cout << "ofxControlPanel : setEnabled : group names match: " << agroup.getName() << endl;
                // check for the param //
                bool bFoundIt=false;
                for( int k = 0; k < xmlObjects[i].guiObj->value.paramGroup.size(); k++ ) {
                    for( int j = 0; j < agroup.size(); j++ ) {
                        if( xmlObjects[i].guiObj->value.paramGroup.get(k).isReferenceTo(agroup.get(j)) ) {
                            // we have it, woohoo //
                            xmlObjects[i].guiObj->setEnabled(ab);
//                            cout << "ofxControlPanel : setEnabled : " << agroup.getName() << " " << agroup.get(j).getName() << endl;
                            bFoundIt = true;
                            break;
                        }
                    }
                    if(bFoundIt) break;
                }
            }
        }
    }
}

//---------------------------------------------
void ofxControlPanel::setEnabled( ofAbstractParameter& aparam, bool ab ) {
    if( aparam.getName().size() < 2 ) return;
//    cout << "ofxControlPanel : setEnabled : " << aparam.getName() << endl;
    for( int i = 0; i < xmlObjects.size(); i++ ) {
        if(xmlObjects[i].guiObj && xmlObjects[i].guiObj->value.paramGroup.size() ) {
            bool bFoundIt=false;
            for( int k = 0; k < xmlObjects[i].guiObj->value.paramGroup.size(); k++ ) {
                if( xmlObjects[i].guiObj->value.paramGroup.get(k).isReferenceTo(aparam) ) {
                    // we have it, woohoo //
                    xmlObjects[i].guiObj->setEnabled(ab);
                    bFoundIt = true;
                    break;
                }
                if(bFoundIt) break;
            }
        }
    }
}

// ############################################################## //
// ##
// ##       Gui elements
// ##
// ############################################################## //

//---------------------------------------------
void ofxControlPanel::addXmlAssociation( shared_ptr<guiBaseObject> object, string xmlName, int paramCount ){
	// check for already-existing xml name
	for( int i=0; i<xmlObjects.size(); i++ ){
		if( xmlObjects[i].xmlName == xmlName ){
			ofLog(OF_LOG_ERROR, "ofxControlPanel::add*: duplicate xmlName '%s', not adding\n", xmlName.c_str() );
			return;
		}
	}
	xmlObjects.push_back( xmlAssociation(object, xmlName, paramCount ) );
}

//---------------------------------------------
shared_ptr<guiBaseObject> ofxControlPanel::addGuiBaseObject( shared_ptr<guiBaseObject> element, int varsToAddToXml) {
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiBaseObject> temp;
        return temp;
    }

	setLayoutFlag(element);

	if( varsToAddToXml > 0 ){
        for(int i = 0; i < varsToAddToXml; i++){
            if( element->value.paramGroup.getName() != ""){
                element->xmlName = element->value.paramGroup.getEscapedName() + ":" + element->xmlName;
            }
        }
		addXmlAssociation(element, element->xmlName, varsToAddToXml);
    }
	
	guiObjects.push_back(element);

    if( bUseTTFFont ){
        element->setFont(&guiTTFFont);
    }

    panels[currentPanel]->addElement(element);
	return element;
}

//---------------------------------------------
void ofxControlPanel::add( ofParameterGroup & group ) {
    vector<bool> temp;
    add( group, temp );
}

//---------------------------------------------
void ofxControlPanel::add( ofParameterGroup & group, vector<bool> bOnesToInclude ) {
    
    if( bOnesToInclude.size() > 0 && group.size() > 0 ) {
        if( bOnesToInclude.size() != group.size() ) {
            ofLogWarning() << "ofxControlPanel :: addGroup : group size and ones to include are diff sizes, including all of them" << endl;
            bOnesToInclude.clear();
        }
    }
    
    if( bOnesToInclude.size() == 0 ) {
        bOnesToInclude.assign( group.size(), true );
    }

    addLabel(group.getName(), true);

    for(int i = 0; i < group.size(); i++){
        if( !bOnesToInclude[i] ) continue;
    
        ofAbstractParameter * ptr = &group.get(i);
        
        if( ptr->type() == typeid(ofParameter<float>).name() ){
            ofParameter <float> & f = dynamic_cast< ofParameter<float> & >( group.get(i) );
            addSlider(f);
        }
        else if( ptr->type() == typeid(ofParameter<int>).name() ){
            ofParameter <int> & f = dynamic_cast< ofParameter<int> & >( group.get(i) );
            addSlider(f);
        }
        else if( ptr->type() == typeid(ofParameter<bool>).name() ){
            ofParameter <bool> & f = dynamic_cast< ofParameter<bool> & >( group.get(i) );
            addToggle(f);
        }else if( ptr->type() == typeid(ofParameter<string>).name() ){
            ofParameter <string> & f = dynamic_cast< ofParameter<string> & >( group.get(i) );
            addLabel(f);
        }
        else if( ptr->type() == typeid(ofParameter<ofColor>).name() ){
            ofParameter<ofColor> & f = dynamic_cast< ofParameter<ofColor> & >( group.get(i) );
            addColorPicker(f);
        }
    }

}

//---------------------------------------------
shared_ptr<guiTypeToggle> ofxControlPanel::addToggle(ofParameter <bool> & param){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiTypeToggle> temp; return temp;
    }

//    guiTypeToggle * tmp = new guiTypeToggle();
    auto tmp = make_shared<guiTypeToggle>();
    tmp->value.addValue(param);
    tmp->setup();
    
	addGuiBaseObject(tmp, 1);
    return tmp;
}

//---------------------------------------------
shared_ptr<guiTypeToggle> ofxControlPanel::add( ofParameter<bool> & param) {
    return addToggle( param );
}

//---------------------------------------------
shared_ptr<guiTypeMultiToggle> ofxControlPanel::addMultiToggle(ofParameter<int> &param, vector <string> boxNames){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiTypeMultiToggle> tt; return tt;
    }
    
    if( param < 0 ){
        param = 0;
    }else if( param >= boxNames.size() ){
        param = boxNames.size()-1;
    }

    //add a new multi toggle to our list
//    guiTypeMultiToggle * tmp = new guiTypeMultiToggle();
    auto tmp = make_shared<guiTypeMultiToggle>();
    tmp->value.addValue(param);
    tmp->setup(boxNames);

	addGuiBaseObject(tmp, 1);
    return tmp;
}

//---------------------------------------------
shared_ptr<guiTypeTextDropDown> ofxControlPanel::addTextDropDown(ofParameter<int> & param, vector <string> boxNames){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ){
        shared_ptr<guiTypeTextDropDown> tt;
        return tt;
    }

	xmlName = checkXmlName(name, xmlName);

    //add a new multi toggle to our list
    auto tmp = make_shared<guiTypeTextDropDown>();
    tmp->value.addValue(param);
    tmp->setup(boxNames);

	addGuiBaseObject(tmp, 1);
    return tmp;
}

//---------------------------------------------
shared_ptr<guiTypeSlider> ofxControlPanel::add( ofParameter <float> &param ) {
    return addSlider( param );
}

//---------------------------------------------
shared_ptr<guiTypeSlider> ofxControlPanel::addSlider(ofParameter <float> & param){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiTypeSlider> tt; return tt;
    }
    
    //add a new slider to our list
    auto tmp = make_shared<guiTypeSlider>();
    
    tmp->value.addValue(param);
    tmp->setup();
    
    addGuiBaseObject(tmp, 1);
    return tmp;
}

//---------------------------------------------
shared_ptr<guiTypeSlider> ofxControlPanel::add( ofParameter<int> &param ) {
    return addSlider( param );
}

//---------------------------------------------
shared_ptr<guiTypeSlider> ofxControlPanel::addSlider(ofParameter <int> & param){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiTypeSlider> tt; return tt;
    }
    
    //add a new slider to our list
    auto tmp = make_shared<guiTypeSlider>();
    
    tmp->value.addValue(param);
    tmp->setup();
    
    addGuiBaseObject(tmp, 1);
    return tmp;
}

//---------------------------------------------
shared_ptr<guiTypeRangeSlider> ofxControlPanel::add( ofParameter<float>& paramMin, ofParameter<float>& paramMax ) {
    return addSlider( paramMin, paramMax );
}

//---------------------------------------------
shared_ptr<guiTypeRangeSlider> ofxControlPanel::addSlider( ofParameter<float>& paramMin, ofParameter<float>& paramMax ) {
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiTypeRangeSlider> tt; return tt;
    }
    
    //add a new slider to our list
    auto tmp = make_shared<guiTypeRangeSlider>();
    
    tmp->value.addValue(paramMin);
    tmp->value.addValue(paramMax);
    tmp->setup();
    
    addGuiBaseObject(tmp, 2);
    return tmp;
}

//-------------------------------
shared_ptr<guiType2DSlider> ofxControlPanel::addSlider2D(ofParameter<int> & param1, ofParameter<int> & param2){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiType2DSlider> tt; return tt;
    }
	
    //add a new slider to our list
    auto tmp = make_shared<guiType2DSlider>();
    tmp->value.addValue(param1);
    tmp->value.addValue(param2);
    tmp->setup();

	addGuiBaseObject(tmp, 2);
    return tmp;
}

//-------------------------------
shared_ptr<guiType2DSlider> ofxControlPanel::addSlider2D(ofParameter<float> & param1, ofParameter<float> & param2){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiType2DSlider> tt; return tt;
    }
	
    //add a new slider to our list
    auto tmp = make_shared<guiType2DSlider>();
    tmp->value.addValue(param1);
    tmp->value.addValue(param2);
    tmp->setup();

	addGuiBaseObject(tmp, 2);
    return tmp;
}


//---------------------------------------------
shared_ptr<guiTypeDrawable> ofxControlPanel::addDrawableRect(string name, ofBaseDraws * drawablePtr, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiTypeDrawable> tt; return tt;
    }

    auto tmp = make_shared<guiTypeDrawable>();
    tmp->setup(name, drawablePtr, drawW, drawH);

	addGuiBaseObject(tmp, 0);
    return tmp;
}

//---------------------------------------------
shared_ptr<guiTypeVideo> ofxControlPanel::addVideoRect(string name, ofVideoPlayer * drawablePtr, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() ) {
        shared_ptr<guiTypeVideo> tt; return tt;
    }
 
	auto tmp = make_shared<guiTypeVideo>();
    tmp->setup(name, drawablePtr, drawW, drawH);

	addGuiBaseObject(tmp, 0);
    return tmp;
}


//---------------------------------------------
shared_ptr<guiTypeCustom> ofxControlPanel::addCustomRect(string name, guiCustomImpl * customPtr, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiTypeCustom> tt; return tt;
    }

    auto tmp = make_shared<guiTypeCustom>();
    tmp->setup(name, customPtr, drawW, drawH);

	addGuiBaseObject(tmp, 0);
    return tmp;
}


//---------------------------------------------
shared_ptr<guiTypeVariableLister> ofxControlPanel::addVariableLister(ofParameterGroup & group){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiTypeVariableLister> tt; return tt;
    }

    //add a new multi toggle to our list
    auto tmp = make_shared<guiTypeVariableLister>();
    tmp->value.paramGroup = group;
    tmp->setup();
	
	addGuiBaseObject(tmp, 0);
    return tmp;
}

//-----------------------------------------------------
shared_ptr<guiTypeChartPlotter> ofxControlPanel::addChartPlotter(ofParameter<float>& param ) {
    return addChartPlotter( param, param.getMin(), param.getMax() );
}

//-----------------------------------------------------
shared_ptr<guiTypeChartPlotter> ofxControlPanel::addChartPlotter(ofParameter <float> & param, float minValY, float maxValY){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiTypeChartPlotter> tt; return tt;
    }

    //add a new multi toggle to our list
    auto tmp = make_shared<guiTypeChartPlotter>();
    tmp->value.addValue(param); 
    tmp->setup(minValY, maxValY);

	addGuiBaseObject(tmp, 0);
    return tmp;
}

//-----------------------------------------------------
shared_ptr<guiTypeFPSChartPlotter> ofxControlPanel::addFpsChartPlotter() {
    return addFpsChartPlotter( 15, 80 );
}

//-----------------------------------------------------
shared_ptr<guiTypeFPSChartPlotter> ofxControlPanel::addFpsChartPlotter(float minValY, float maxValY) {
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiTypeFPSChartPlotter> tt; return tt;
    }
    
    //add a new multi toggle to our list
    auto tmp = make_shared<guiTypeFPSChartPlotter>();
    //tmp->value.addValue(param);
    tmp->setup(minValY, maxValY);
    
    addGuiBaseObject(tmp, 0);
    return tmp;
}

//-----------------------------------------------------
shared_ptr<guiTypeLabel> ofxControlPanel::addLabel(string text, bool highlight, bool saveToXml){
    ofParameter <string> * tmp = new ofParameter<string>;
    
    (*tmp) = text;
    internalParams.push_back(tmp);

    return addLabel(*tmp, highlight, saveToXml);
}

//-----------------------------------------------------
shared_ptr<guiTypeLabel> ofxControlPanel::addLabel(ofParameter<string> & text, bool highlight, bool saveToXml){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() ) {
        shared_ptr<guiTypeLabel> tt; return tt;
    }

    //add a new multi toggle to our list
    auto tmp = make_shared<guiTypeLabel>();
    tmp->value.addValue(text);
    tmp->setup(text, highlight);

	addGuiBaseObject(tmp, (int)saveToXml);
    return tmp;
}


////---------------------------------------------
//weak_ptr<guiTypeLogger> ofxControlPanel::addLogger( string name, simpleLogger * logger, int drawW, int drawH ) {
//    if( currentPanel < 0 || currentPanel >= panels.size() ) {
//        weak_ptr<guiTypeLogger> tt; return tt;
//    }
//    auto loggerType = make_shared<guiTypeLogger>();
//
//    setLayoutFlag(loggerType);
//
//    loggerType->setup(name, logger, drawW, drawH);
//    panels[currentPanel]->addElement(loggerType);
//
//    guiObjects.push_back(loggerType);
//
//    if( bUseTTFFont ){
//        loggerType->setFont(&guiTTFFont);
//    }
//
//    return loggerType;
//}

////---------------------------------------------
//weak_ptr<guiTypeFileLister> ofxControlPanel::addFileLister(string name, simpleFileLister * lister, int drawW, int drawH){
//    if( currentPanel < 0 || currentPanel >= panels.size() ) {
//        weak_ptr<guiTypeFileLister> tt; return tt;
//    }
//    auto listerType = make_shared<guiTypeFileLister>();
//
//    setLayoutFlag(listerType);
//
//    listerType->setup(name, lister, drawW, drawH);
//    panels[currentPanel]->addElement(listerType);
//
//    guiObjects.push_back(listerType);
//
//    if( bUseTTFFont ){
//        listerType->setFont(&guiTTFFont);
//    }
//
//    return listerType;
//}

//---------------------------------------------
shared_ptr<guiTypeFileLister> ofxControlPanel::addFileLister( ofParameter<string>& aparam, string aDirectory ) {
    if( currentPanel < 0 || currentPanel >= panels.size() ) {
        shared_ptr<guiTypeFileLister> tt; return tt;
    }
    auto listerType = make_shared<guiTypeFileLister>();

    setLayoutFlag(listerType);
    listerType->value.addValue(aparam);
    listerType->setup( aparam, aDirectory, getDefaultColumnWidth(), 14 * 3 + 10 );
    panels[currentPanel]->addElement(listerType);

    guiObjects.push_back(listerType);

    if( bUseTTFFont ){
        listerType->setFont(&guiTTFFont);
    }

    return listerType;
}

//---------------------------------------------
shared_ptr<guiTypeFilePicker> ofxControlPanel::addFilePicker( ofParameter<string>& aparam ) {
    if( currentPanel < 0 || currentPanel >= panels.size() ) {
        shared_ptr<guiTypeFilePicker> tt; return tt;
    }
    
    auto fp = make_shared<guiTypeFilePicker>();
    fp->setup( aparam );
//    panels[currentPanel]->addElement(fp);
    
//    guiObjects.push_back(fp);
    addGuiBaseObject(fp, 1);
    
    return fp;
}

shared_ptr<guiTypeColorPicker> ofxControlPanel::addColorPicker( ofParameter<ofColor>& aparam ) {
    if( currentPanel < 0 || currentPanel >= panels.size() ) {
        shared_ptr<guiTypeColorPicker> tt; return tt;
    }
    
    auto cp = make_shared<guiTypeColorPicker>();
    cp->setup( aparam );
    addGuiBaseObject(cp, 1);
    return cp;
}

//---------------------------------------------
bool ofxControlPanel::newPanelSelected(){
	return bNewPanelSelected;
}

//---------------------------------------------
string ofxControlPanel::getSelectedPanelName(){
    if( selectedPanel < 0 || selectedPanel >= panels.size() )return "no panel";
	return panels[selectedPanel]->name;
}



// ############################################################## //
// ##
// ##       Settings to/from xml
// ##
// ############################################################## //

//-----------------------------
void ofxControlPanel::setIncrementalSave(string incrementalFileBaseName){
    incrementSaveName = incrementalFileBaseName;
    incrementSave = true;
}
//-----------------------------
void ofxControlPanel::disableIncrementalSave(){
    incrementSave = false;
}

//-----------------------------
bool ofxControlPanel::loadSettings(string xmlFile){
    ofLogNotice("ofxControlPanel :: loading settings from : ") << xmlFile;

    for(unsigned int i = 0; i < guiObjects.size(); i++) {
		guiObjects[i]->loadSettings(xmlFile);
    }

    currentXmlFile = xmlFile;

    settingsDirectory = currentXmlFile;

    int posLastSlash = settingsDirectory.rfind("/");
    if( posLastSlash > 0) settingsDirectory.erase(settingsDirectory.begin()+ posLastSlash+1, settingsDirectory.end()  );
    else settingsDirectory = "";

    bool bLoadOk = settings.loadFile(currentXmlFile);
    usingXml = true;

    for(unsigned int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj ) {
            int numParams = xmlObjects[i].numParams;

            for(int j = 0; j < numParams; j++){
                string str = xmlObjects[i].xmlName+":val_"+ofToString(j);
                
                string type = xmlObjects[i].guiObj->value.getTypeAsString();
                if( type == "ofcolor" ) {
                    xmlObjects[i].guiObj->value.paramGroup.getColor(j).fromString(settings.getValue(str, xmlObjects[i].guiObj->value.getValueAsString(j)));
                }
                else if( type == "string" ){
                    xmlObjects[i].guiObj->value.paramGroup.getString(j) = settings.getValue(str, xmlObjects[i].guiObj->value.getValueAsString(j));
                }else{
                    float val = settings.getValue(str, xmlObjects[i].guiObj->value.getValueF(j));
                    xmlObjects[i].guiObj->setValue(val, j);
                }
            }
            xmlObjects[i].guiObj->updateValue();
        }
    }
    
    ofNotifyEvent( guiLoadEvent, bLoadOk, this );
    return bLoadOk;
}

//-----------------------------
bool ofxControlPanel::reloadSettings() {
    if( currentXmlFile == "" ){
        currentXmlFile = "controlPanelSettings.xml";
    }
    return loadSettings(currentXmlFile);
}

//-----------------------------
bool ofxControlPanel::reloadSettingsForPanel( string name ){
    
//    guiTypePanel * panelPtr = NULL;
    shared_ptr<guiTypePanel> panelPtr;
    
    for(int i = 0; i < panels.size(); i++){
        if( panels[i]->name == name ){
            panelPtr = panels[i]; 
            break;
        }
    }
    
    if( currentXmlFile == "" ){
        currentXmlFile = "controlPanelSettings.xml";
    }
    
//    if( panelPtr == NULL ){
    if( !panelPtr ){
        ofLogError("ofxControlPanel::reloadSettingsForPanel - couldn't find panel ") << name;
        return false;
    } 
    
    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj ){
            if( panelPtr->containsElement( xmlObjects[i].guiObj ) ){
                int numParams = xmlObjects[i].numParams;
                //                printf("loading setting for xmlObject %s\n", xmlObjects[i].xmlName.c_str() );

                for(int j = 0; j < numParams; j++){
                    string str = xmlObjects[i].xmlName+":val_"+ofToString(j);

                    string type = xmlObjects[i].guiObj->value.getTypeAsString();
                    if( type == "ofcolor" ) {
                        xmlObjects[i].guiObj->value.paramGroup.getColor(j).fromString(settings.getValue(str, xmlObjects[i].guiObj->value.getValueAsString(j)));
                    }
                    else if( type == "string" ){
                        xmlObjects[i].guiObj->value.paramGroup.getString(j) = settings.getValue(str, xmlObjects[i].guiObj->value.getValueAsString(j));
                    }else{
                        float val = settings.getValue(str, xmlObjects[i].guiObj->value.getValueF(j));
                        xmlObjects[i].guiObj->setValue(val, j);
                    }
                }
                xmlObjects[i].guiObj->updateValue();
            }

        }
    }
    
    for(unsigned int i = 0; i < panelPtr->children.size(); i++){
        if(panelPtr->children[i] != NULL ) {
            panelPtr->children[i]->loadSettings(currentXmlFile);
        }
	}
    return true;
}

//-------------------------------
bool ofxControlPanel::saveSettings(string xmlFile,  bool bUpdateXmlFile){
    ofLogNotice("ofxControlPanel :: saving settings to ") << xmlFile;

    for(int i = 0; i < (int) guiObjects.size(); i++)guiObjects[i]->saveSettings(xmlFile);

    for(int i = 0; i < (int) xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj ){
            int numParams = xmlObjects[i].numParams;

            for(int j = 0; j < numParams; j++){
                string str = xmlObjects[i].xmlName+":val_"+ofToString(j);
                settings.setValue(str, xmlObjects[i].guiObj->value.getValueAsString(j));
            }
        }
    }

    if( incrementSave ){
        string xmlName = incrementSaveName;
        xmlName += ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
        xmlName += ".xml";
        xmlName = settingsDirectory + xmlName;
        settings.saveFile(xmlName);
    }
	//printf("saving %s\n", xmlFile.c_str());
    bool bLoadOk = settings.saveFile(xmlFile);
	if( bUpdateXmlFile ){
		currentXmlFile = xmlFile;
	}
    usingXml = true;
    ofNotifyEvent( guiSaveEvent, bLoadOk, this );
    return bLoadOk;
}

//-----------------------------
bool ofxControlPanel::saveSettings(){
    if(currentXmlFile == "") {
        currentXmlFile = "controlPanelSettings.xml";
    }
    return saveSettings(currentXmlFile, true);
//    usingXml = true;
}

//-------------------------------
void ofxControlPanel::setXMLFilename(string xmlFile){
    currentXmlFile = xmlFile;
}

//-------------------------------
bool ofxControlPanel::isSaveButtonPressed() {
    return saveDown;
}

// ############################################################## //
// ##
// ##       Visibilty
// ##
// ############################################################## //


//---------------------------
void ofxControlPanel::setMinimized(bool bMinimize){
    if( bMinimize ) {
        if( !minimize ) {
            for( int i = 0; i < panels.size(); i++ ) {
                panels[i]->lostFocus();
            }
        }
    }
    
    minimize = bMinimize;
}

//---------------------------
bool ofxControlPanel::isMinimized() {
    return minimize;
}

//---------------------------
bool ofxControlPanel::toggleMinimized() {
    setMinimized( !minimize );
    return minimize;
    //return minimize = !minimize;
}

//---------------------------
void ofxControlPanel::setDraggable(bool bDrag){
    bDraggable = bDrag;
}

//-------------------------------
bool ofxControlPanel::mouseIsInteracting(){
	return hitSomething;
}

//-------------------------------
void ofxControlPanel::show(){
    hidden = false;
}

//-------------------------------
void ofxControlPanel::hide(){
    if( !hidden ) {
        for( int i = 0; i < panels.size(); i++ ) {
            panels[i]->lostFocus();
        }
    }
    hidden = true;
}

//-------------------------------
void ofxControlPanel::toggleView(){
    if( hidden ) {
        show();
    } else {
        hide();
    }
    //hidden = !hidden;
}

//-------------------------------
void ofxControlPanel::setInvisibleMode(bool bInvis){
	bInvisible = bInvis;
}

//-------------------------------
bool ofxControlPanel::isHidden(){
	return hidden;
}

//-------------------------------
bool ofxControlPanel::isVisible(){
	return !hidden;
}

//-------------------------------
void ofxControlPanel::setStatusMessage(ofParameter <string> & message, int whichPanel){
	if( whichPanel == -1 ){
        for(int i = 0; i < panels.size(); i++){
            panels[i]->enableStatusBar();
            panels[i]->setStatus(message);
        }
    }else{
        if(whichPanel >= 0 && whichPanel < panels.size()){
            panels[whichPanel]->enableStatusBar();
            panels[whichPanel]->setStatus(message);
        }
    }
}

// ############################################################## //
// ##
// ##       Mouse Events
// ##
// ############################################################## //

#pragma mark - Mouse Events
//-------------------------------
bool ofxControlPanel::mousePressed(float x, float y, int button){
    if( hidden ) return false;

	hitSomething	  =	false;
    bool tabButtonPressed = false;
	
	ofRectangle checkRect = boundingBox;
	if( minimize ){
		checkRect.height = topBar.height;
	}	

	//we do this so people can check if mouse is interacting with panel
	if( isInsideRect(x, y, checkRect) ){
		hitSomething = true;
	}

    if( isInsideRect(x, y, minimizeButton)){
//        minimize = !minimize;
        toggleMinimized();
    }else if( usingXml && isInsideRect(x, y, saveButton) ){
        saveSettings();
        saveDown = true;
//        if( eventsEnabled && bEventsSetup ) {
//            guiCallbackData saveData;
//            saveData.setup("CP_SAVE", "CP_SAVE");
//            saveData.addValueS( currentXmlFile );
//            ofNotifyEvent( guiEvent, saveData, this );
//        }
//		printf("saving settings!\n");
    }else if( usingXml && isInsideRect(x, y, saveAsButton) ){
		
        string newFilePath = ofSystemSaveDialog("settingsBackup-"+ofGetTimestampString()+".xml", "set xml file to save settings to eg: mySettingsCopy.xml").filePath;
        string curSettingsFileBackup = currentXmlFile;
        
        if( newFilePath.length() > 0 ){
            if( ofFile::doesFileExist(newFilePath) ){
                ofSystemAlertDialog("ERROR: can't do 'save as', xml file already exists");
                newFilePath = curSettingsFileBackup;
            }
        }else{
            newFilePath = curSettingsFileBackup;
        }
        
        saveSettings(newFilePath, false);
        
        saveAsDown = true;
//        if( eventsEnabled && bEventsSetup ) {
//            guiCallbackData saveData;
//            saveData.setup("CP_SAVE", "CP_SAVE");
//            saveData.addValueS( newFilePath );
//            ofNotifyEvent( guiEvent, saveData, this );
//        }
//		printf("saving settings!\n");
    }else if( usingXml && isInsideRect(x, y, restoreButton) ){
		printf("restoring settings!\n");
        reloadSettings();
        restoreDown = true;
    }else if( usingXml && isInsideRect(x, y, loadButton) ){
        
        string loadPath = ofSystemLoadDialog("select an xml settings file", false, ofFilePath::getAbsolutePath( ofToDataPath("settings") ) ).filePath;
        
//        cout << "loading settings from " << loadPath << endl;
        
        if( ofFilePath::getFileExt(loadPath) == "xml" ){
            string backupFile = currentXmlFile;
            loadSettings(loadPath);
            currentXmlFile = backupFile;
        }
        
        loadDown = true;
    }else if( isInsideRect(x, y, topBar) && bDraggable){
        dragging = true;
        mouseDownPoint.set(x - boundingBox.x, y-boundingBox.y, 0);
    }else if(!minimize){
		//int lastSelectedPanel = selectedPanel;
        for(int i = 0; i < (int) panels.size(); i++){
            if( isInsideRect(x, y, panelTabs[i]) ){
                setSelectedPanel(i);
//                selectedPanel = i;
//				if( lastSelectedPanel != selectedPanel ){
//					bNewPanelSelected = true;
//				}else{
//					bNewPanelSelected = false;
//				}
                tabButtonPressed = true;
                break;
            }
        }
    }

    if(minimize == false && tabButtonPressed == false && isInsideRect(x, y, boundingBox) ){
        for(int i = 0; i < (int) panels.size(); i++){
             if( i == selectedPanel ){
				elementSelected = panels[i]->checkHit( x - hitArea.x, y - hitArea.y, button);
			 }
		}
    }

    prevMouse.set(x, y);

	return hitSomething;
}


//-------------------------------
bool ofxControlPanel::mouseDragged(float x, float y, int button){
    if( hidden ) return false;
	
	//we do this so people can check if mouse is interacting with panel
	bool isDragging = dragging;
		
	if( !isDragging ){
		ofRectangle checkRect = boundingBox;
		if( minimize ){
			checkRect.height = topBar.height;
		}	
		if( isInsideRect(x, y, checkRect) ){
			isDragging = true;
		}
	}

    if(dragging)setPosition( MAX(0, x - mouseDownPoint.x), MAX(0, y -mouseDownPoint.y));
    else if(!minimize){
        for(int i = 0; i < (int) panels.size(); i++){
            if( i == selectedPanel ){

                if(button){
                    panels[i]->updateGui( x - prevMouse.x, y - prevMouse.y, false, true);
                }else{
                    panels[i]->updateGui( x - hitArea.x, y - hitArea.y, false, false);
                }
            }
        }
    }

    prevMouse.set(x, y);
	return isDragging;
}

//-------------------------------
void ofxControlPanel::mouseReleased(float x, float y, int button){
	elementSelected = false;
	hitSomething	= false;

    if( hidden ) return;

    for(int i = 0; i < (int) panels.size(); i++){
        if( i == selectedPanel ){
        
            if(button){
                panels[i]->release( x - prevMouse.x, y - prevMouse.y, true);
            }else{
                panels[i]->release( x - hitArea.x, y - hitArea.y, false);
            }
    
            panels[i]->resetSelectedElement();
        }
    }
    dragging        = false;
    saveDown        = false;
    saveAsDown      = false;
    restoreDown     = false;
    loadDown        = false;
}

//-------------------------------
void ofxControlPanel::mouseMoved(float x, float y ) {
    if( hidden ) return;
    
	hitSomething	  =	false;
    bool tabButtonPressed = false;
	
	ofRectangle checkRect = boundingBox;
	if( minimize ){
		checkRect.height = topBar.height;
	}
    
	//we do this so people can check if mouse is interacting with panel
	if( isInsideRect(x, y, checkRect) ){
		hitSomething = true;
	}
    
    
    if(minimize == false && tabButtonPressed == false && isInsideRect(x, y, boundingBox) ){
        for(int i = 0; i < (int) panels.size(); i++){
            if( i == selectedPanel ){
//				elementSelected = panels[i]->checkHit( x - hitArea.x, y - hitArea.y );
                panels[i]->mouseMoved( x - hitArea.x, y - hitArea.y, false );
            }
		}
    }
    
//    prevMouse.set(x, y);
    
//	return hitSomething;
}

//-------------------------------
bool ofxControlPanel::keyPressed(int k){

	if ( hidden ) return false;
	if ( minimize ) return false;
    
    for( int i=0; i<(int)panels.size(); i++ ){
        if( i == selectedPanel ){
            bool eaten = panels[i]->keyPressed( k );
            if ( eaten ){
                return true;
            }
        }
    }
    
    if( k == OF_KEY_RIGHT ){
        setSelectedPanel(getSelectedPanel() + 1);
        return true;
    }

    if( k == OF_KEY_LEFT ){
        setSelectedPanel(getSelectedPanel() - 1);
        return true;
    }
    
//	for( int i=0; i<(int)panels.size(); i++ ){
//        if( i == selectedPanel ){
//            bool eaten = panels[i]->keyPressed( k );
//            if ( eaten ){
//                return true;
//            }
//        }
//	}
	return false;
	
}

//-------------------------------
bool ofxControlPanel::keyReleased(int k){
	if ( hidden ) return false;
	if ( minimize ) return false;
    
    if( k == OF_KEY_RIGHT || k == OF_KEY_LEFT ){
        return true;
    }
    
	for( int i=0; i<(int)panels.size(); i++ ){
        if( i == selectedPanel ){
            bool eaten = panels[i]->keyReleased( k );
            if ( eaten ){
                return true;
            }

        }
	}
    return false;
}

// ############################################################## //
// ##
// ##       Updater
// ##
// ############################################################## //

//-------------------------------
void ofxControlPanel::update(){
    guiBaseObject::update();
	
	if( panels.size() == 0 ) return; 
	
	displayText.setText(getSelectedPanelName());

    minimizeButton   = ofRectangle(boundingBox.x + boundingBox.width - 24, boundingBox.y + 4, 20, 10 );
	if ( !bDoSaveRestore )
	{
		saveButton		= ofRectangle( boundingBox.x, boundingBox.y, 0, 0 );
		restoreButton	= ofRectangle( boundingBox.x, boundingBox.y, 0, 0 );
        saveAsButton    = saveButton;
        loadButton      = restoreButton;
	}
	else
	{
		saveButton       = ofRectangle(boundingBox.x + displayText.getTextWidth() + 20, boundingBox.y + 4, MAX(40, 8 + displayText.getTextWidth("save")) , MAX(12, displayText.getTextSingleLineHeight()) );
        
		saveAsButton       = ofRectangle(saveButton.x + saveButton.width + 15, boundingBox.y + 4, MAX(40, 8 + displayText.getTextWidth("save as")) , MAX(12, displayText.getTextSingleLineHeight()) );
		
        restoreButton    = ofRectangle(saveAsButton.x + saveAsButton.width + 50, boundingBox.y + 4,  MAX(60, 8 + displayText.getTextWidth("restore")) , MAX(12, displayText.getTextSingleLineHeight()) );

        loadButton    = ofRectangle(restoreButton.x + restoreButton.width + 15, boundingBox.y + 4,  MAX(60, 8 + displayText.getTextWidth("load from")) , MAX(12, displayText.getTextSingleLineHeight()) );

	}


    for(int i = 0; i < (int) panels.size(); i++){
        panels[i]->update();

        panelTabs[i].x      = i * tabWidth + hitArea.x + borderWidth;
        panelTabs[i].y      = hitArea.y-tabHeight;
        panelTabs[i].width  = tabWidth;
        panelTabs[i].height = tabHeight;

    }
}

//---------------------------------------------
void ofxControlPanel::updateBoundingBox(){
    topBar           = ofRectangle(boundingBox.x, boundingBox.y, boundingBox.width, MAX(20, displayText.getTextSingleLineHeight() * 1.2 ) );
	ofxControlPanel::topSpacing = MAX(20, topBar.height);
    hitArea.y = boundingBox.y + topSpacing*2.0;
    
//    if(bShowText){
//        //we need to update out hitArea because the text will have moved the gui down
//        hitArea.y = boundingBox.y + topSpacing;
//        boundingBox.height = hitArea.height + displayText.getTextHeight() + titleSpacing;
//        boundingBox.width  = MAX( hitArea.width, displayText.getTextWidth() );
//    }else{
//        hitArea.y = boundingBox.y;
//        boundingBox.height = hitArea.height;
//        boundingBox.width  = hitArea.width;
//    }
}


// ############################################################## //
// ##
// ##       Drawing
// ##
// ############################################################## //

//-------------------------------
void ofxControlPanel::draw(){
    if( hidden ) return;

	lastFrameDrawn	= ofGetFrameNum();

    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, 255);
    ofFill();

        float panelH = boundingBox.height;
        if( minimize ){
            panelH = topBar.height;
        }

		if(!bInvisible || !elementSelected){

			ofPushMatrix();
				ofTranslate(boundingBox.x, boundingBox.y, 0);
				//draw the background
				ofFill();
				ofSetColor(bgColor.getColor());

				ofDrawRectangle(0, 0, boundingBox.width, panelH);

				//draw the outline
				ofNoFill();
				ofSetColor(outlineColor.getColor());
				ofDrawRectangle(0, 0, boundingBox.width, panelH);
				ofDrawLine(0, topBar.height, boundingBox.width, topBar.height);
			ofPopMatrix();

			ofDrawRectangle(minimizeButton.x, minimizeButton.y, minimizeButton.width, minimizeButton.height);

			if ( bDoSaveRestore ){
				ofPushStyle();
					ofFill();

					if( saveDown )ofSetColor(fgColor.getSelectedColor());
					else ofSetColor(fgColor.getColor());

					ofDrawRectangle(saveButton);
					ofSetColor(255, 255, 255);
				
					displayText.renderString("save", saveButton.x + 3, saveButton.y + saveButton.height -3);


					ofFill();

					if( saveAsDown )ofSetColor(fgColor.getSelectedColor());
					else ofSetColor(fgColor.getColor());

					ofDrawRectangle(saveAsButton);
					ofSetColor(255, 255, 255);
				
					displayText.renderString("save as", saveAsButton.x + 3, saveAsButton.y + saveAsButton.height -3);


					ofFill();

					if( restoreDown )ofSetColor(fgColor.getSelectedColor());
					else ofSetColor(fgColor.getColor());

					ofDrawRectangle(restoreButton);
					ofSetColor(255, 255, 255);
				
					displayText.renderString("restore", restoreButton.x + 3, restoreButton.y + restoreButton.height -3);
				

					ofFill();

					if( loadDown )ofSetColor(fgColor.getSelectedColor());
					else ofSetColor(fgColor.getColor());

					ofDrawRectangle(loadButton);
					ofSetColor(255, 255, 255);
				
					displayText.renderString("load from", loadButton.x + 3, loadButton.y + loadButton.height -3);
				

				
				ofPopStyle();
			}


			ofPushMatrix();
				ofTranslate(2,0,0);
				ofSetColor(textColor.getColor());
				guiBaseObject::renderText();
			ofPopMatrix();
		
		}

        if( !minimize ){
                    
            ofPushMatrix();
                ofTranslate(hitArea.x, hitArea.y, 0);
                for(int i = 0; i < (int) panels.size(); i++){
                    if( i == selectedPanel ){
                        panels[i]->setShowOnlySelectedElement(bInvisible);
                        panels[i]->render();
                    }
                }
            ofPopMatrix();

        
            if(!bInvisible || !elementSelected){
                for(int i = 0; i < (int) panelTabs.size(); i++){
                    if( i == selectedPanel){
                        ofPushStyle();
                            ofFill();
                            ofSetColor(fgColor.getSelectedColor());
                            ofDrawRectangle(panelTabs[i].x, panelTabs[i].y, panelTabs[i].width, panelTabs[i].height);
                            ofSetColor(outlineColor.getColor());
                        ofPopStyle();
                    }
                    ofSetColor(outlineColor.getColor());
                    ofNoFill();
                    ofDrawRectangle(panelTabs[i].x, panelTabs[i].y, panelTabs[i].width, panelTabs[i].height);
                }
            }
        
            
            ofPushMatrix();
                ofTranslate(hitArea.x, hitArea.y, 0);
                for(int i = 0; i < (int) panels.size(); i++){
                    if( i == selectedPanel ){
                        panels[i]->renderStatus();
                    }
                }
            ofPopMatrix();
        }

    ofPopStyle();
	
}



