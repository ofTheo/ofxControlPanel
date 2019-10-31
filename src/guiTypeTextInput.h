//
//  guiTypeTextInput.h
//  ofxControlPanel2
//
//  Created by Nick Hardeman on 10/15/19.
//

#pragma once
#include "guiBaseObject.h"

class guiTypeTextInput : public guiBaseObject {
public:
    guiTypeTextInput();
    ~guiTypeTextInput();
    
    void setup( ofAbstractParameter& aparam );
    void updateValue() override;
    
    void setText( string atext );
    string getText();
    bool isANumber();
    
    bool hasFocus();
    void setFocus( bool ab );
    bool wasHit();
    
    virtual void lostFocus() override;
    
    bool checkHit(float x, float y, bool isRelative) override;
    //should  be called on mouse up
    virtual void release(float x, float y, bool isRelative) override;
    
    bool keyPressed(int k) override;
    bool keyReleased(int k) override;
    
    virtual void addToRenderMesh( ofMesh& arenderMesh, float ax, float ay );
    virtual void addToTextRenderMesh( ofMesh& arenderMesh, float ax, float ay );
    
    glm::vec2 mousePt;
    
    ofEvent<string> textEnteredEvent;
    ofEvent<string> textChangeEvent;
    
protected:
    void _updateCursorPos();
    void _onTextChange();
    void _onTextEntered();
    void _updateParamFromString( string astring );
    bool _isTextValid();
    void _onParamChange( ofAbstractParameter& aparam );
    
    bool bInFocus = false;
    
    float mTextWidth = 32;
    bool bWasHit = false;
    
    string mTextInput = "";
    int mCursorIndex = 0;
    glm::vec2 mCursorPos;
    
    int mLastTimeHit = -1000;
    
    ofMesh mTextMesh;
    bool bsetup = false;
};
