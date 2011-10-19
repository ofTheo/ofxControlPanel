#include "videoMotionExample.h"

//------------
void videoMotionExample::setup(int width, int height){

	color.allocate(width, height);
	gray.allocate(width, height);
	thresh.allocate(width, height);
	prev.allocate(width, height);
	motion.allocate(width, height);
	threshF.allocate(width, height);

	fadeAmnt    = 0.66;
	threshAmnt  = 29;
}


//-----------
//some funky code that theo wrote
//goes through for every vector in the vector field
//and looks at the closest pixel and its neighbours.
//motion is then calculated as the sum of the vectors between
//pixel and its neighbours

//works on the motion history image (gradiant image)
void videoMotionExample::convertMotionHistoryToField(){
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
void videoMotionExample::update(unsigned char * pixelsIn, int width, int height ){

	prev = gray;
	color.setFromPixels(pixelsIn, width, height);

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
	motion *= fadeAmnt;

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
ofxVec2f videoMotionExample::getOverallMotionFromField(){

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

//-----------
void videoMotionExample::draw(float x, float y){
	ofSetColor(0xFFFFFF);
	gray.draw(x, y);
	thresh.draw(x, y + gray.height);
	motion.draw(x, y + gray.height + thresh.height);
	motionField.draw(x, y, gray.width, gray.height, 4);
	motionField.draw(x, y + gray.height + thresh.height, gray.width,gray.height, 10);
}

//-----------
void videoMotionExample::setFadeAmnt(float fadeVal){
	fadeAmnt = fadeVal;
}

//-----------
void videoMotionExample::setThreshold(int threshVal){
	threshAmnt = (int)threshVal;
}


