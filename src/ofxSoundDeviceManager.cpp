//
//  ofxSoundDeviceManager.cpp
//  audioInputManager
//
//  Created by Roy Macdonald on 14-07-22.
//

#include "ofxSoundDeviceManager.h"

void ofxSoundDeviceManager::setup(bool enableInput){
    if(_bSetup){
        ofLogWarning("ofxSoundDeviceManager::setup") << "is already setup.";
        return;
    }
    gui.setup("Sound Devices", "sound_devices.json");

    sampleRates.disableMultipleSelection();
    sampleRates.enableCollapseOnSelection();
    
    bufferSize.disableMultipleSelection();
    bufferSize.enableCollapseOnSelection();
    
    _enableInput = enableInput;
    settings.bufferSize = 256;
    settings.numBuffers = 1;
    
    _makeBufferSizes();
    bufferSize.selectedValue = settings.bufferSize;
    
    if(_enableInput){
        inputParams = make_unique<ofxSoundDeviceParams>(true);
        inputParams->setup(this);
    }
    outputParams = make_unique<ofxSoundDeviceParams>(false);
    outputParams->setup(this);

    
    
    gui.add(&sampleRates);
    gui.add(&bufferSize);
    gui.add(numBuffers);
    
    gui.add(setupStream);
    listeners.push( setupStream.newListener(this, &ofxSoundDeviceManager::setStream));
    
    if(ofFile::doesFileExist("sound_devices.json")){
        gui.loadFromFile("sound_devices.json");
        setStream();
    }
    _bSetup = true;
    
}

void ofxSoundDeviceManager::_makeBufferSizes(){
    unsigned int v = 64;
    for(int i = 0; i < 6; i++){
        bufferSize.add(v);
        v *= 2;
    }
    
}

void ofxSoundDeviceManager::updateSampleRates(){
    sampleRates.clear();
    if(!_enableInput){
        sampleRates.add(outputParams->currentDevice.sampleRates);
    }else{
        auto & o_sr = outputParams->currentDevice.sampleRates;
        auto & i_sr = inputParams->currentDevice.sampleRates;
        std::set<unsigned int> merged_SR;
        for(auto&i:  i_sr){
            if(merged_SR.count(i) == 0){
                merged_SR.emplace(i);
            }
        }
        for(auto&i:  o_sr){
            if(merged_SR.count(i) == 0){
                merged_SR.emplace(i);
            }
        }
        std::vector<unsigned int> merged (merged_SR.begin(), merged_SR.end());
        
        ofSort(merged);
        
        sampleRates.add(merged);
        
    }
    
}

void ofxSoundDeviceManager::setStream(){
    if(_enableInput && inputParams) inputParams->updateSettings();
    if(outputParams)outputParams->updateSettings();
    settings.bufferSize = bufferSize.selectedValue.get();
    settings.sampleRate = sampleRates.selectedValue.get();
    settings.numBuffers = numBuffers.get();
    stream.stop();
    stream.close();
    stream.setup(settings);
    stream.setOutput(output);
    if(_enableInput){
        stream.setInput(input);
    }
}

void ofxSoundDeviceManager::draw(){
    gui.draw();
}
