#pragma once

#define OF_ADDON_USING_OFXOPENCV
//#define OF_ADDON_USING_OFXDIRLIST
#define OF_ADDON_USING_OFXVECTORMATH
//#define OF_ADDON_USING_OFXXMLSETTINGS


#include "ofMain.h"
#include "ofAddons.h"
#include "vectorField.h"


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


class computerVision{

	public:

        //----------
        void loadVideo(string path){
            video.loadMovie(path);
            video.play();

            color.allocate(video.width, video.height);
            gray.allocate(video.width, video.height);
            thresh.allocate(video.width, video.height);
            prev.allocate(video.width, video.height);
            motion.allocate(video.width, video.height);
            threshF.allocate(video.width, video.height);

            fadeAmnt    = 0.66;
            threshAmnt  = 29;
            bPauseVideo = false;
            frameCount  = 0;
            bUseCamera  = false;
        }

        //------------
        void setupCamera(int which, int width, int height){
            camera.setDeviceID(which);
            camera.initGrabber(width, height);

            color.allocate(camera.width, camera.height);
            gray.allocate(camera.width, camera.height);
            thresh.allocate(camera.width, camera.height);
            prev.allocate(camera.width, camera.height);
            motion.allocate(camera.width, camera.height);
            threshF.allocate(camera.width, camera.height);

            fadeAmnt    = 0.66;
            threshAmnt  = 29;
            bPauseVideo = false;
            frameCount  = 0;
            bUseCamera  = true;
        }


        //-----------
        //some funky code that theo wrote
        //goes through for every vector in the vector field
        //and looks at the closest pixel and its neighbours.
        //motion is then calculated as the sum of the vectors between
        //pixel and its neighbours

        //works on the motion history image (gradiant image)
        void convertMotionHistoryToField(){
            int numPixels = motion.width * motion.height;
            unsigned char * pixels = motion.getPixels();

            unsigned char gradientThresh = 10;
            int index = 0;
            int yPos  = 0;
            int yypos = 0;
            unsigned char tmpVal = 0;
            int pixX, pixY;

            ofxVec2f vec;
            ofxVec2f tmp;

            float xPct = 0;
            float yPct = 0;

            float xStep = (float)motion.width / NUM_BINS_X;
            float yStep = (float)motion.height / NUM_BINS_Y;

            //go through for every vector in the vector field
            for(int y = 0; y < NUM_BINS_Y; y++){
                pixY = yStep * (float)y ;
                yPos = pixY  * motion.width;

                for(int x = 0; x < NUM_BINS_X; x++){
                    pixX = xStep * (float)x;

                    index = pixX + yPos;

                    //if the pixel is close to black we don't want to use for motion information as it is old
                    if(pixels[index] < gradientThresh){
                        motionField.field[x][y] = 0;
                    }else{
                        tmp = 0;
                        //go through for the pixels neighbours
                        for(int yy = -1; yy < 2; yy++){
                            for(int xx = -1; xx < 2; xx++){
                                //if we are outside the image - we skip the pixel
                                if(pixX + xx < 0 || pixX + xx > motion.width ||  pixY + yy < 0 || pixY + yy > motion.height){
                                    continue;
                                }else{
                                   tmpVal = pixels[index + xx + (yy*motion.width) ];

                                   //if the neighbour is dark enough to be considered black
                                   if(tmpVal <= gradientThresh)continue;

                                   //otherwise we add the value to our overall vector for the pixel we are looking at
                                   if( pixels[index] < tmpVal ){
                                        tmp.x += xx;
                                        tmp.y += yy;
                                   }else if( pixels[index] > tmpVal ){
                                        tmp.x -= xx;
                                        tmp.y -= yy;
                                   }
                                }
                            }
                        }

                        //finally update the field
                        motionField.field[x][y] = tmp;
                    }
                }
            }
        }

