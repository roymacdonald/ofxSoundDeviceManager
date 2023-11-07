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
#include "ofxPrerecordedInput.h"


enum ofxSoundDeviceManagerMode{
    OFX_SOUND_MANAGER_NO_INPUT = 0,
    OFX_SOUND_MANAGER_LIVE_INPUT = 1,
    OFX_SOUND_MANAGER_FILE_INPUT = 2
};


class ofxSoundDeviceManager{
public:
    
    
    void setup(ofxSoundDeviceManagerMode mode);
    bool load(string folderPath, bool stream = false);
    bool loadAsync(std::filesystem::path folderPath, bool bAutoplay);
    void saveSettings();
    void loadSettings();
    
    void draw();
    
    
    
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
    
    
//    ofxSoundInput input;
    
    ofxSoundObject * getInput();
    
    ofxSoundDeviceManagerMode getMode(){return _mode;}
    
    ofxSoundInput* getLiveInput();
    ofxPrerecordedInput* getPrerecordedInput();
    
protected:
    
    
    unique_ptr<ofxSoundInput> inputLive = nullptr;
    unique_ptr<ofxPrerecordedInput> inputPrerecorded = nullptr;
    
    void _makeBufferSizes();
    
    void setStream();
private:

    ofxSoundDeviceManagerMode _mode = OFX_SOUND_MANAGER_NO_INPUT;
    
    bool _bSetup = false;
    
};
