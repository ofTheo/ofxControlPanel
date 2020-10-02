#pragma once

#include "guiBaseObject.h"
//#include "guiColor.h"
//#include "simpleColor.h"
//#include "guiValue.h"

#define LOCK_WIDTH 10
#define LOCK_HEIGHT 10
#define LOCK_BORDER 3

class guiTypePanel : public guiBaseObject{
public:

    //------------------------------------------------
    guiTypePanel();

    void setup(string panelName);
    void addColumn(float minWidth);

    void enableStatusBar();
    void disableStatusBar();
    void setStatus(ofParameter <string> & msg);

    bool selectColumn(int which);
    void setElementSpacing(float spacingX, float spacingY);
    void setElementYSpacing( float spacingY ) { spacingAmntY = spacingY; }
    virtual bool checkHit(float x, float y, bool isRelative);

    /// add exactly this many pixels of space
    void addSpace( int height );
    
    /// add a region of blank space, height pixels high. will also add spacingAmntY space
    void addYBlank( float height ) {
        columns[col].y += height+spacingAmntY;
    }

    void updateBoundingBox();

    void updateGui(float x, float y, bool firstHit, bool isRelative);
    void mouseMoved(float x, float y, bool isRelative);
    
    virtual void update();

    void addElement( shared_ptr<guiBaseObject> element );
    void removeElement( shared_ptr<guiBaseObject> element );
    bool containsElement( shared_ptr<guiBaseObject> element );
    bool containsElement( string xmlName );
    shared_ptr<guiBaseObject> getElement( string xmlName );
    
    void relayout();
		
    void resetSelectedElement();
    void setShowOnlySelectedElement(bool showOnlySelected);
    bool hasSelectedElement();
    shared_ptr<guiBaseObject> getSelectedElement();
	
    void drawLocked();
    void drawUnlocked();
    void render();
    void renderStatus();
	
    ofRectangle lockRect;

    float currentXPos;
    float currentYPos;
    float spacingAmntX;
    float spacingAmntY;

    vector <ofRectangle> columns;
    vector <int> whichColumn;
    vector<int> targetColumns;
    int col = 0;
    int targetColumn = 0;
    
    bool showOnly;
    bool elementInteracting;
    int whichElementInteracting;

    bool showStatus;
    ofParameterGroup statusGroup;
    
protected:
#ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    ofMesh mRenderMesh;
    ofMesh mLinesMesh;
//    ofMesh mTextMesh;
#endif
    int elementWithFocusIndex = -1;

};
