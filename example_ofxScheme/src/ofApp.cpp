#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // OF STUFF
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableAntiAliasing();

    // INIT Scheme and register API
    scheme.setup();

    // init drawing FBO
    fbo = new ofFbo();
    fbo->allocate(ofGetWindowWidth(),ofGetWindowHeight(),GL_RGBA32F_ARB,4);
    fbo->begin();
    ofClear(0,0,0,255);
    fbo->end();

    // open default script
    filepath = ofToDataPath("sketch.scm",true);
    watcher.start();
    needToLoadScript = true;
    scriptLoaded = false;
}

//--------------------------------------------------------------
void ofApp::update(){

    if(needToLoadScript){
        needToLoadScript = false;
        loadScript(filepath);
    }

    while(watcher.waitingEvents()) {
        pathChanged(watcher.nextEvent());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(scriptLoaded){
        fbo->begin();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
        ofPushView();
        ofPushStyle();
        ofPushMatrix();

        scheme.evalScript(sketchContent.getText());

        ofPopMatrix();
        ofPopStyle();
        ofPopView();
        glPopAttrib();
        fbo->end();

        ofSetColor(255);
        fbo->draw(0,0,fbo->getWidth(),fbo->getHeight());
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void ofApp::pathChanged(const PathWatcher::Event &event) {
    switch(event.change) {
    case PathWatcher::CREATED:
        //ofLogVerbose(PACKAGE) << "path created " << event.path;
        break;
    case PathWatcher::MODIFIED:
        //ofLogVerbose(PACKAGE) << "path modified " << event.path;
        filepath = event.path;
        needToLoadScript = true;
        scriptLoaded = false;
        break;
    case PathWatcher::DELETED:
        //ofLogVerbose(PACKAGE) << "path deleted " << event.path;
        return;
    default: // NONE
        return;
    }

}

//--------------------------------------------------------------
void ofApp::loadScript(string scriptFile){
    currentScriptFile.open(scriptFile);
    sketchContent = ofBufferFromFile(currentScriptFile.getAbsolutePath());

    watcher.removeAllPaths();
    watcher.addPath(scriptFile);

    scriptLoaded = true;
}
