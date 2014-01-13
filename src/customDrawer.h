/*
 *  customDrawer.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 02/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "guiCustomImpl.h"
#include "guiBaseObject.h" //theo added for isInsideRect()

class customDrawer : public guiCustomImpl {
	
private:
	
	vector < vector <ofPoint> > pts;
	
public:
	
	bool mouseIsRelative;
	//ofRectangle boundingRect; //theo changed
	
	customDrawer(){
	
	}
	
	// -------------------------------------
	void saveSettings(string filename){
	}
	
	void reloadSettings(){
	
	}
	
	void saveSettings(){
	
	}
	
	void loadSettings(string filename) {
	
	}
	
	// -------------------------------------
	float getWidth() {
		return boundingRect.width;
	}
	float getHeight() {
		return boundingRect.height;
	}
	
	// -------------------------------------
	void clearPoints() {
		for (int i=0; i<pts.size(); i++) {
			pts[i].clear();
		}
	}
	
	// -------------------------------------
	void addPoint(float x, float y) {
		pts.back().push_back(ofPoint(x, y));
	}
	
	void draw(float x, float y, float w, float h) {
		
		//printf("x is %f y is %f w is %f h is %f\n", x, y, w, h);
		//printf("bounds x is %f y is %f w is %f h is %f\n", boundingRect.x, boundingRect.y, boundingRect.width, boundingRect.height);
		
		glPushMatrix();
		glTranslatef(x, y, 0); //theo changed
		
		//always check if you are diving by zero - or do  h/MAX(1, boundingRect.height)
		glScalef(w/boundingRect.width, h/boundingRect.height, 0);

		ofNoFill();
		ofSetColor(255, 90, 9);
				
		for (int i=0; i<pts.size(); i++) {
			
			ofBeginShape();
			for (int j=0; j<pts[i].size(); j++) {
				ofVertex(pts[i][j].x, pts[i][j].y);
			}		
			ofEndShape(false);
			
		}
		
		glPopMatrix();
		
		ofNoFill();
		ofSetColor(255, 33, 33);
		ofRect(x, y, w, h);
	}
	
	// -------------------------------------
	void mousePressed(float x, float y) {
		vector <ofPoint> newPoints;
		pts.push_back(newPoints);
	}

	//these values come in with 0,0 being the top left of your bounding rect 
	// -------------------------------------
	void mouseDragged(float x, float y){

		//theo changed
		
		if( isInsideRect(x, y, ofRectangle(0, 0, boundingRect.width, boundingRect.height) ) ){
			addPoint(x, y); 
		}
	}
	
	void mouseReleased(){
	
	}
	
};