//
//  guiTypeTextInput.cpp
//  ofxControlPanel2
//
//  Created by Nick Hardeman on 10/15/19.
//

#include "guiTypeTextInput.h"

//-----------------------------------------------------------
guiTypeTextInput::guiTypeTextInput() {}

guiTypeTextInput::~guiTypeTextInput() {
    if( bsetup ) {
        ofRemoveListener(value.paramGroup.parameterChangedE(), this, &guiTypeTextInput::_onParamChange );
    }
    bsetup = false;
}

//--------------------------------------------------------------
void guiTypeTextInput::setup( ofAbstractParameter& aparam ) {
//    mTextInput = avalue;
//    setText( avalue );
    value.addValue( aparam );
    mCursorIndex = mTextInput.size();
    _updateCursorPos();
    ofAddListener(value.paramGroup.parameterChangedE(), this, &guiTypeTextInput::_onParamChange );
    bsetup = true;
}

//--------------------------------------------------------------
void guiTypeTextInput::setupAsInput( ofAbstractParameter& aparam ) {
    value.addValue( aparam );
    mCursorIndex = mTextInput.size();
    _updateCursorPos();
    ofAddListener(value.paramGroup.parameterChangedE(), this, &guiTypeTextInput::_onParamChange );
    bsetup = true;
    guiBaseObject::setupNamesFromParams();
    setDimensions(getDefaultColumnWidth(), 14);
    displayText.setText(name);
}

//-----------------------------------------------
void guiTypeTextInput::onEnabledChanged() {
    onRelayout();
    
    if( !isEnabled() ) {
        setFocus(false);
    }
}

//-----------------------------------------------
void guiTypeTextInput::updateValue() {
    if( value.getNumValues() == 0 ){
        return;
    }
    setText( getVarAsString(0) );
}

//--------------------------------------------------------------
void guiTypeTextInput::setText( string atext ) {
    if( mTextInput != atext ) {
        mTextMesh.clear();
    }
    mTextInput = atext;
}

//--------------------------------------------------------------
string guiTypeTextInput::getText() {
    return mTextInput;
}

//--------------------------------------------------------------
bool guiTypeTextInput::isANumber() {
    if( mTextInput.size() < 1 ) return false;
    return mTextInput.find_first_not_of("0123456789.-e", 0) == std::string::npos
              && std::count(mTextInput.begin(), mTextInput.end(), '.') <= 1;
}

//--------------------------------------------------------------
bool guiTypeTextInput::hasFocus() {
    return bInFocus;
}

//--------------------------------------------------------------
void guiTypeTextInput::setFocus( bool ab ) {
    if( ab && !bInFocus ) {
        mCursorIndex = mTextInput.size();
        _updateCursorPos();
    }
    
    if( !ab && hasFocus() ) {
        setText( getVarAsString(0) );
    }
    
    bInFocus = ab;
}

//--------------------------------------------------------------
bool guiTypeTextInput::wasHit() {
    return bWasHit;
}

//--------------------------------------------------------------
void guiTypeTextInput::lostFocus() {
    setFocus( false );
    guiBaseObject::lostFocus();
}

//--------------------------------------------------------------
bool guiTypeTextInput::checkHit(float x, float y, bool isRelative) {
//    cout << "GuiTypeTextINput :: check hit " << (hitArea.inside( x, y )) << " | " << ofGetFrameNum() << endl;
    if( !isEnabled() ) {
        bWasHit = false;
        return false;
    }
    mousePt = glm::vec2(x,y);
    _updateCursorPos();
    if( hitArea.inside( x, y )) {
        if( !hasFocus() ) {
            if( ofGetElapsedTimeMillis() - mLastTimeHit < 500 ) {
                setFocus(true);
            }
            mLastTimeHit = ofGetElapsedTimeMillis();
        }
        bWasHit=true;
        return true;
    } else {
        setFocus(false);
    }
    return false;
}

//--------------------------------------------------------------
void guiTypeTextInput::release(float x, float y, bool isRelative) {
    bWasHit=false;
}

