#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "guiIncludes.h"

class xmlAssociation{
public:
    xmlAssociation(shared_ptr<guiBaseObject> objPtr, string xmlNameIn, int numParamsIn){
        guiObj      = objPtr;
        xmlName     = xmlNameIn;
        numParams   = numParamsIn;
    }

    shared_ptr<guiBaseObject> guiObj;// = nullptr;
    string xmlName = "";
    int numParams=0;
};

extern guiColor gTextColor;
extern guiColor gFgColor;
extern guiColor gBgColor;
extern guiColor gOutlineColor;


class ofxControlPanel: public guiBaseObject{
public:
    
    static bool shouldRemoveGuiBaseObject( const shared_ptr<guiBaseObject> aObj );
	
    static float borderWidth;
    static float topSpacing;
    static float tabWidth;
    static float tabHeight;
    static ofBitmapFont bitmapFont;

    ofxControlPanel();
    ~ofxControlPanel();
    
    //set the control panel name
    void setup(string controlPanelName, float panelX = 0, float panelY = 0, float width = ofGetWidth(), float height = ofGetHeight(), bool doSaveRestore = true, bool bPanelLocked = false);

		//add as many panels as you need
    bool hasPanel( string aPanelName );
    weak_ptr<guiTypePanel> addPanel(string panelName, int numColumns = 8, bool locked = false);
    void removePanel( string panelName );
    
    //set which panel you are adding your object to.
    void setWhichPanel(int whichPanel);
    void setWhichPanel(string panelName);
    void setWhichColumn(int column);

    void lock();
    void unlock();
    
    void add( ofParameterGroup & group );
    void add( ofParameterGroup & group, vector<bool> bOnesToInclude );

//        guiTypeToggle * addToggle(string name, bool defaultValue);
    shared_ptr<guiTypeToggle> addToggle( ofParameter<bool> & param);
    shared_ptr<guiTypeToggle> add( ofParameter<bool> & param);

//        guiTypeMultiToggle * addMultiToggle(string name, int defaultBox, vector <string> boxNames);
    shared_ptr<guiTypeMultiToggle> addMultiToggle(ofParameter <int> & param, vector <string> boxNames);
    
//        guiTypeTextDropDown * addTextDropDown(string name, int defaultBox, vector <string> boxNames);
    shared_ptr<guiTypeTextDropDown> addTextDropDown(ofParameter<int>& param, vector <string> boxNames);

    shared_ptr<guiTypeSlider> add( ofParameter <float> &param );
    shared_ptr<guiTypeSlider> addSlider( ofParameter <float> &param );
//		guiTypeSlider * addSlider(string sliderName, float value , float min, float max);

    shared_ptr<guiTypeSlider> add( ofParameter <int> &param );
    shared_ptr<guiTypeSlider> addSlider( ofParameter <int> &param );
//        guiTypeSlider * addSliderInt(string sliderName, int value , int min, int max);

    shared_ptr<guiTypeRangeSlider> add( ofParameter<float>& paramMin, ofParameter<float>& paramMax );
    shared_ptr<guiTypeRangeSlider> addSlider( ofParameter<float>& paramMin, ofParameter<float>& paramMax );
//        guiTypeRangeSlider* addSlider(string sliderName, float value1, float value2, float min, float max);

//        guiType2DSlider * addSlider2D(string sliderName, float valueX, float valueY, float minX, float maxX, float minY, float maxY);
    shared_ptr<guiType2DSlider> addSlider2D( ofParameter <int> &param1, ofParameter <int> &param2 );
    shared_ptr<guiType2DSlider> addSlider2D( ofParameter <float> &param1, ofParameter <float> &param2 );

    shared_ptr<guiTypeVariableLister> addVariableLister(ofParameterGroup & group);
    
    shared_ptr<guiTypeChartPlotter> addChartPlotter(ofParameter<float>& param );
    shared_ptr<guiTypeChartPlotter> addChartPlotter(ofParameter<float>& param, float minYVal, float maxYVal);
    shared_ptr<guiTypeFPSChartPlotter> addFpsChartPlotter();
    shared_ptr<guiTypeFPSChartPlotter> addFpsChartPlotter(float minValY, float maxValY);

