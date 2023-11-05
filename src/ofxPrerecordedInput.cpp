//
//  ofxPrerecordedInput.cpp
//  example
//
//  Created by Roy Macdonald on 15-07-22.
//

#include "ofxPrerecordedInput.h"



bool ofxPrerecordedInput::load(const ofSoundBuffer& loadBuffer, const std::string& name) {
    getBuffer() = loadBuffer;
    setName(name);
    return true;
}
bool ofxPrerecordedInput::_loadHelper(std::filesystem::path filePath, bool stream, bool bAutoPlay, bool bAsync) {
    bool bSuccess = false;
    ofFile f(filePath);
    if(f.isDirectory()){
        _filePath = filePath;
        ofDirectory dir(filePath);
        dir.allowExt("wav");
        dir.allowExt("aiff");
        dir.allowExt("mp3");
        dir.listDir();
        dir.sort();
        players.resize(dir.size());

        //this example will connect each sound player to a consecutive sound output.
        for (int i = 0; i < dir.size(); i++) {
            players[i] = make_shared<ofxSoundPlayerObject>();
            bool bLoaded = false;
            if(bAsync){
                bLoaded = players[i]->loadAsync(dir.getPath(i), bAutoPlay);
            }else{
                bLoaded = players[i]->load(dir.getPath(i), stream);
            }
            if(bLoaded){
//                    auto chans = players[i]->getSoundFile().getNumChannels();
                //create a vector<int> object that will define which are the channels you want to connect to
                //
                bSuccess = true;
                
                // the next line is just a regular way of connecting sound objects.
//                    players[i]->connectTo();
                players[i]->play();
                players[i]->setLoop(true);
            }
        }
        
    }
    
    return bSuccess;
}
bool ofxPrerecordedInput::load(std::filesystem::path filePath, bool stream) {
    return _loadHelper(filePath, stream, false, false) ;
}
bool ofxPrerecordedInput::loadAsync(std::filesystem::path filePath, bool bAutoplay) {
    return _loadHelper(filePath, false, bAutoplay, true) ;
}
void ofxPrerecordedInput::unload() {
    for(auto& p: players){
        p->unload();
    }
}
float ofxPrerecordedInput::getPosition() const {
    if(players.size() > 0){
        return players[0]->getPosition();
    }
    return 0.0f;
}
int   ofxPrerecordedInput::getPositionMS() const {
    if(players.size() > 0){
        return players[0]->getPositionMS();
    }
    return 0;
}
bool  ofxPrerecordedInput::isPlaying() const {
    if(players.size() > 0){
        return players[0]->isPlaying();
    }
    return false;
}
float ofxPrerecordedInput::getSpeed() const {
    if(players.size() > 0){
        return players[0]->getSpeed();
    }
    return 0.0f;
}
float ofxPrerecordedInput::getRelativeSpeed() const {
    if(players.size() > 0){
        return players[0]->getRelativeSpeed();
    }
    return 0.0f;
}
float ofxPrerecordedInput::getPan() const {
    if(players.size() > 0){
        return players[0]->getPan();
    }
    return 0.0f;
}
bool  ofxPrerecordedInput::isLoaded() const {
    if(players.size() > 0){
        return players[0]->isLoaded();
    }
    return false;
}
float ofxPrerecordedInput::getVolume() const {
    if(players.size() > 0){
        return players[0]->getVolume();
    }
    return 0;
}
bool  ofxPrerecordedInput::isLooping() const {
    if(players.size() > 0){
        return players[0]->isLooping();
    }
    return false;
}
unsigned long ofxPrerecordedInput::getDurationMS() {
    if(players.size() > 0){
        return players[0]->getDurationMS();
    }
    return 0;
}
bool ofxPrerecordedInput::isReplaying() const {
    if(players.size() > 0){
        return players[0]->isReplaying();
    }
    return false;
}
int ofxPrerecordedInput::getSourceSampleRate() const {
    if(players.size() > 0){
        return players[0]->getSourceSampleRate();
    }
    return 0;
}
size_t ofxPrerecordedInput::getNumFrames() const {
    if(players.size() > 0){
        return players[0]->getNumFrames();
    }
    return 0;
}
std::string ofxPrerecordedInput::getFilePath() const {
    return _filePath;
}

void ofxPrerecordedInput::play(){
    for(auto& p : players){
        p->play();
    }
}
void ofxPrerecordedInput::stop(){
    for(auto& p : players){
        p->stop();
    }
}

void ofxPrerecordedInput::audioOut(ofSoundBuffer &output) {
    ofxSoundUtils::checkBuffers(output, getBuffer());
    auto & buf = getBuffer();
    buf.setSampleRate(output.getSampleRate());
    buf.setNumChannels(1);
    buf.allocate(output.getNumFrames(), 1);
    numOutChannels = players.size();
    output.allocate(output.getNumFrames(), numOutChannels);
    
    
//    audioOutCount = audioOutCount +1;
   
//    output.setNumChannels(players.size());
    float _gain = gain.load();
    for(size_t i = 0; i < numOutChannels; i++){
        players[i]->audioOut(buf);
        buf *= _gain;
        output.setChannel(buf, i);
    }
    
//    cout << "-";
    
//    output = buf;
    
}

void ofxPrerecordedInput::setGain(float _gain){
    gain = _gain;
}

string ofxPrerecordedInput::getCurrentStats(){
    stringstream ss;
    ss << "ofxPrerecordedInput" << endl;
    ss << "    Num players " << players.size() << endl;
    ss << "    audioOutCount: " << audioOutCount << endl;
    ss << "    numOutChannels: " << numOutChannels << endl;
    
    
    for(size_t i = 0; i < players.size(); i++){
        ss << i << " : ";
        ss << "frames: " << players[i]->getNumFrames() << " ";
        ss << "channels: " << players[i]->getNumChannels() << " ";
        ss << "playing: " << std::boolalpha << players[i]->isPlaying() << " ";
        ss << "position : " << players[i]->getPosition() << endl;
    }
    
    
    return ss.str();
}

void ofxPrerecordedInput::setPositionMS(int ms){
    for(auto& p : players){
        p->setPositionMS(ms);
    }
}

void ofxPrerecordedInput::getPlayersCombinedBuffer(ofSoundBuffer & outBuffer)
{
    outBuffer.clear();
    
    outBuffer.setNumChannels(players.size());
    outBuffer.allocate(players[0]->getNumFrames(), players.size());
    for(size_t i = 0; i < players.size(); i++){
        outBuffer.setChannel(players[i]->getSoundFile().getBuffer(), i);
    }
    outBuffer *= gain.load();
    outBuffer.setSampleRate(players[0]->getSourceSampleRate());

}
size_t ofxPrerecordedInput::getNumChannels(){
    size_t c = 0;
    for(auto& p : players){
        c += p->getNumChannels();
    }
    return c;
}
