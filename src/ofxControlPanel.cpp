#include "ofxControlPanel.h"
#include "ofEvents.h"

float ofxControlPanel::borderWidth = 0;
float ofxControlPanel::topSpacing  = 15;
float ofxControlPanel::tabWidth = 35;
float ofxControlPanel::tabHeight = 10;

vector <ofxControlPanel *> ofxControlPanel::globalPanelList;

//----------------------------
bool ofxControlPanel::shouldRemoveGuiBaseObject( const guiBaseObject* aObj ) {
    return (aObj == NULL);
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
    restoreDown     = false;
    incrementSave   = false;
    hidden          = false;
    bDraggable      = true;
	eventsEnabled	= false;
	bEventsSetup	= false;
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
    ofLogError() << " ofxControlPanel::~ofxControlPanel " << endl; 

    xmlObjects.clear();
    
	for(int i = 0; i < customEvents.size(); i++){
		if( customEvents[i] != NULL ){
            ofRemoveListener(guiObjects[i]->guiEvent, this, &ofxControlPanel::eventsIn);
			delete customEvents[i];
			customEvents[i] = NULL;
		}
	}
	customEvents.clear();

    for(unsigned int i = 0; i < guiObjects.size(); i++){
        if( guiObjects[i] != NULL ){
			delete guiObjects[i];
			guiObjects[i] = NULL;
		}
    }
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
    guiTTFFont.loadFont(fontName, fontsize);
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
    printf("ofxControlPanel::setSize\n");
    
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
guiTypePanel * ofxControlPanel::addPanel(string panelName, int numColumns, bool locked){
    guiTypePanel * panelPtr = new guiTypePanel();
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
                
                delete panels[i]->children[j];
                panels[i]->children[j] = NULL;
            }
            panels[i]->children.clear();
            
            panelTabs.erase( panelTabs.begin()+i );
            
            for(int j = 0; j < guiObjects.size(); j++ ) {
                if(guiObjects[j] != NULL) {
                    if(guiObjects[j] == panels[i] ) {
//                        delete  guiObjects[j];
                        guiObjects[j] = NULL;
                        guiObjects.erase( guiObjects.begin()+j );
                        break;
                    }
                }
            }
            
            if( currentPanel == i ) {
                setWhichPanel( panels.size()-1 );
            }
            
            
            delete panels[i];
            panels[i] = NULL;
            
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
		selectedPanel = whichPanel;
	}
}

// ############################################################## //
// ##
// ##       Gui elements
// ##
// ############################################################## //