    //add objects
    shared_ptr<guiBaseObject> addGuiBaseObject( shared_ptr<guiBaseObject> element, int varsToAddToXml); //set varsToAdd to 0 if you don't need xml saving
    
    shared_ptr<guiTypeLabel> addLabel( string text, bool highlight = false, bool saveToXml = false);
    shared_ptr<guiTypeLabel> addLabel( ofParameter <string> & text, bool highlight = false, bool saveToXml = false);

    //add objects - helpers
    shared_ptr<guiTypeDrawable> addDrawableRect(string name, ofBaseDraws* drawablePtr, int drawW, int drawH);
    shared_ptr<guiTypeVideo> addVideoRect(string name, ofVideoPlayer* drawablePtr, int drawW, int drawH);
    shared_ptr<guiTypeCustom> addCustomRect(string name, guiCustomImpl* customPtr, int drawW, int drawH);
    

//    weak_ptr<guiTypeLogger> addLogger(string name, simpleLogger * logger, int drawW, int drawH);
//    weak_ptr<guiTypeFileLister> addFileLister( string name, simpleFileLister * lister, int drawW, int drawH );
    shared_ptr<guiTypeFileLister> addFileLister( ofParameter<string>& aparam, string aDirectory );
    shared_ptr<guiTypeFilePicker> addFilePicker( ofParameter<string>& aparam );
    
    shared_ptr<guiTypeColorPicker> addColorPicker( ofParameter<ofColor>& aparam );

//		guiTypeTextInput* addTextInput( string name, string text, int width );

    // remove an object
//    void removeObject( string xmlName );
    
    template<typename Type>
    shared_ptr<Type> get( ofAbstractParameter& aparam ) {
        if( aparam.getName().size() < 2 ) {
            shared_ptr<Type> temp; return temp;
        }
        for( int i = 0; i < xmlObjects.size(); i++ ) {
            if(xmlObjects[i].guiObj && xmlObjects[i].guiObj->value.paramGroup.size() ) {
                bool bFoundIt=false;
                for( int k = 0; k < xmlObjects[i].guiObj->value.paramGroup.size(); k++ ) {
                    if( xmlObjects[i].guiObj->value.paramGroup.get(k).isReferenceTo(aparam) ) {
                        // we have it, woohoo //
                        return dynamic_pointer_cast<Type>(xmlObjects[i].guiObj);
                        bFoundIt = true;
                        break;
                    }
                    if(bFoundIt) break;
                }
            }
        }
        shared_ptr<Type> temp;
        return temp;
    }
    
    
    string getSelectedPanelName();
    int getSelectedPanel();
    void setSelectedPanel(int whichPanel);
    int getWhichPanel() { return currentPanel; }
    int getNumPanels(){ return panels.size(); }
    
    void setEnabled( ofParameterGroup& agroup, bool ab );
    void setEnabled( ofAbstractParameter& aparam, bool ab );
		
    //other
    void loadFont( string fontName, int fontsize );
    void setSize( int newWidth, int newHeight );

    //layout
    void enableIgnoreLayoutFlag(){
        bIgnoreLayout = true;
    }
    void disableIgnoreLayoutFlag(){
        bIgnoreLayout = false;
    }

    // keyboard events
    void enableKeyboardEvents();
    void disableKeyboardEvents();
    

    bool newPanelSelected();
    
    void setIncrementalSave(string incrmentalFileBaseName);
    void disableIncrementalSave();
    bool loadSettings(string xmlFile);
    
    bool reloadSettings();
    bool reloadSettingsForPanel(string panelName);

    bool saveSettings(string xmlFile, bool bUpdateXmlFile = true);
    bool saveSettings();
    void setXMLFilename(string xmlFile);
    bool isSaveButtonPressed();

    bool mouseIsInteracting(); //if the mouse is currently pressed inside the control panel
    void setDraggable(bool bDrag);
    void setMinimized(bool bMinimize);
    bool isMinimized();
    bool toggleMinimized();
    void show();
    void hide();
    