        //-----------
        void update(){

            if(bPauseVideo ) return;

            if(bUseCamera){
				camera.grabFrame();

				if(camera.isFrameNew()){
					prev = gray;
					color.setFromPixels(camera.getPixels(), camera.width, camera.height);
					color.mirror(false, true);
				}
				
            }else{

                video.idleMovie();
                if(video.isFrameNew()){
                     prev = gray;
                     frameCount++;
                }else{
                    return;
                }

                color.setFromPixels(video.getPixels(), video.width, video.height);
            }

            //convert the color image to grayscale
            gray = color;

            //do absolute diff between the prev frame and current frame
            //all pixels that are different will show up as non-black
            thresh.absDiff(gray, prev);

            //threshold to 0 or 255 value
            thresh.threshold(threshAmnt);

            //convert to float
            threshF = thresh;

            //fade the motion history image
            motion /= 2;

            //add in the floating point version of the thresholded image
            motion += threshF;

            //do our motion tracking and blur the results a little
            convertMotionHistoryToField();
            motionField.blur(0.1);

        }

        //-----------
        //here we add up all the directions from the
        //vector field and average them out to an overall
        //direction - this will be quite small so you will want
        //to scale it up.
        ofxVec2f getOverallMotionFromField(){

            int numVecs = NUM_BINS_X * NUM_BINS_Y;

            ofxVec2f avg = 0;
            for(int y = 0; y < NUM_BINS_Y; y++){
                for(int x = 0; x < NUM_BINS_X; x++){
                    avg += motionField.field[x][y];
                }
            }

            avg /= numVecs;
            return avg;
        }
		
		float verticalMotion(){
			int numPixels = thresh.width * thresh.height;
            unsigned char * pixels = thresh.getPixels();
			int runningTotal;
			runningTotal = 0;
			int xMovement[thresh.width];
			
			int firstLeft, firstRight;
			firstLeft = -1;
			firstRight = -1;
			
			float moment;
			moment = 0;
			float weightTotal;
			weightTotal = 0;
			
			for (int i = 0; i < thresh.width; i++){
				xMovement[i] = 0;
			}
			
			for (int j = 0; j < numPixels; j += thresh.width){
				for (int i = 0; i < thresh.width; i ++){
					if (pixels[i+j] > 10) xMovement[i]++;
				}
			}
			
			for (int i = 0; i < thresh.width; i++){
				if(xMovement[i] != 0){
					if(firstLeft == -1) firstLeft = i;
					firstRight = i;
				}
			}
			
			//cout << runningTotal << endl;
			
			for (int i = firstLeft; i < firstRight + 1; i++){
				moment += ((i - firstLeft) * xMovement[i]);
			weightTotal += xMovement[i];
			}
	
			float weightedXVal;
			weightedXVal = (moment / weightTotal) + firstLeft;
	
			if(firstLeft == -1 || firstRight == -1) weightedXVal = -1;
			
			
			return weightedXVal;
		}

        //-----------
        void draw(float x, float y){
            ofSetColor(0xFFFFFF);
            gray.draw(x, y);
            thresh.draw(x, y + gray.height);
            motion.draw(x, y + gray.height + thresh.height);
            motionField.draw(x, y, gray.width, gray.height, 4);
            motionField.draw(x, y + gray.height + thresh.height, gray.width,gray.height, 10);
        }

        //-----------
        void setFadeAmnt(float fadeVal){
            fadeAmnt = fadeVal;
        }

        //-----------
        void setThreshold(int threshVal){
            threshAmnt = threshVal;
        }

        //-----------
        void togglePlayback(){
           motion = thresh;
           bPauseVideo = !bPauseVideo;
           if(bPauseVideo){
                video.setPaused(true);
           }else{
                video.setPaused(false);
           }
        }
		
        ofxCvGrayscaleImage gray;
        ofxCvColorImage color;

        ofxCvGrayscaleImage thresh;
        ofxCvGrayscaleImage prev;

        ofxCvFloatImage motion;
        ofxCvFloatImage threshF;

        ofVideoPlayer video;
        ofVideoGrabber camera;

        vectorField motionField;

        float threshAmnt, fadeAmnt;
        bool bPauseVideo;
        bool bUseCamera;
        int frameCount;

};


