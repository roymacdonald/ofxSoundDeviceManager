//
//  ofxSoundDeviceManager.hpp
//  audioInputManager
//
//  Created by Roy Macdonald on 14-07-22.
//

#pragma once
#include "ofMain.h"
#include "ofxSoundObjects.h"
#include "ofxGui.h"
#include "ofxDropdown.h"
#include "ofxSoundDeviceParams.h"



class ofxSoundDeviceManager{
public:
    void setup(bool enableInput);
    void draw();
    
    
    ofxSoundInput input;
    ofxSoundOutput output;
    ofSoundStream stream;
    ofSoundStreamSettings settings;

    ofxPanel gui;
    unique_ptr<ofxSoundDeviceParams> inputParams = nullptr;
    unique_ptr<ofxSoundDeviceParams> outputParams = nullptr;

    ofParameter<void> setupStream = {"Setup Stream"};
    ofEventListeners listeners;
    
    ofxDropdown_<unsigned int> sampleRates = {"sampleRates"} ;
    ofxDropdown_<unsigned int> bufferSize = {"Buffer Size"};
    ofParameter<unsigned int> numBuffers = {"Num Buffers", 1, 1, 8};
    
    void updateSampleRates();
    
protected:
    
    void _makeBufferSizes();
    
    void setStream();
private:

    bool _enableInput = false;
    bool _bSetup = false;
    
};
