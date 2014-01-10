#ifndef _VEC_FIELD
#define _VEC_FIELD

#define OF_ADDON_USING_OFXVECTORMATH

#include "ofMain.h"
#include "ofAddons.h"

#define NUM_BINS_X 120
#define NUM_BINS_Y 80

class vectorField{

	public:
		vectorField(){
			binW = 1.0/NUM_BINS_X;
			binH = 1.0/NUM_BINS_Y;
		}

		//-------------------------------------------------
		void addIntoField(float x, float y, ofVec2f vec, float radius = 1.0){

			if(x >= 1.0 || x < 0 || y >= 1.0 || y < 0){
				 return;
			}

			ofVec2f pos(x,y);
			ofVec2f raster;

			raster.x = pos.x;
			raster.y = pos.y;

			float startX = raster.x - radius;
			float startY = raster.y- radius;

			float endX = raster.x + radius;
			float endY = raster.y + radius;

			int sx = (startX * NUM_BINS_X)-1;
			int sy = (startY * NUM_BINS_Y)-1;

			int ex = (endX * NUM_BINS_X)+1;
			int ey = (endY * NUM_BINS_Y)+1;

			sx  = MAX(sx, 0);
			sy  = MAX(sy, 0);
			ex  = MAX(ex, 0);
			ey  = MAX(ey, 0);

			sx  = MIN(sx, NUM_BINS_X);
			sy  = MIN(sy, NUM_BINS_Y);
			ex  = MIN(ex, NUM_BINS_X);
			ey  = MIN(ey, NUM_BINS_Y);

			ofVec2f binPos;
			ofVec2f distVec;
			float dist = 0;
			float amnt = 0;

			for(int yy = sy; yy < ey; yy++){
				for(int xx = sx; xx < ex; xx++){

					binPos.set( (float)xx * binW, (float)yy * binH);
					distVec = binPos - pos;
					dist = distVec.length();

					if(dist > radius)continue;

					amnt = 1.0 - (dist - radius);

					//amnt * amnt makes the rolloff less linear
					mfield[xx][yy] += vec*amnt*amnt;
				}
			}
		}

		//-------------------------------------------------
		void blur(float amnt){

			if(amnt > 1.0)amnt = 1.0;
			else if(amnt < 0.0)amnt = 0.0;

			ofVec2f blur;

			float ramnt = (1.0 - amnt) * 0.25;

			for(int yy = 1; yy < NUM_BINS_Y-1; yy++){
				for(int xx = 1; xx < NUM_BINS_X-1; xx++){

					blur = mfield[xx][yy] * amnt + mfield[xx+1][yy] * ramnt +  mfield[xx-1][yy] * ramnt + mfield[xx][yy+1] * ramnt + mfield[xx][yy-1] * ramnt;
					mfield[xx][yy] = blur;
					
					if( fabs(mfield[xx][yy].x) <= 0.0001 && fabs(mfield[xx][yy].y) <= 0.0001 ){
						mfield[xx][yy] = ofVec2f(0.0);
					}	
				}
			}
		}

		//-------------------------------------------------
		ofVec2f getTrueAverage(){

			ofVec2f vec;
			for(int yy = 0; yy < NUM_BINS_Y; yy++){
				for(int xx = 0; xx < NUM_BINS_X; xx++){
					vec += mfield[xx][yy];
				}
			}
			
			int total = NUM_BINS_X * NUM_BINS_Y;
			if( total <= 0 )return ofVec2f(0);
			
			vec /= (float)(total);

			return vec;
		}

		//-------------------------------------------------
		ofVec2f getModeAverage(){
			
			int count = 0;
			ofVec2f vec;
			for(int yy = 0; yy < NUM_BINS_Y; yy++){
				for(int xx = 0; xx < NUM_BINS_X; xx++){
					
					if( mfield[xx][yy].x != 0.0 && mfield[xx][yy].y != 0.0 ){
						vec += mfield[xx][yy];
						count++;
					}
				}
			}
			
			if( count <= 0 )return ofVec2f(0);
			vec /= (float)count;

			return vec;
		}

		//-------------------------------------------------
		ofVec2f readFromField(float x, float y){

			if(x >= 1.0 || x < 0 || y >= 1.0 || y < 0){
				 return ofVec2f();
			}

			int rasterX = x * NUM_BINS_X;
			int rasterY = y * NUM_BINS_Y;

			ofVec2f vec = mfield[rasterX][rasterY];

			return vec;
		}


		//-------------------------------------------------
		void normalize(){

			for(int yy = 0; yy < NUM_BINS_Y; yy++){
				for(int xx = 0; xx < NUM_BINS_X; xx++){
					mfield[xx][yy].normalize();
				}
			}
		}

		//-------------------------------------------------
		void randomize(float mag, float pct){

			pct = MIN(1.0, pct);
			pct = MAX(0.0, pct);

			for(int yy = 0; yy < NUM_BINS_Y; yy++){
				for(int xx = 0; xx < NUM_BINS_X; xx++){
					mfield[xx][yy] *= 1.0 - pct;
					mfield[xx][yy] += ofVec2f( ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0)) * mag * pct;
				}
			}
		}

		//-------------------------------------------------
		void clear(){

			for(int yy = 0; yy < NUM_BINS_Y; yy++){
				for(int xx = 0; xx < NUM_BINS_X; xx++){
					mfield[xx][yy].set(0,0);
				}
			}
		}

		//-------------------------------------------------
		void fade(float pct){

			for(int yy = 0; yy < NUM_BINS_Y; yy++){
				for(int xx = 0; xx < NUM_BINS_X; xx++){
					mfield[xx][yy] *= pct;
				}
			}
		}


		//-------------------------------------------------
		void draw(int x, int y, float width, float height, float scale = 1.0){

			ofVec2f pixelPos;
			ofVec2f vec;

			glPushMatrix();
				glTranslatef(x, y, 0);

				for(int yy = 0; yy < NUM_BINS_Y; yy++){
					for(int xx = 0; xx < NUM_BINS_X; xx++){

						pixelPos.set( (float)xx * binW * width, (float)yy * binH * height );
						vec = (mfield[xx][yy]) * scale;


						glBegin(GL_LINES);
							ofSetColor(0x000044);
							glVertex2f(pixelPos.x, pixelPos.y);
							ofSetColor(0xFFFF33);
							glVertex2f(pixelPos.x + vec.x, pixelPos.y + vec.y);
						glEnd();

						//ofCircle(pixelPos.x, pixelPos.y, 2);
					}
				}

			glPopMatrix();
		}


	ofVec2f mfield[NUM_BINS_X][NUM_BINS_Y];
	float binW, binH;


};

#endif

