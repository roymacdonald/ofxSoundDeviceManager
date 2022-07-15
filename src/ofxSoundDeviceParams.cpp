//
//  ofxSoundDeviceParams.cpp
//  audioInputManager
//
//  Created by Roy Macdonald on 15-07-22.
//

#include "ofxSoundDeviceParams.h"
#include "ofxSoundDeviceManager.h"

ofxSoundDeviceParams::ofxSoundDeviceParams(bool _bIsInput):bIsInput(_bIsInput)
{
    device.disableMultipleSelection();
    device.enableCollapseOnSelection();
    
    channels.disableMultipleSelection();
    channels.enableCollapseOnSelection();
    
}
void ofxSoundDeviceParams::setup(ofxSoundDeviceManager * _manager){
    manager = _manager;
    if(bIsInput){
        devices = ofxSoundUtils::getInputSoundDevices();
    }else{
        devices = ofxSoundUtils::getOutputSoundDevices();
    }
    
    device.clear();
//    sampleRates.clear();
    channels.clear();
    for(auto& dev: devices){
        stringstream ss;
        ss << dev.deviceID << " - " << dev.name;
        ss << " in";
        if(dev.isDefaultInput) ss << "*";
        ss << ":" << dev.inputChannels;
        ss << " out";
        if(dev.isDefaultOutput) ss << "*";
        ss << ":" << dev.outputChannels << "  ";
        
        device.add(dev.deviceID, ss.str());
        device.deselect();
    }
    
    device.addListener(this, &ofxSoundDeviceParams::deviceChanged);
//    sampleRates.addListener(this, &ofxSoundDeviceParams::sampleRatesChanged);
    channels.addListener(this, &ofxSoundDeviceParams::channelsChanged);
    
    group.setup(bIsInput?"Input":"Output");
    group.add(&device);
//    group.add(&sampleRates);
    group.add(&channels);
    if(manager){
        manager->gui.add(&group);
    }
    
}

void ofxSoundDeviceParams::deviceChanged(int& deviceID){
    bool bFound = false;
    size_t found_index;
    for(size_t i = 0; i < devices.size(); i ++){
        if(devices[i].deviceID == deviceID){
            bFound = true;
            found_index = i;
            break;
        }
    }
    if(!bFound){
        ofLogWarning("ofxSoundDeviceParams::deviceChanged") << deviceID << " does not exist";
    }else{
        currentDevice = devices[found_index];

        channels.clear();
        auto chans = bIsInput?currentDevice.inputChannels:currentDevice.outputChannels;
        for(unsigned int i = 0; i < chans; i++){
            channels.add( i + 1 );
        }
        
        
        if(manager){
            manager->updateSampleRates();
        }
        
    }
    
}

void ofxSoundDeviceParams::channelsChanged(unsigned int& c){

}

void ofxSoundDeviceParams::updateSettings(){
    if(manager){
        if(bIsInput){
            manager->settings.setInDevice(currentDevice);
            manager->settings.numInputChannels = channels.selectedValue.get();
            
        }else{
            manager->settings.setOutDevice(currentDevice);
            manager->settings.numOutputChannels = channels.selectedValue.get();
        
        }
        
//        managersettings-> sampleRate = sampleRates.selectedValue.get();

    }
}

