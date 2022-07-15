//
//  ofxSoundDeviceParams.hpp
//  audioInputManager
//
//  Created by Roy Macdonald on 15-07-22.
//

#pragma once
#include "ofMain.h"
#include "ofxSoundObjects.h"
#include "ofxGui.h"
#include "ofxDropdown.h"


class ofxSoundDeviceManager;



class ofxSoundDeviceParams{
public:
    ofxSoundDeviceParams(bool _bIsInput);

    void setup(ofxSoundDeviceManager * _manager);
    
    void deviceChanged(int& deviceID);
    

    void channelsChanged(unsigned int& c);
    
    void updateSettings();
    
    
    vector<ofSoundDevice> devices;
    ofSoundDevice currentDevice;
    ofxDropdown_<int> device = {"device"} ;
    
    ofxDropdown_<unsigned int> channels = {"num channels"} ;
    
    bool bIsInput = false;
    ofxGuiGroup group;
    
    
    ofxSoundDeviceManager * manager = nullptr;
    
};