//--------------------------------------------------------------
bool guiTypeTextInput::keyPressed(int k) {
    if(!isEnabled()) return false;
    if( hasFocus() ) {
        if( k == OF_KEY_LEFT ) {
            mCursorIndex -= 1;
            if( mCursorIndex < 0 ) mCursorIndex = 0;
            _updateCursorPos();
        } else if( k == OF_KEY_RIGHT ) {
            mCursorIndex += 1;
            if( mCursorIndex > mTextInput.size() ) mCursorIndex = mTextInput.size();
            _updateCursorPos();
        } else if( k == OF_KEY_BACKSPACE ) {
            if( mCursorIndex > 0 && mTextInput.size() > 0 ) {
                mCursorIndex -= 1;
                if( mCursorIndex < mTextInput.size() ) {
                    mTextInput.erase( mCursorIndex );
                }
               _onTextChange();
            }
        } else if( k == OF_KEY_DEL ) {
            if( mCursorIndex < mTextInput.size()-1 && mTextInput.size() > 0 ) {
               // mCursorIndex += 1;
                if( mCursorIndex < mTextInput.size() ) {
                    mTextInput.erase( mCursorIndex );
                }
                _onTextChange();
            }
//        } else if( (k >= '0' && k <= '9') || k == '.' || k == '-') {
//            mTextInput += k;
//            mCursorIndex = mTextInput.size();
//            _onTextChange();
        } else if( k == OF_KEY_RETURN ) {
            _onTextEntered();
            return true;
        }
        
        if(_isValidInputKey(k)) {
            mTextInput += k;
            mCursorIndex = mTextInput.size();
            _onTextChange();
        }
        
//        cout << "guiTypeTextInput :: keyPressed: " << k << " str: " << mTextInput << " | " << ofGetFrameNum() << endl;
        
        return true;
    }
    return false;
}

