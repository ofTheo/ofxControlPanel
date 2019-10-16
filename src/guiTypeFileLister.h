#pragma once

#include "guiBaseObject.h"
//#include "guiColor.h"
//#include "simpleColor.h"
//#include "guiValue.h"
#include "simpleFileLister.h"

class guiTypeFileLister : public guiBaseObject{
public:

    guiTypeFileLister();
    ~guiTypeFileLister();
//    void setup(string listerName, simpleFileLister * listerPtr, float listerWidth, float listerHeight);
    void setup( ofParameter<string>& aparam, string aDirectory, float listerWidth, float listerHeight);
    
    virtual string getSelectedStringValue() override { return mParamRef; };

    virtual void updateText() override;
    void updateGui(float x, float y, bool firstHit, bool isRelative) override;
    void release(float x, float y, bool isRelative) override;
    void drawRecords(float x, float y, float width, float height);
    void render() override;

//		void notify();
	
	void clearSelection();
	
    int lineSpacing;
    int dblClickTime;

    int selection;
    int selectionTmp;
    long lastClickTime;
    bool usingSlider;
    float sliderWidth;
    float selectPct;
    float pct;
    int startPos, endPos;
    
	bool hasSelectionChanged() { return selection_has_changed; }
	void clearSelectionChangedFlag() { selection_has_changed = false; }
	
private:
    ofParameter<string> mParamRef;
    shared_ptr<simpleFileLister> lister;
	bool selection_has_changed;
    
};
