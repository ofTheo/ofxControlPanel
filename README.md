ofxControlPanel
===============
by Theo Watson, Nick Hardeman 

Licensed under: https://opensource.org/licenses/MIT 

New:
- OF 0.10.1 support
- Only uses ofParameter now. The easiest usage is to create ofParameterGroups for a set of ofParameters and then add the group to the control panel. You can also add individual parameters as well
- Added a color picker 
- Added an example which you should be able to run from the `addons/ofxControlPanel/example` folder showing how the parameter stuff works. 
- Lock panel, now shows an overlay to make it clear its locked. 

Features:
- ofBaseDraws display area ( for showing video / images / FBOs etc ) 
- video player with controls 
- multiple tabbed panels 
- color picker
- sliders of course! 
- multi toggle 
- drop down selection 
- file lister 
- file loader
- labels and variable listers ( for just displaying values ) 
- range slider 
- 2D slider 
- chart plotter 
- text input 
- status bar for showing messages 
- guiTypeCustom for implementing your own gui item for both drawing, mouse input and save/load to xml 


Quick example: 
```
    //ofApp.h 
    ofxControlPanel gui; 
    ofVideoGrabber vid; 

    ofParameterGroup cvControls; //this will be in your header file 
    ofParameter <bool> bInvert; 
    ofParameter <float> threshold; 
    
    //ofApp.cpp 
    //In ofApp::setup()
    gui.setup("my gui"); 
  
    //add a video to the panel   
    gui.addDrawableRect("video feed", &vid, 320, 240); 
    
    cvControls.setName("cv controls");
    cvControls.add(bInvert.set("invert", false));
    cvControls.add(threshold.set("threshold", 29.0, 1.0, 255.0));
        
    //now add the param group to the gui 
    gui.add( cvControls ); 
    
    gui.loadSettings("settings.xml"); 
    
    //In ofApp::update(){
    gui.update(); 
    
    //In ofApp::draw(){
    gui.draw(); 
```