//--------------------------------------------------------------
bool guiTypeTextInput::keyReleased(int k) {
    if( hasFocus() ) {
        
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void guiTypeTextInput::addToRenderMesh( ofMesh& arenderMesh ) {
    addToRenderMesh( arenderMesh, 0, 0 );
}

//--------------------------------------------------------------
void guiTypeTextInput::addToLinesRenderMesh( ofMesh& arenderMesh ) {
    addRectangleToLinesMesh( arenderMesh, hitArea, outlineColor.getColor() );
}

//--------------------------------------------------------------
void guiTypeTextInput::addToTextRenderMesh( ofMesh& arenderMesh ) {
    addToTextRenderMesh( arenderMesh, 0, 0 );
}

//--------------------------------------------------------------
void guiTypeTextInput::addToRenderMesh( ofMesh& arenderMesh, float ax, float ay ) {
    ofRectangle brrect( ax+hitArea.x, ay+hitArea.y, hitArea.width, hitArea.height );
    ofFloatColor tcolor(0.0,0.0,0.0, 0.7);
    if( hasFocus() ) {
        tcolor = ofFloatColor(0.0,0.0,0.0, 0.9);
    }
    addRectangleToMesh(arenderMesh, brrect, tcolor );
    
    if( !hasFocus() ) return;
    
    float tc = sin( ofGetElapsedTimef() * 13.f ) * 0.5 + 0.5;
    ofRectangle rrect( mCursorPos.x+hitArea.x+ax, hitArea.getTop()+ay, 2, hitArea.getHeight() );
    addRectangleToMesh(arenderMesh, rrect, ofFloatColor( 1, 1, 1, tc ) );
}

//--------------------------------------------------------------
void guiTypeTextInput::addToTextRenderMesh( ofMesh& arenderMesh, float ax, float ay ) {
    
    if( mTextMesh.getNumVertices() == 0 ) {
        mTextMesh.clear();
        
        float textW = displayText.getTextWidth(getText());
        displayText.addStringToMesh( mTextMesh, getText(), ax + hitArea.x + 2.0, ay + hitArea.y, textColor.getColor() );
        displayText.addStringToMesh( mTextMesh, name, boundingBox.x, boundingBox.y, textColor.getColor() );
    }
    arenderMesh.append(mTextMesh);
}


//--------------------------------------------------------------
void guiTypeTextInput::_updateCursorPos() {
    if( mTextInput.size() < 1 ) {
        mCursorPos.x = 0;
        return;
    }
    
    float tw = displayText.getTextWidth(mTextInput);
    float perChar = tw / (float)mTextInput.size();
    mCursorPos.x = (float)mCursorIndex * perChar;
    
}

//--------------------------------------------------------------
void guiTypeTextInput::_onTextChange() {
    _updateCursorPos();
    string ts = getText();
    mTextMesh.clear();
    ofNotifyEvent( textChangeEvent, ts, this );
}

//--------------------------------------------------------------
void guiTypeTextInput::_onTextEntered() {
    
    if( _isTextValid() ) {
        string ts = getText();
        _updateParamFromString( getText() );
        mTextMesh.clear();
        ofNotifyEvent(textEnteredEvent, ts, this );
    } else {
        setText( getVarAsString(0) );
    }
    _updateCursorPos();
    setFocus(false);
}

//--------------------------------------------------------------
void guiTypeTextInput::_updateParamFromString( string astring ) {
    if(value.getNumValues() > 0) {
        string type = value.getTypeAsString(0);
        if( type == "float" ) {
            value.setValue( ofToFloat(astring) );
        } else if( type == "int" ) {
            value.setValue( ofToInt(astring) );
        } else if( type == "string" ) {
            value.paramGroup.getString(0) = astring;
        } else if( type == "bool" ) {
            if( ofToLower(astring) == "true" || astring == "1" ) {
                value.setValue(1);
            } else {
                value.setValue(0);
            }
        }
    }
}

//--------------------------------------------------------------
bool guiTypeTextInput::_isTextValid() {
    if(value.getNumValues() > 0) {
        string type = value.getTypeAsString(0);
        if( type == "float" ) {
            return isANumber();
        } else if( type == "int" ) {
            return isANumber();
        } else if( type == "string" ) {
            return true; //?
        } else if( type == "bool" ) {
            if( ofToLower(mTextInput) == "true" || mTextInput == "1" ) {
                return true;
            } else if( ofToLower(mTextInput) == "false" || mTextInput == "0" ) {
                return true;
            }
        }
    }
    return false;
}

//--------------------------------------------------------------
bool guiTypeTextInput::_isParamNumber() {
    string type = value.getTypeAsString(0);
    if( type == "float" ) { return true; }
    if( type == "int" ) { return true; }
    return false;
}

//--------------------------------------------------------------
bool guiTypeTextInput::_isValidInputKey( int akey ) {
    
    vector<int> nonoKeys = {
        OF_KEY_DEL, OF_KEY_ALT, OF_KEY_BACKSPACE, OF_KEY_UP, OF_KEY_DOWN, OF_KEY_RIGHT, OF_KEY_LEFT,
        OF_KEY_RETURN, OF_KEY_F1, OF_KEY_F2, OF_KEY_F3, OF_KEY_F4, OF_KEY_F5, OF_KEY_F6, OF_KEY_F7, OF_KEY_F8, OF_KEY_F9,
        OF_KEY_SHIFT, OF_KEY_COMMAND, OF_KEY_ESC, OF_KEY_SUPER, OF_KEY_LEFT_SHIFT, OF_KEY_RIGHT_SHIFT
    };
    
    for( int i = 0; i < nonoKeys.size(); i++ ) {
        if( akey == nonoKeys[i] ) {
            return false;
        }
    }
    
    if( _isParamNumber() ) {
        if( (akey >= '0' && akey <= '9') || akey == '.' || akey == '-') {
            return true;
        }
        return false;
    }
    
    return true;
}

//--------------------------------------------------------------
void guiTypeTextInput::_onParamChange(ofAbstractParameter& aparam) {
    setText( getVarAsString(0) );
}


