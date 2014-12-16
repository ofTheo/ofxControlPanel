ofxControlPanel
===============

Added an example which you should be able to run from the `addons/ofxControlPanel/example` folder showing how the parameter stuff works. 

The easiest usage is to create ofParameterGroups for a set of ofParameters and then add the group to the control panel. 
You can also add individual parameters though instead. 

Quick example: 
```
    ofParameterGroup cvControls; //this will be in your header file 
    ofParameter <bool> bInvert; 
    ofParemeter <float> threshold; 
    
    //then in setup 
    cvControls.setName("cv controls");
    cvControls.add(bInvert.set("invert", false));
    cvControls.add(threshold.set("threshold", 29.0, 1.0, 255.0));
    
    //now add the param group to the gui 
    gui.addGroup( cvControls ); 
```
