//	I just put this here because I couldn't be bothered to update all the files with
//	the new way of using Addons yet. Pretty sure this isn't causing the problem.

//-------------------------------------------- openCV
#ifdef OF_ADDON_USING_OFXOPENCV
	#include "ofxCvMain.h"
#endif

//-------------------------------------------- obj loader
#ifdef OF_ADDON_USING_OFXOBJLOADER
	#include "ofxObjLoader.h"
#endif

//-------------------------------------------- dir list
#ifdef OF_ADDON_USING_OFXDIRLIST
	#include "ofxDirList.h"
#endif

//-------------------------------------------- vector math
#ifdef OF_ADDON_USING_OFXVECTORMATH
	#include "ofxVectorMath.h"
#endif

//-------------------------------------------- network
#ifdef OF_ADDON_USING_OFXNETWORK
	#include "ofxNetwork.h"
#endif

//-------------------------------------------- vector graphics
#ifdef OF_ADDON_USING_OFXVECTORGRAPHICS
	#include "ofxVectorGraphics.h"
#endif

//-------------------------------------------- OSC
#ifdef OF_ADDON_USING_OFXOSC
	#include "ofxOsc.h"
#endif

//-------------------------------------------- ofthread
#ifdef OF_ADDON_USING_OFXTHREAD
	#include "ofxThread.h"
#endif

//-------------------------------------------- ofXML
#ifdef OF_ADDON_USING_OFXXMLSETTINGS
	#include "ofxXmlSettings.h"
#endif