//---------------------------------------------
void ofxControlPanel::addXmlAssociation( guiBaseObject* object, string xmlName, int paramCount ){
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
guiBaseObject * ofxControlPanel::addGuiBaseObject( guiBaseObject * element,  int varsToAddToXml){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;

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
void ofxControlPanel::addGroup( ofParameterGroup & group ){

    addLabel(group.getName(), true);

    for(int i = 0; i < group.size(); i++){
    
        ofAbstractParameter * ptr = &group.get(i);

        if( ptr->type() == typeid(ofParameter<float>).name() ){
            ofParameter <float> & f = dynamic_cast< ofParameter<float> & >( group.get(i) );
            guiTypeSlider * ptr = addSlider(f);
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
    }

}

//---------------------------------------------
guiTypeToggle * ofxControlPanel::addToggle(string name, bool defaultValue){
    ofParameter <bool> * tmp = new ofParameter<bool>;
    tmp->set(name, defaultValue, 0, 1);
    
    internalParams.push_back(tmp);
    return addToggle(*tmp);
}

//---------------------------------------------
guiTypeToggle * ofxControlPanel::addToggle(ofParameter <bool> & param){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;

    guiTypeToggle * tmp = new guiTypeToggle();
    tmp->value.addValue(param);
    tmp->setup();
    
	addGuiBaseObject(tmp, 1);
    return tmp;
}

//---------------------------------------------
guiTypeMultiToggle * ofxControlPanel::addMultiToggle(string name, int defaultBox, vector <string> boxNames){
    ofParameter <int> * tmp = new ofParameter<int>;
    tmp->set(name, defaultBox, 0, boxNames.size());
    
    internalParams.push_back(tmp);
    return addMultiToggle(*tmp, boxNames);

}

//---------------------------------------------
guiTypeMultiToggle * ofxControlPanel::addMultiToggle(ofParameter<int> &param, vector <string> boxNames){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;
    
    if( param < 0 ){
        param = 0;
    }else if( param >= boxNames.size() ){
        param = boxNames.size()-1;
    }

    //add a new multi toggle to our list
    guiTypeMultiToggle * tmp = new guiTypeMultiToggle();
    tmp->value.addValue(param);
    tmp->setup(boxNames);

	addGuiBaseObject(tmp, 1);
    return tmp;
}

//---------------------------------------------
guiTypeTextDropDown * ofxControlPanel::addTextDropDown(string name, int defaultBox, vector <string> boxNames){
    ofParameter <int> * tmp = new ofParameter<int>;
    tmp->set(name, defaultBox, 0, boxNames.size());
    
    internalParams.push_back(tmp);
    return addTextDropDown(*tmp, boxNames);
}

//---------------------------------------------
guiTypeTextDropDown * ofxControlPanel::addTextDropDown(ofParameter<int> & param, vector <string> boxNames){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;

	xmlName = checkXmlName(name, xmlName);

    //add a new multi toggle to our list
    guiTypeTextDropDown * tmp = new guiTypeTextDropDown();
    tmp->value.addValue(param);
    tmp->setup(boxNames);

	addGuiBaseObject(tmp, 1);
    return tmp;
}


//---------------------------------------------
guiTypeSlider * ofxControlPanel::addSlider(string name, float value , float min, float max){
    
    ofParameter <float> * tmp = new ofParameter<float>;
    tmp->set(name, value, min, max);
    
    internalParams.push_back(tmp);    
    return addSlider(*tmp);
}

//---------------------------------------------
guiTypeSlider * ofxControlPanel::addSliderInt(string name, int value, int min, int max){
    
    ofParameter <int> * tmp = new ofParameter<int>;
    tmp->set(name, value, min, max);
    
    internalParams.push_back(tmp);    
    return addSlider(*tmp);
}


//---------------------------------------------
guiTypeSlider * ofxControlPanel::addSlider(ofParameter <int> & param){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;

    //add a new slider to our list
    guiTypeSlider * tmp = new guiTypeSlider();
    
    tmp->value.addValue(param);
    tmp->setup();
	
	addGuiBaseObject(tmp, 1);
    return tmp;
}

//---------------------------------------------
guiTypeSlider * ofxControlPanel::addSlider(ofParameter <float> & param){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;

    //add a new slider to our list
    guiTypeSlider * tmp = new guiTypeSlider();
    
    tmp->value.addValue(param);
    tmp->setup();
	
	addGuiBaseObject(tmp, 1);
    return tmp;
}


//-------------------------------
guiType2DSlider * ofxControlPanel::addSlider2D(string name, float valueX, float valueY, float minX, float maxX, float minY, float maxY){
    ofParameter <float> * tmp1 = new ofParameter<float>;
    ofParameter <float> * tmp2 = new ofParameter<float>;

    tmp1->set(name, valueX, minX, maxX);
    tmp1->set(name, valueY, minY, maxY);
    
    internalParams.push_back(tmp1);
    internalParams.push_back(tmp2);
    
    return addSlider2D(*tmp1, *tmp2);
}

//-------------------------------
guiType2DSlider * ofxControlPanel::addSlider2D(ofParameter<int> & param1, ofParameter<int> & param2){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;
	
    //add a new slider to our list
    guiType2DSlider * tmp = new guiType2DSlider();
    tmp->value.addValue(param1);
    tmp->value.addValue(param2);
    tmp->setup();

	addGuiBaseObject(tmp, 2);
    return tmp;
}

//-------------------------------
guiType2DSlider * ofxControlPanel::addSlider2D(ofParameter<float> & param1, ofParameter<float> & param2){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;
	
    //add a new slider to our list
    guiType2DSlider * tmp = new guiType2DSlider();
    tmp->value.addValue(param1);
    tmp->value.addValue(param2);
    tmp->setup();

	addGuiBaseObject(tmp, 2);
    return tmp;
}


//---------------------------------------------
guiTypeDrawable * ofxControlPanel::addDrawableRect(string name, ofBaseDraws * drawablePtr, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;

    guiTypeDrawable * tmp = new guiTypeDrawable();
    tmp->setup(name, drawablePtr, drawW, drawH);

	addGuiBaseObject(tmp, 0);
    return tmp;
}

//---------------------------------------------
guiTypeVideo * ofxControlPanel::addVideoRect(string name, ofVideoPlayer * drawablePtr, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
 
	guiTypeVideo * tmp = new guiTypeVideo();
    tmp->setup(name, drawablePtr, drawW, drawH);

	addGuiBaseObject(tmp, 0);
    return tmp;
}


//---------------------------------------------
guiTypeCustom * ofxControlPanel::addCustomRect(string name, guiCustomImpl * customPtr, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;

    guiTypeCustom * tmp = new guiTypeCustom();
    tmp->setup(name, customPtr, drawW, drawH);

	addGuiBaseObject(tmp, 0);
    return tmp;
}


//---------------------------------------------
guiTypeVairableLister * ofxControlPanel::addVariableLister(ofParameterGroup & group){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;

    //add a new multi toggle to our list
    guiTypeVairableLister * tmp = new guiTypeVairableLister();
    tmp->value.paramGroup = group;
    tmp->setup();
	
	addGuiBaseObject(tmp, 0);
    return tmp;
}

//-----------------------------------------------------
guiTypeChartPlotter * ofxControlPanel::addChartPlotter(ofParameter <float> & param, float minValY, float maxValY){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;

    //add a new multi toggle to our list
    guiTypeChartPlotter * tmp = new guiTypeChartPlotter();
    tmp->value.addValue(param); 
    tmp->setup(minValY, maxValY);

	addGuiBaseObject(tmp, 0);
    return tmp;
}

//-----------------------------------------------------
guiTypeLabel * ofxControlPanel::addLabel(string text, bool highlight, bool saveToXml){
    ofParameter <string> * tmp = new ofParameter<string>;
    
    (*tmp) = text;
    internalParams.push_back(tmp);

    return addLabel(*tmp, highlight, saveToXml);
}

//-----------------------------------------------------
guiTypeLabel * ofxControlPanel::addLabel(ofParameter<string> & text, bool highlight, bool saveToXml){
    if( currentPanel < 0 || currentPanel >= (int) panels.size() )return NULL;

    //add a new multi toggle to our list
    guiTypeLabel * tmp = new guiTypeLabel();
    tmp->value.addValue(text);
    tmp->setup(text, highlight);

	addGuiBaseObject(tmp, (int)saveToXml);
    return tmp;
}


//---------------------------------------------
guiTypeLogger * ofxControlPanel::addLogger(string name, simpleLogger * logger, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
    guiTypeLogger * loggerType = new guiTypeLogger();

        setLayoutFlag(loggerType);

    loggerType->setup(name, logger, drawW, drawH);
    panels[currentPanel]->addElement(loggerType);

    guiObjects.push_back(loggerType);

    if( bUseTTFFont ){
        loggerType->setFont(&guiTTFFont);
    }

    return loggerType;
}

//---------------------------------------------
guiTypeFileLister * ofxControlPanel::addFileLister(string name, simpleFileLister * lister, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
    guiTypeFileLister * listerType = new guiTypeFileLister();

        setLayoutFlag(listerType);

    listerType->setup(name, lister, drawW, drawH);
    panels[currentPanel]->addElement(listerType);

    guiObjects.push_back(listerType);

    if( bUseTTFFont ){
        listerType->setFont(&guiTTFFont);
    }

    return listerType;
}


// ############################################################## //
// ##
// ##       events 
// ##
// ############################################################## //

//THIS SHOULD BE CALLED AFTER ALL GUI SETUP CALLS HAVE HAPPENED
//---------------------------------------------
void ofxControlPanel::setupEvents(){
	eventsEnabled = true;
	for(int i = 0; i < guiObjects.size(); i++){
		ofAddListener(guiObjects[i]->guiEvent, this, &ofxControlPanel::eventsIn);
	}
	
	//setup an event group for each panel
	for(int i = 0; i < panels.size(); i++){
	
		vector <string> xmlNames;
		
		for(int j = 0; j < panels[i]->children.size(); j++){
			xmlNames.push_back(panels[i]->children[j]->xmlName);
		}
		
		string groupName = "PANEL_EVENT_"+ofToString(i);
		createEventGroup(groupName, xmlNames);
		//printf("creating %s\n", groupName.c_str());
	}
	
	bEventsSetup = true;
}

// Create a single common event which fired whenever any of the gui elements represented by xmlNames is changed
//---------------------------------------------
ofEvent <guiCallbackData> & ofxControlPanel::createEventGroup(string eventGroupName, vector <string> xmlNames){
	customEvents.push_back( new guiCustomEvent() );
	customEvents.back()->group = eventGroupName;
	customEvents.back()->names = xmlNames;
	return customEvents.back()->guiEvent;	
}		

//this takes a single GUI name and makes it an event group - ie if you wanted to listen to just one slider or button
//---------------------------------------------
ofEvent <guiCallbackData> & ofxControlPanel::createEventGroup(string xmlName){
	vector <string> xmlNames;
	xmlNames.push_back(xmlName);
	customEvents.push_back( new guiCustomEvent() );
	customEvents.back()->group = xmlName;
	customEvents.back()->names = xmlNames;
	return customEvents.back()->guiEvent;
}		

//---------------------------------------------
void ofxControlPanel::enableEvents(){
	if( !bEventsSetup ){
		setupEvents();
	}
	eventsEnabled = true;
}

//---------------------------------------------
void ofxControlPanel::disableEvents(){
	eventsEnabled = false;
}

// Get an event object for just a panel
//---------------------------------------------
ofEvent <guiCallbackData> & ofxControlPanel::getEventsForPanel(int panelNo){
	if( panelNo < panels.size() ){
		return getEventGroup("PANEL_EVENT_"+ofToString(panelNo));
	}else{			
		return guiEvent;
	}
}

//---------------------------------------------
ofEvent <guiCallbackData> & ofxControlPanel::getAllEvents(){
	return guiEvent;
 } 


// Use the name you made for your custom group to get back the event object
//---------------------------------------------
ofEvent <guiCallbackData> & ofxControlPanel::getEventGroup(string eventGroupName){
	for(int i = 0; i < customEvents.size(); i++){
		if( eventGroupName == customEvents[i]->group ){
			return customEvents[i]->guiEvent;
		}
	}
	
	//if we don't find a match we return the global event
	ofLog(OF_LOG_ERROR, "error eventGroup %s does not exist - returning the global event instead", eventGroupName.c_str());
	return guiEvent;
}

//This is protected
//---------------------------------------------
void ofxControlPanel::eventsIn(guiCallbackData & data){

	if( !eventsEnabled ) return;
	
	//we notify the ofxControlPanel event object - aka the global ALL events callback
	ofNotifyEvent(guiEvent, data, this);
	
	//we then check custom event groups
	for(int i = 0; i < customEvents.size(); i++){
		for(int k = 0; k < customEvents[i]->names.size(); k++){
			if( customEvents[i]->names[k] == data.getXmlName() ){
				ofNotifyEvent(customEvents[i]->guiEvent, data, this);
			}
		}
	}
}
		
// ############################################################## //
// ##
// ##       get and set values
// ##
// ############################################################## //

//---------------------------------------------
void ofxControlPanel::setValueB(string xmlName, bool value,  int whichParam){
    for(int i = 0; i < (int) guiObjects.size(); i++){
        if( guiObjects[i]->xmlName == xmlName){
            if( whichParam >= 0  ){
                guiObjects[i]->value.setValue(value, whichParam);
                return;
            }
        }
    }
}

//---------------------------------------------
void ofxControlPanel::setValueI(string xmlName, int value,  int whichParam){
    for(int i = 0; i < (int) guiObjects.size(); i++){
        if( guiObjects[i]->xmlName == xmlName){
            if( whichParam >= 0  ){
                guiObjects[i]->value.setValue(value, whichParam);
                return;
            }
        }
    }
}

//---------------------------------------------
void ofxControlPanel::setValueF(string xmlName, float value,  int whichParam){
    for(int i = 0; i < (int) guiObjects.size(); i++){
        if( guiObjects[i]->xmlName == xmlName){
            if( whichParam >= 0  ){
                guiObjects[i]->value.setValue(value, whichParam);
                return;
            }
        }
    }
}


//---------------------------------------------
bool ofxControlPanel::getValueB(string xmlName, int whichParam){
    for(int i = 0; i < (int) xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL && xmlName == xmlObjects[i].xmlName ){
            if( whichParam >= 0 && whichParam < xmlObjects[i].numParams ){
                return xmlObjects[i].guiObj->value.getValueB(whichParam);
            }
        }
    }
    ofLog(OF_LOG_WARNING, "ofxControlPanel - parameter requested %s doesn't exist - returning 0", xmlName.c_str());
    return 0;
}

//---------------------------------------------
float ofxControlPanel::getValueF(string xmlName, int whichParam){
    for(int i = 0; i < (int) xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL && xmlName == xmlObjects[i].xmlName ){
            if( whichParam >= 0 && whichParam < xmlObjects[i].numParams ){
                return xmlObjects[i].guiObj->value.getValueF(whichParam);
            }
        }
    }
    ofLog(OF_LOG_WARNING, "ofxControlPanel - parameter requested %s doesn't exist - returning 0", xmlName.c_str());
    return 0;
}

//---------------------------------------------
int ofxControlPanel::getValueI(string xmlName, int whichParam){
    for(int i = 0; i < (int) xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL && xmlName == xmlObjects[i].xmlName ){
            if( whichParam >= 0 && whichParam < xmlObjects[i].numParams ){
                return xmlObjects[i].guiObj->value.getValueI(whichParam);
            }
        }
    }
    ofLog(OF_LOG_WARNING, "ofxControlPanel - parameter requested %s doesn't exist - returning 0", xmlName.c_str());
    return 0;
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
void ofxControlPanel::loadSettings(string xmlFile){
    for(unsigned int i = 0; i < guiObjects.size(); i++)
		guiObjects[i]->loadSettings(xmlFile);

    currentXmlFile = xmlFile;

    settingsDirectory = currentXmlFile;

    int posLastSlash = settingsDirectory.rfind("/");
    if( posLastSlash > 0) settingsDirectory.erase(settingsDirectory.begin()+ posLastSlash+1, settingsDirectory.end()  );
    else settingsDirectory = "";

    settings.loadFile(currentXmlFile);
    usingXml = true;

    for(unsigned int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL ){
            int numParams = xmlObjects[i].numParams;

            for(int j = 0; j < numParams; j++){
                string str = xmlObjects[i].xmlName+":val_"+ofToString(j);
                
                string type = xmlObjects[i].guiObj->value.getTypeAsString();
                if( type != "string" ){
                    float val = settings.getValue(str, xmlObjects[i].guiObj->value.getValueF(j));
                    xmlObjects[i].guiObj->setValue(val, j);
                }else{
                    xmlObjects[i].guiObj->value.paramGroup.getString(j) = settings.getValue(str, xmlObjects[i].guiObj->value.getValueAsString(j));
                }
            }
            xmlObjects[i].guiObj->updateValue();
        }
    }
}

