//
//  ofxSoundDeviceManager.cpp
//  audioInputManager
//
//  Created by Roy Macdonald on 14-07-22.
//

#include "ofxSoundDeviceManager.h"

void ofxSoundDeviceManager::setup(){
    if(_bSetup){
        ofLogWarning("ofxSoundDeviceManager::setup") << "is already setup.";
        return;
    }

    
    gui.setup("Sound Devices", "sound_devices.json");

    sampleRates.disableMultipleSelection();
    sampleRates.enableCollapseOnSelection();
    
    bufferSize.disableMultipleSelection();
    bufferSize.enableCollapseOnSelection();
    
    
    settings.bufferSize = 256;
    settings.numBuffers = 1;
    
    _makeBufferSizes();
    bufferSize.selectedValue = settings.bufferSize;
    
        inputLive = make_unique<ofxSoundInput>();
        
        inputParams = make_unique<ofxSoundDeviceParams>(true);
        inputParams->setup(this);
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

void ofxSoundDeviceManager::setStream(){
    if(inputParams) inputParams->updateSettings();
    if(outputParams)outputParams->updateSettings();
    
    settings.bufferSize = bufferSize.selectedValue.get();
    settings.sampleRate = sampleRates.selectedValue.get();
    settings.numBuffers = numBuffers.get();
    stream.stop();
    stream.close();
    stream.setup(settings);
    stream.setOutput(output);
    if(inputLive){
        stream.setInput(*inputLive);
    }
}

void ofxSoundDeviceManager::draw(){
    gui.draw();
}

ofxSoundObject * ofxSoundDeviceManager::getInput(){
//    if(_mode == OFX_SOUND_MANAGER_LIVE_INPUT ){
        return inputLive.get();
//    }else if(_mode == OFX_SOUND_MANAGER_FILE_INPUT ){
//        return inputPrerecorded.get();
//    }
//    return nullptr;
}

bool isFolder(string folderPath){
    ofFile f(folderPath);
    if(f.exists()){
        return f.isDirectory();
    }
    return false;
}

bool folderCheck(string folderPath, string msgHeader){
    if(!isFolder(folderPath)){
        ofLogWarning(msgHeader) << "folderPath does not point to a folder";
        return false;
    }
    return true;
}

//bool ofxSoundDeviceManager::load(string folderPath, bool stream){
//
//    if(!folderCheck(folderPath, "ofxSoundDeviceManager::load")){ return false; }
//
//    if(_mode == OFX_SOUND_MANAGER_FILE_INPUT){
//        if(inputPrerecorded){
//            return inputPrerecorded->load(folderPath, stream);
//        }
//        return false;
//    }
//    ofLogWarning("ofxSoundDeviceManager::load") << "you should no call load function when mode is other than OFX_SOUND_MANAGER_FILE_INPUT";
//    return false;
//}
//
//bool ofxSoundDeviceManager::loadAsync(std::filesystem::path folderPath, bool bAutoplay){
//    if(!folderCheck(folderPath, "ofxSoundDeviceManager::loadAsync")){ return false; }
//    if(_mode == OFX_SOUND_MANAGER_FILE_INPUT){
//        if(inputPrerecorded){
//            return inputPrerecorded->loadAsync(folderPath, bAutoplay);
//        }
//        return false;
//    }
//    ofLogWarning("ofxSoundDeviceManager::load") << "you should no call load function when mode is other than OFX_SOUND_MANAGER_FILE_INPUT";
//    return false;
//}

ofxSoundInput* ofxSoundDeviceManager::getLiveInput(){
    if(!inputLive)
    { return nullptr; }
    return  inputLive.get();
}
//ofxPrerecordedInput* ofxSoundDeviceManager::getPrerecordedInput(){
//    if(!inputPrerecorded){
//        return nullptr;}
//    return  inputPrerecorded.get();
//}
