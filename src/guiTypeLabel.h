/*
 *  guiTypeLabel.h
 *  artvertiser
 *
 *  Created by damian on 10/10/10.
 *  Copyright 2010 frey damian@frey.co.nz. All rights reserved.
 *
 */

#pragma once
#include "guiBaseObject.h"
//#include "guiColor.h"
//#include "simpleColor.h"
//#include "guiValue.h"

class guiTypeLabel : public guiBaseObject{
	
public:
	
	//------------------------------------------------
	void setup(ofParameter <string>  & label, bool highlight );
	void setText( string new_text );
    void internalUpdate();
	void render();
    
    //so we don't mess up our nice text formatting
    virtual void updateText(){}
    
    float getVerticalSpacing();
    
    #ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
    virtual void addToRenderMesh( ofMesh& arenderMesh );
    virtual void addToLinesRenderMesh( ofMesh& arenderMesh );
    virtual void addToTextRenderMesh( ofMesh& arenderMesh );
    #endif
    
    ofParameter <string> textLabel;
	
    bool bHighlight;
    
protected:
    #ifndef OFX_CONTROL_PANEL_NO_BATCH_RENDER
//    ofMesh mTextMesh;
    #endif
    string prevString = "---";
};
