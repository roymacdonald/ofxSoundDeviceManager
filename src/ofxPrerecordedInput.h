//
//  ofxPrerecordedInput.hpp
//  example
//
//  Created by Roy Macdonald on 15-07-22.
//

#pragma once
#include "ofMain.h"
#include "ofxSoundObjects.h"
#include <atomic>

class ofxPrerecordedInput: public ofxBaseSoundPlayer{
public:
    ofxPrerecordedInput(){
        audioOutCount = 0;
    }
    
    virtual bool load(const ofSoundBuffer& loadBuffer, const std::string& name) override;
    
    virtual bool load(std::filesystem::path filePath, bool stream = false) override;
    virtual bool loadAsync(std::filesystem::path filePath, bool bAutoplay) override;
    
    virtual void unload() override;
    
    virtual float getPosition() const override;
    virtual int   getPositionMS() const override;
    virtual bool  isPlaying() const override;
    virtual float getSpeed() const override;
    virtual float getRelativeSpeed() const override;
    virtual float getPan() const override;
    virtual bool  isLoaded() const override;
    virtual float getVolume() const override;
    virtual bool  isLooping() const override;
    virtual unsigned long getDurationMS() override;
    virtual bool isReplaying() const override;
    virtual int getSourceSampleRate() const override;
    virtual size_t getNumFrames() const override;
    
    virtual std::string getFilePath() const override;
    
    void play();
    void stop();
    
    virtual void audioOut(ofSoundBuffer &output) override;
    
    string getCurrentStats();

private:
    bool _loadHelper(std::filesystem::path filePath, bool stream, bool bAutoPlay, bool bAsync);
    //when puting ofxSoundPlayerObjects into a vector you need to do so using shared_ptr or unique_ptr
    vector<shared_ptr<ofxSoundPlayerObject>> players;
    std::filesystem::path _filePath;
    
    ofEventListeners asyncLoadListeners;
    enum AsyncLoadState{
        ASYNC_LOAD_UNLOADED,
        ASYNC_LOAD_SUCCESS,
        ASYNC_LOAD_FAIL
    };
    std::map<string, AsyncLoadState> asyncFileLoaded;
  
    atomic<size_t> audioOutCount;
    atomic<size_t> numOutChannels;
    
    
};