    void setStatusMessage(ofParameter <string> & message, int whichPanel = -1);
    
    //this only shows the element you are interacting with when the mouse is pressed
    void setInvisibleMode(bool bInvis);
    
    bool isVisible();
    bool isHidden();

    void toggleView();
    bool mousePressed(float x, float y, int button);
    bool mouseDragged(float x, float y, int button);
    void mouseReleased(float x, float y, int button);
    void mouseMoved( float x, float y );
            
    /// return true if we consume the keypress
    bool keyPressed( int k );
    bool keyReleased( int k );

    void updateBoundingBox();
    void update();
    void draw();

    void mousePressed(ofMouseEventArgs & args){
        if( fabs( ofGetFrameNum() - lastFrameDrawn ) <= 1 ){
            mousePressed(args.x, args.y, args.button);
        }
    }
    
    void mouseDragged(ofMouseEventArgs & args){
        if( fabs( ofGetFrameNum() - lastFrameDrawn ) <= 1 ){
            mouseDragged(args.x, args.y, args.button);
        }
    }
    
    void mouseMoved(ofMouseEventArgs & args){
        if( fabs( ofGetFrameNum() - lastFrameDrawn ) <= 1 ){
            mouseMoved( args.x, args.y );
        }
    }
    
    void mouseReleased(ofMouseEventArgs & args){
        if( fabs( ofGetFrameNum() - lastFrameDrawn ) <= 1 ){
            mouseReleased(args.x, args.y, args.button);
        }
    }
    
    void keyPressed(ofKeyEventArgs & args){
        if( fabs( ofGetFrameNum() - lastFrameDrawn ) <= 1 ){
            keyPressed(args.key);
        }
    }
    
    void keyReleased(ofKeyEventArgs & args){
        if( fabs( ofGetFrameNum() - lastFrameDrawn ) <= 1 ){
            keyReleased(args.key);
        }
    }

    vector <xmlAssociation> xmlObjects;
    
    ofEvent<bool> guiSaveEvent;
    ofEvent<bool> guiLoadEvent;
		
protected:
    ofTrueTypeFont guiTTFFont;

    vector < shared_ptr<guiBaseObject> > guiObjects;
    vector < shared_ptr<guiTypePanel> > panels;
    vector <ofRectangle> panelTabs;
    
//		vector <guiCustomEvent *> customEvents;
    vector <ofAbstractParameter *> internalParams;

    ofxXmlSettings settings;
    string currentXmlFile;
    string settingsDirectory;

    ofRectangle topBar;
    ofRectangle minimizeButton;
    ofRectangle saveButton;
    ofRectangle saveAsButton;
    ofRectangle restoreButton;
    ofRectangle loadButton;

    void setLayoutFlag( shared_ptr<guiBaseObject> obj ){
        obj->bRemoveFromLayout = bIgnoreLayout;
    }
            
//		void eventsIn(guiCallbackData & data);

    static vector <ofxControlPanel *> globalPanelList;
    static ofxControlPanel * getPanelInstance(string panelName);

    void addXmlAssociation( shared_ptr<guiBaseObject> object, string xmlName, int defaultValue );

    //use isHidden() or isVisible() to access this
    bool hidden;
    bool usingXml;
    bool bUseTTFFont;
    bool minimize;

    bool loadDown;
    bool saveDown;
    bool saveAsDown;
    bool incrementSave;
    bool restoreDown;
    bool bDraggable;

    bool bDoSaveRestore;

    int selectedPanel;
    int currentPanel;

    ofPoint prevMouse;

    int sliderWidth;
    int lastFrameDrawn;
    
//		bool bEventsSetup;
//		bool eventsEnabled;
    bool bIgnoreLayout;
    bool bInvisible;
    bool hitSomething;

    bool bHasKeyboardEvents;
    
    ofPoint mouseDownPoint;

    bool dragging;
    bool bNewPanelSelected;
    bool elementSelected;
    
    string incrementSaveName;

};
