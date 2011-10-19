#pragma once

#include "ofMain.h"
#include "vectorField.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"

class motionDraw : public ofBaseDraws{
	public:
	
		motionDraw(){
			vid		= NULL;
			field	= NULL;
			drawScale = 1.0;
		}
	
		void setup( ofBaseDraws * vidPtr, vectorField * fieldPtr ){
			vid		= vidPtr;
			field	= fieldPtr;
		}
		
		void setDrawScale(float scale){
			drawScale = scale;
		}
	
		void draw(float x, float y, float w, float h){
			ofSetColor(0xFFFFFF);
			vid->draw(x, y, w, h);
			field->draw(x, y, w, h, drawScale);
		}
		
		void draw(float x, float y){
			draw(x, y, 320, 240);
		}
	
		virtual float getHeight(){
			return 240.0f;
		}
		virtual float getWidth(){
			return 320.0f;
		}	
		
		ofBaseDraws * vid;
		vectorField * field;
		float drawScale;
};

class motionStats : public ofBaseDraws{
		public:
		
		motionStats(){
			drawScale = 1.0;
		}
		
		void updateFromField( vectorField &field, float smoothing = 1.0){
			ofxVec2f valIn = field.getTrueAverage();
			
			val *= 0.9;
			val += valIn * 0.1;
			
			normalized *= smoothing;
			normalized += valIn.normalized() * (1.0 - smoothing);
		}
		
		void setDrawScale(float scale){
			drawScale = scale;
		}

		void draw(float x, float y, float w, float h){
			ofPushMatrix();
			ofTranslate(x, y, 0);
			
			ofSetColor(255, 120, 33);
			ofLine(w/2, h/2, w/2 + normalized.x * h * 0.4, h/2 + normalized.y * h * 0.4);			

			ofSetColor(0, 200, 33);
			ofLine(w/2, h/2, w/2 + val.x * drawScale, h/2 + val.y * drawScale);
			
			ofPopMatrix();
		}

		void draw(float x, float y){
			draw(x, y, 320, 240);
		}
	
		virtual float getHeight(){
			return 240.0f;
		}
		virtual float getWidth(){
			return 320.0f;
		}	

		float drawScale;
		ofxVec2f normalized;
		ofPoint val;
};


class videoMotionExample{

	public:

        //------------
        void setup(int width, int height);

        //-----------
        //some funky code that theo wrote
        //goes through for every vector in the vector field
        //and looks at the closest pixel and its neighbours.
        //motion is then calculated as the sum of the vectors between
        //pixel and its neighbours

        //works on the motion history image (gradiant image)
        void convertMotionHistoryToField();

        //-----------
        void update(unsigned char * pixelsIn, int width, int height );

        //-----------
        //here we add up all the directions from the
        //vector field and average them out to an overall
        //direction - this will be quite small so you will want
        //to scale it up.
        ofxVec2f getOverallMotionFromField();

        //-----------
        void draw(float x, float y);
        void setFadeAmnt(float fadeVal);
        void setThreshold(int threshVal);
		
        ofxCvGrayscaleImage gray;
        ofxCvColorImage color;

        ofxCvGrayscaleImage thresh;
        ofxCvGrayscaleImage prev;

        ofxCvFloatImage motion;
        ofxCvFloatImage threshF;

        vectorField motionField;

        float threshAmnt, fadeAmnt;
};