//-----------------------------
void ofxControlPanel::reloadSettings(){
    
    if( currentXmlFile == "" ){
        currentXmlFile = "controlPanelSettings.xml";
    }
    loadSettings(currentXmlFile);

}

//-----------------------------
void ofxControlPanel::reloadSettingsForPanel( string name ){
    
    guiTypePanel * panelPtr = NULL; 
    
    for(int i = 0; i < panels.size(); i++){
        if( panels[i]->name == name ){
            panelPtr = panels[i]; 
            break;
        }
    }
    
    if( currentXmlFile == "" ){
        currentXmlFile = "controlPanelSettings.xml";
    }
    
    if( panelPtr == NULL ){
        ofLogError("ofxControlPanel::reloadSettingsForPanel - couldn't find panel ") << name;
        return; 
    } 
    
    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL ){
                if( panelPtr->containsElement( xmlObjects[i].guiObj ) ){
                int numParams = xmlObjects[i].numParams;
//                printf("loading setting for xmlObject %s\n", xmlObjects[i].xmlName.c_str() );
                
                for(int j = 0; j < numParams; j++){
                    string str = xmlObjects[i].xmlName+":val_"+ofToString(j);
                    
                    string type = xmlObjects[i].guiObj->value.getTypeAsString();
                    if( type != "string" ){
                        float val = settings.getValue(str, xmlObjects[i].guiObj->value.getValueF(j));
                        xmlObjects[i].guiObj->setValue(val, j);
                    }else{
                        xmlObjects[i].guiObj->value.paramGroup.getString(j) = settings.getValue(str, xmlObjects[i].guiObj->value.getValueAsString(j));
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
}

//-------------------------------
void ofxControlPanel::saveSettings(string xmlFile,  bool bUpdateXmlFile){
    for(int i = 0; i < (int) guiObjects.size(); i++)guiObjects[i]->saveSettings(xmlFile);

    for(int i = 0; i < (int) xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL ){
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
    settings.saveFile(xmlFile);
	if( bUpdateXmlFile ){
		currentXmlFile = xmlFile;
	}
    usingXml = true;
}

//-----------------------------
void ofxControlPanel::saveSettings(){

    if(currentXmlFile == ""){
        currentXmlFile = "controlPanelSettings.xml";
    }
    
    saveSettings(currentXmlFile, true);
    
    usingXml = true;
}

//-------------------------------
void ofxControlPanel::setXMLFilename(string xmlFile){
    currentXmlFile = xmlFile;
}

// ############################################################## //
// ##
// ##       Visibilty
// ##
// ############################################################## //


//---------------------------
void ofxControlPanel::setMinimized(bool bMinimize){
    minimize = bMinimize;
}

//---------------------------
bool ofxControlPanel::isMinimized() {
    return minimize;
}

//---------------------------
bool ofxControlPanel::toggleMinimized() {
    return minimize = !minimize;
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
    hidden = true;
}

//-------------------------------
void ofxControlPanel::toggleView(){
    hidden = !hidden;
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
        minimize = !minimize;
    }else if( usingXml && isInsideRect(x, y, saveButton) ){
        saveSettings();
        saveDown = true;
        if( eventsEnabled && bEventsSetup ) {
            guiCallbackData saveData;
            saveData.setup("CP_SAVE", "CP_SAVE");
            saveData.addValueS( currentXmlFile );
            ofNotifyEvent( guiEvent, saveData, this );
        }
		printf("saving settings!\n");
    }else if( usingXml && isInsideRect(x, y, restoreButton) ){
		printf("restoring settings!\n");
        reloadSettings();
        restoreDown = true;
    }else if( isInsideRect(x, y, topBar) && bDraggable){
        dragging = true;
        mouseDownPoint.set(x - boundingBox.x, y-boundingBox.y, 0);
    }else if(!minimize){
		int lastSelectedPanel = selectedPanel;
        for(int i = 0; i < (int) panels.size(); i++){
            if( isInsideRect(x, y, panelTabs[i]) ){
                selectedPanel = i;
				if( lastSelectedPanel != selectedPanel ){
					bNewPanelSelected = true;
				}else{
					bNewPanelSelected = false;
				}
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
    restoreDown     = false;
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
    
    if( k == OF_KEY_RIGHT ){
        setSelectedPanel(getSelectedPanel() + 1);
        return true;
    }

    if( k == OF_KEY_LEFT ){
        setSelectedPanel(getSelectedPanel() - 1);
        return true;
    }
    
	for( int i=0; i<(int)panels.size(); i++ ){
        if( i == selectedPanel ){
            bool eaten = panels[i]->keyPressed( k );
            if ( eaten ){
                return true;
            }
        }
	}
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
	}
	else
	{
		saveButton       = ofRectangle(boundingBox.x + displayText.getTextWidth() + 20, boundingBox.y + 4, MAX(40, 8 + displayText.getTextWidth("save")) , MAX(12, displayText.getTextSingleLineHeight()) );
		restoreButton    = ofRectangle(saveButton.x + saveButton.width + 15, boundingBox.y + 4,  MAX(60, 8 + displayText.getTextWidth("restore")) , MAX(12, displayText.getTextSingleLineHeight()) );
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

        float panelH = boundingBox.height;
        if( minimize ){
            panelH = topBar.height;
        }

		if(!bInvisible || !elementSelected){

			glPushMatrix();
				glTranslatef(boundingBox.x, boundingBox.y, 0);
				//draw the background
				ofFill();
				glColor4fv(bgColor.getColorF());
				ofRect(0, 0, boundingBox.width, panelH);

				//draw the outline
				ofNoFill();
				glColor4fv(outlineColor.getColorF());
				ofRect(0, 0, boundingBox.width, panelH);
				ofLine(0, topBar.height, boundingBox.width, topBar.height);
			glPopMatrix();

			ofRect(minimizeButton.x, minimizeButton.y, minimizeButton.width, minimizeButton.height);

			if ( bDoSaveRestore ){
				ofPushStyle();
					ofFill();

					if( saveDown )glColor4fv(fgColor.getSelectedColorF());
					else glColor4fv(fgColor.getColorF());

					ofRect(saveButton.x, saveButton.y, saveButton.width,saveButton.height);
					ofSetColor(255, 255, 255);
				
					displayText.renderString("save", saveButton.x + 3, saveButton.y + saveButton.height -3);

				ofPopStyle();

				ofPushStyle();
					ofFill();

					if( restoreDown )glColor4fv(fgColor.getSelectedColorF());
					else glColor4fv(fgColor.getColorF());

					ofRect(restoreButton.x, restoreButton.y, restoreButton.width,restoreButton.height);
					ofSetColor(255, 255, 255);
				
					displayText.renderString("restore", restoreButton.x + 3, restoreButton.y + restoreButton.height -3);
				
				ofPopStyle();
			}


			ofPushMatrix();
				ofTranslate(2,0,0);
				glColor4fv(textColor.getColorF());
				guiBaseObject::renderText();
			ofPopMatrix();
		
		}

        if( !minimize ){
                    
            //don't let gui elements go out of their panels
//            glEnable(GL_SCISSOR_TEST);
            //glScissor(boundingBox.x, ofGetHeight() - boundingBox.getBottom()-topSpacing, boundingBox.width - borderWidth , boundingBox.height);

                glPushMatrix();
                    glTranslatef(hitArea.x, hitArea.y, 0);
                    for(int i = 0; i < (int) panels.size(); i++){
                        if( i == selectedPanel ){
							panels[i]->setShowOnlySelectedElement(bInvisible);
							panels[i]->render();
						}
					}
                glPopMatrix();

//            glDisable(GL_SCISSOR_TEST);
            
            if(!bInvisible || !elementSelected){
                for(int i = 0; i < (int) panelTabs.size(); i++){
                    if( i == selectedPanel){
                        ofPushStyle();
                            ofFill();
                            glColor4fv(fgColor.getSelectedColorF());
                            ofRect(panelTabs[i].x, panelTabs[i].y, panelTabs[i].width, panelTabs[i].height);
                            glColor4fv(outlineColor.getColorF());
                        ofPopStyle();
                    }
                    glColor4fv(outlineColor.getColorF());
                    ofNoFill();
                    ofRect(panelTabs[i].x, panelTabs[i].y, panelTabs[i].width, panelTabs[i].height);
                }
            }
        
            
            glPushMatrix();
                glTranslatef(hitArea.x, hitArea.y, 0);
                for(int i = 0; i < (int) panels.size(); i++){
                    if( i == selectedPanel ){
                        panels[i]->renderStatus();
                    }
                }
            glPopMatrix();
        }

    ofPopStyle();
	
}



