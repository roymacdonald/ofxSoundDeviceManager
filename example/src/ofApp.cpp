#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    
//    auto mode = OFX_SOUND_MANAGER_LIVE_INPUT;
    auto mode = OFX_SOUND_MANAGER_FILE_INPUT;
//    auto mode = OFX_SOUND_MANAGER_NO_INPUT;
    
    
    deviceManage.setup(mode);
    
    if(mode == OFX_SOUND_MANAGER_FILE_INPUT){
        auto r = ofSystemLoadDialog("Select folder with wav  files", true);
        if(r.bSuccess){
            if(deviceManage.load(r.getPath())){
                cout << " LOADED: " << r.getPath() << endl;
                deviceManage.getPrerecordedInput()->play();
            }
        }
    }
    
    wave.setup(0, 0, ofGetWidth(), ofGetHeight());
    
    auto input = deviceManage.getInput();
    if(input != nullptr){
        input->connectTo(wave);
    }
    
    wave.connectTo(deviceManage.output);
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    wave.draw();
    
    auto stats =  deviceManage.getPrerecordedInput()->getCurrentStats();
    ofDrawBitmapStringHighlight(stats , 20, 20);
    
    
    deviceManage.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        auto i = deviceManage.getPrerecordedInput();
        if(i){
            if(i->isPlaying()){
                i->stop();
            }else{
                i->play();
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
