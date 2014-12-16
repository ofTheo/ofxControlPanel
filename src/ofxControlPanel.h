#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "guiIncludes.h"


class xmlAssociation{
    public:
        xmlAssociation(guiBaseObject * objPtr, string xmlNameIn, int numParamsIn){
            guiObj      = objPtr;
            xmlName     = xmlNameIn;
            numParams   = numParamsIn;
        }

        guiBaseObject * guiObj;
        string xmlName;
        int numParams;
};

extern guiColor gTextColor;
extern guiColor gFgColor;
extern guiColor gBgColor;
extern guiColor gOutlineColor;

class ofxControlPanel: public guiBaseObject{

	public:
    
    static bool shouldRemoveGuiBaseObject( const guiBaseObject* aObj );
	
        static float borderWidth;
        static float topSpacing;
        static float tabWidth;
        static float tabHeight;

        ofxControlPanel();
        ~ofxControlPanel();
		
		//set the control panel name
        void setup(string controlPanelName, float panelX = 0, float panelY = 0, float width = 1024, float height = 768, bool doSaveRestore = true, bool bPanelLocked = false);

		//add as many panels as you need
        guiTypePanel * addPanel(string panelName, int numColumns = 8, bool locked = false);
        void removePanel( string panelName );
		
		//set which panel you are adding your object to.
        void setWhichPanel(int whichPanel);
        void setWhichPanel(string panelName);
        void setWhichColumn(int column);
		
    
        void addGroup( ofParameterGroup & group );

		guiTypeSlider * addSlider(string sliderName, float value , float min, float max);
		guiTypeSlider * addSliderInt(string sliderName, int value , int min, int max);

        guiTypeSlider * addSlider( ofParameter <int> &param );
        guiTypeSlider * addSlider( ofParameter <float> &param );

        guiType2DSlider * addSlider2D(string sliderName, float valueX, float valueY, float minX, float maxX, float minY, float maxY);
        guiType2DSlider * addSlider2D( ofParameter <int> &param1, ofParameter <int> &param2 );
        guiType2DSlider * addSlider2D( ofParameter <float> &param1, ofParameter <float> &param2 );

        guiTypeToggle * addToggle(string name, bool defaultValue);
        guiTypeToggle * addToggle( ofParameter<bool> & param);

		guiTypeMultiToggle * addMultiToggle(string name, int defaultBox, vector <string> boxNames);
		guiTypeMultiToggle * addMultiToggle(ofParameter <int> & param, vector <string> boxNames);

        guiTypeTextDropDown * addTextDropDown(string name, int defaultBox, vector <string> boxNames);
        guiTypeTextDropDown * addTextDropDown(ofParameter <int> & param, vector <string> boxNames);

        guiTypeVairableLister * addVariableLister(ofParameterGroup & group);
		guiTypeChartPlotter * addChartPlotter(ofParameter <float> & param, float minYVal, float maxYVal);
    
		//add objects
		guiBaseObject * addGuiBaseObject(guiBaseObject * element, int varsToAddToXml); //set varsToAdd to 0 if you don't need xml saving
		
        guiTypeLabel * addLabel( string text, bool highlight = false, bool saveToXml = false);
        guiTypeLabel * addLabel( ofParameter <string> & text, bool highlight = false, bool saveToXml = false);

		//add objects - helpers
    

    
        guiTypeDrawable * addDrawableRect(string name, ofBaseDraws * drawablePtr, int drawW, int drawH);
		guiTypeVideo * addVideoRect(string name, ofVideoPlayer * drawablePtr, int drawW, int drawH);       
		 guiTypeCustom * addCustomRect(string name, guiCustomImpl * customPtr, int drawW, int drawH);
		
    
		guiTypeLogger * addLogger(string name, simpleLogger * logger, int drawW, int drawH);
		guiTypeFileLister * addFileLister(string name, simpleFileLister * lister, int drawW, int drawH);

//		guiTypeTextInput* addTextInput( string name, string text, int width );

		// remove an object
		void removeObject( string xmlName );
		
		
		string getSelectedPanelName();				
		int getSelectedPanel();
		void setSelectedPanel(int whichPanel);
		int getWhichPanel() { return currentPanel; }
		int getNumPanels(){ return panels.size(); }
		
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
		
		
		//events
		void setupEvents();
		ofEvent <guiCallbackData> & createEventGroup(string eventGroupName, vector <string> xmlNames);
		ofEvent <guiCallbackData> & createEventGroup(string xmlName);

		void enableEvents();
		void disableEvents();
		ofEvent <guiCallbackData> & getEventsForPanel(int panelNo);
		ofEvent <guiCallbackData> & getAllEvents();
		ofEvent <guiCallbackData> & getEventGroup(string eventGroupName);

        void setValueB(string xmlName, bool value,  int whichParam = 0);
        void setValueI(string xmlName, int value,  int whichParam = 0);
        void setValueF(string xmlName, float value,  int whichParam = 0);
        bool getValueB(string xmlName, int whichParam = 0);
        float getValueF(string xmlName, int whichParam = 0);
        int getValueI(string xmlName, int whichParam = 0);

		bool newPanelSelected();
		
        void setIncrementalSave(string incrmentalFileBaseName);
        void disableIncrementalSave();
        void loadSettings(string xmlFile);
        
        void reloadSettings();
        void reloadSettingsForPanel(string panelName); 
    
        void saveSettings(string xmlFile, bool bUpdateXmlFile = true);
        void saveSettings();
        void setXMLFilename(string xmlFile);

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
		
	protected:
        ofTrueTypeFont guiTTFFont;

        vector <guiBaseObject *> guiObjects;
        vector <guiTypePanel *> panels;
        vector <ofRectangle> panelTabs;
		
		vector <guiCustomEvent *> customEvents;
		vector <ofAbstractParameter *> internalParams;

		ofxXmlSettings settings;
		string currentXmlFile;
		string settingsDirectory;

		ofRectangle topBar;
		ofRectangle minimizeButton;
		ofRectangle saveButton;
		ofRectangle restoreButton;

		void setLayoutFlag( guiBaseObject * obj ){
			obj->bRemoveFromLayout = bIgnoreLayout;
		}
				
		void eventsIn(guiCallbackData & data);

		static vector <ofxControlPanel *> globalPanelList;
		static ofxControlPanel * getPanelInstance(string panelName);

		void addXmlAssociation( guiBaseObject* object, string xmlName, int defaultValue );

		//use isHidden() or isVisible() to access this
        bool hidden;
        bool usingXml;
        bool bUseTTFFont;
		bool minimize;
		bool saveDown;
		bool incrementSave;
		bool restoreDown;
		bool bDraggable;
	
        bool bDoSaveRestore;

		int selectedPanel;
        int currentPanel;

        ofPoint prevMouse;

		int sliderWidth;
		int lastFrameDrawn;
		
		bool bEventsSetup;
		bool eventsEnabled;
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
