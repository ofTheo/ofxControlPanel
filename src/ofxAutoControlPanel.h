#pragma once

/*
 r4
 
 ofxAutoControlPanel makes ofxControlPanel a little easier to work with.
 
 the control panel is autoamtically toggle-able using the tab key.
 the panel is hooked up to mouse, draw, and update events automatically.
 addPanel() does an addPanel() followed by a setWhichPanel() to avoid redundancy and mistakes.
 the fps is autoamtically drawn at the lower right corner of the screen.
 */

#include "ofxControlPanel.h"
#include "ofEvents.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
template <typename T>
struct variadic_ : public vector<T> {
	variadic_(const T& t) {
		(*this)(t);
	}
	variadic_& operator()(const T& t) {
		this->push_back(t);
		return *this;
	}
};

typedef variadic_<string> variadic;

class ofxAutoControlPanel : public ofxControlPanel {
public:
	string msg;
	
	ofxAutoControlPanel() {
		msg = "";
		setXMLFilename("settings.xml");
		ofAddListener(ofEvents().update, this, &ofxAutoControlPanel::update);
		ofAddListener(ofEvents().draw, this, &ofxAutoControlPanel::draw);
		ofAddListener(ofEvents().keyPressed, this, &ofxAutoControlPanel::keyPressed);
		ofAddListener(ofEvents().mousePressed, this, &ofxAutoControlPanel::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &ofxAutoControlPanel::mouseReleased);
		ofAddListener(ofEvents().mouseDragged, this, &ofxAutoControlPanel::mouseDragged);
	}
	void setup() {
		ofxControlPanel::setup("Control Panel", 5, 5, 280, 600);
	}
	void setup(int width, int height) {
		ofxControlPanel::setup("Control Panel", 5, 5, width, height);
	}
	void update(ofEventArgs& event) {
		ofxControlPanel::update();
	}
	void draw(ofEventArgs& event) {
		if(!hidden) {
			ofPushStyle();
			ofPushMatrix();
			ofTranslate(.5, .5);
			ofxControlPanel::draw();
			ofPopMatrix();
			if(msg != "") {
				ofSetColor(0);
				ofFill();
				int textWidth =  10 + msg.length() * 8;
				ofRect(5, ofGetHeight() - 22, textWidth, 20);
				ofSetColor(255, 255, 255);
				ofDrawBitmapString(msg, 10, ofGetHeight() - 10);
			}
			
			ofSetColor(255);
			ofFill();
			ofRect(ofGetWidth() - 45, ofGetHeight() - 25, 40, 20);
			ofSetColor(0);
			ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 40, ofGetHeight() - 10);
			ofPopStyle();
		}
	}
	using ofxControlPanel::hasValueChanged;
	// usage: panel.hasValueChanged(variadic(1)(2)(3)(4)(5));
	bool hasValueChanged(const vector<string>& values) {
		for(int i = 0; i < values.size(); i++) {
			string cur = values[i];
			if(ofxControlPanel::hasValueChanged(cur)) {
				return true;
			}
		}
		return false;
	}
	void keyPressed(ofKeyEventArgs& event) {
		if(event.key == '\t') {
			if(hidden) {
				show();
			} else {
				hide();
			}
		}
		if(event.key == 'f') {
			ofToggleFullscreen();
		}
	}
	void show() {
		ofxControlPanel::show();
		//ofShowCursor();
	}
	void hide() {
		ofxControlPanel::hide();
		//ofHideCursor();
	}
	void mousePressed(ofMouseEventArgs& event) {
		ofxControlPanel::mousePressed(event.x, event.y, event.button);
	}
	void mouseReleased(ofMouseEventArgs& event) {
		ofxControlPanel::mouseReleased();
	}
	void mouseDragged(ofMouseEventArgs& event) {
		ofxControlPanel::mouseDragged(event.x, event.y, event.button);
	}
	
	// by default, make sliders float
	using ofxControlPanel::addSlider;
	void addSlider(string name, string xmlName, float value, float low, float high, bool isInt = false) {
		ofxControlPanel::addSlider(name, xmlName, value, low, high, isInt);
	}
	void addSlider(string name, float value, float low, float high, bool isInt = false) {
		ofxControlPanel::addSlider(name, name, value, low, high, isInt);
	}
	using ofxControlPanel::addToggle;
	void addToggle(string name, bool value = false) {
		ofxControlPanel::addToggle(name, name, value);
	}
	using ofxControlPanel::addMultiToggle;
	void addMultiToggle(string name, int initial, vector<string> values) {
		ofxControlPanel::addMultiToggle(name, name, initial, values);
	}
	using ofxControlPanel::addPanel;
	void addPanel(string panelName, int columns = 1) {
		ofxControlPanel::addPanel(panelName, columns);
		ofxControlPanel::setWhichPanel(panelName);
	}
};
