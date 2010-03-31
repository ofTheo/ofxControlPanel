#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "guiIncludes.h"


class xmlAssociation
{
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

class ofxControlPanel: public guiBaseObject{

	public:
        static float borderWidth;
        static float topSpacing;
        static float tabWidth;
        static float tabHeight;

        ofxControlPanel();
        ~ofxControlPanel();

        void setup(string controlPanelName, float panelX, float panelY, float width, float height);
        void loadFont( string fontName, int fontsize );

        guiTypePanel * addPanel(string panelName, int numColumns, bool locked = false);

        void setWhichPanel(int whichPanel);
        void setWhichPanel(string panelName);
        void setWhichColumn(int column);

		string getCurrentPanelName();

        void setSliderWidth(int width);

        guiTypeToggle * addToggle(string name, string xmlName, bool defaultValue);
        guiTypeMultiToggle * addMultiToggle(string name, string xmlName, int defaultBox, vector <string> boxNames);
        guiTypeSlider * addSlider(string sliderName, string xmlName, float value , float min, float max, bool isInt);
        guiType2DSlider * addSlider2D(string sliderName, string xmlName, float valueX, float valueY, float minX, float maxX, float minY, float maxY, bool isInt);
        guiTypeDrawable * addDrawableRect(string name, ofBaseDraws * drawablePtr, int drawW, int drawH);
		guiTypeVideo * addVideoRect(string name, ofVideoPlayer * drawablePtr, int drawW, int drawH);
        guiTypeCustom * addCustomRect(string name, guiCustomImpl * customPtr, int drawW, int drawH);
        guiTypeButtonSlider * addButtonSlider(string sliderName, string xmlName, float value , float min, float max, bool isInt);
        guiTypeTextDropDown * addTextDropDown(string name, string xmlName, int defaultBox, vector <string> boxNames);

		//THIS SHOULD BE CALLED AFTER ALL GUI SETUP CALLS HAVE HAPPENED
		void setupEvents(){
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
				printf("creating %s\n", groupName.c_str());
			}
			
			bEventsSetup = true;
		}
		
		void enableEvents(){
			if( !bEventsSetup ){
				setupEvents();
			}
			eventsEnabled = true;
		}
		
		void disableEvents(){
			eventsEnabled = false;
		}
		
		ofEvent <guiCallbackData> & getEventsForPanel(int panelNo){
			if( panelNo < panels.size() ){
				return getEventGroup("PANEL_EVENT_"+ofToString(panelNo));
			}else{			
				return guiEvent;
			}
		}
		
		void createEventGroup(string eventGroupName, vector <string> xmlNames){
			customEvents.push_back( new guiCustomEvent() );
			customEvents.back()->group = eventGroupName;
			customEvents.back()->names = xmlNames;
		}
		
		ofEvent <guiCallbackData> & getEventGroup(string eventGroupName){
			for(int i = 0; i < customEvents.size(); i++){
				if( eventGroupName == customEvents[i]->group ){
					return customEvents[i]->guiEvent;
				}
			}
			
			//if we don't find a match we return the global event
			ofLog(OF_LOG_ERROR, "error eventGroup %s does not exist - returning the global event instead", eventGroupName.c_str());
			return guiEvent;
		}
		
				
        void setValueB(string xmlName, bool value,  int whichParam = 0);
        void setValueI(string xmlName, int value,  int whichParam = 0);
        void setValueF(string xmlName, float value,  int whichParam = 0);
        bool getValueB(string xmlName, int whichParam = 0);
        float getValueF(string xmlName, int whichParam = 0);
        int getValueI(string xmlName, int whichParam = 0);

        void setIncrementSave(string incrmentalFileBaseName);
        void disableIncrementSave();
        void loadSettings(string xmlFile);
        void reloadSettings();
        void saveSettings(string xmlFile);
        void saveSettings();
        void setXMLFilename(string xmlFile);

        void setDraggable(bool bDrag);
        void setMinimized(bool bMinimize);
        void show();
        void hide();

        void toggleView();
        void mousePressed(float x, float y, int button);
        void mouseDragged(float x, float y, int button);
        void mouseReleased();

        void updateBoundingBox();
        void update();
        void draw();


        ofTrueTypeFont guiTTFFont;

        vector <xmlAssociation> xmlObjects;
        vector <guiBaseObject *> guiObjects;
        vector <guiTypePanel *> panels;
        vector <ofRectangle> panelTabs;
		
		vector <guiCustomEvent *> customEvents;

		ofxXmlSettings settings;
		string currentXmlFile;
		string settingsDirectory;

		ofRectangle topBar;
		ofRectangle minimizeButton;
		ofRectangle saveButton;
		ofRectangle restoreButton;

        string incrementSaveName;

        bool hidden;
        bool usingXml;
        bool bUseTTFFont;
		bool minimize;
		bool saveDown;
		bool incrementSave;
		bool restoreDown;
		bool bDraggable;

		int selectedPanel;
        int currentPanel;

        ofPoint prevMouse;

		int sliderWidth;
		
		bool bEventsSetup;
		bool eventsEnabled;

		ofPoint mouseDownPoint;

		bool dragging;
		
		
		protected:
		
		void eventsIn(guiCallbackData & data){
			if( !eventsEnabled ) return;
			
			//we notify the ofxControlPanel event object - aka the global ALL events callback
			ofNotifyEvent(guiEvent, data, this);
			
			//we then check custom event groups
			for(int i = 0; i < customEvents.size(); i++){
				for(int k = 0; k < customEvents[i]->names.size(); k++){
					if( customEvents[i]->names[k] == data.groupName ){
						ofNotifyEvent(customEvents[i]->guiEvent, data, this);
					}
				}
			}
		}

		

};
