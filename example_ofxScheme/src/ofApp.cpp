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
    fbo->allocate(ofGetScreenWidth(),ofGetScreenHeight() ,GL_RGBA32F_ARB,4);
    fbo->begin();
    ofClear(0,0,0,255);
    fbo->end();

    isFullscreen = false;
    thposX = thposY = thdrawW = thdrawH = 0.0f;
    scaleTextureToWindow(fbo->getWidth(), fbo->getHeight(), 1280, 720);
    scheme.setWindowDim(fbo->getWidth(), fbo->getHeight());

    // load editor font
    ofxEditor::loadFont("fonts/PrintChar21.ttf", 24);
    hideEditor = false;
    // load scheme syntax
    syntax.loadFile("schemeSyntax.xml");
    editor.getSettings().addSyntax(&syntax);
    // syntax highlighter colors
    //colorScheme.loadFile("colorScheme.xml");
    //editor.setColorScheme(&colorScheme);

    // open default script
    filepath = ofToDataPath("sketch.scm",true);
    watcher.start();
    needToLoadScript = true;
    scriptLoaded = false;

    eval = true;
}

//--------------------------------------------------------------
void ofApp::update(){

    scheme.setMouse((ofGetMouseX() - thposX)/thdrawW * fbo->getWidth(),(ofGetMouseY() - thposY)/thdrawH * fbo->getHeight());
    scheme.update();

    if(needToLoadScript){
        needToLoadScript = false;
        loadScript(filepath);
    }

    if(eval){
        eval = false;
        scriptBuffer = editor.getText();
    }

    while(watcher.waitingEvents()) {
        pathChanged(watcher.nextEvent());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    if(scriptLoaded){
        fbo->begin();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
        ofPushView();
        ofPushStyle();
        ofPushMatrix();

        scheme.evalScript(scriptBuffer);

        ofPopMatrix();
        ofPopStyle();
        ofPopView();
        glPopAttrib();
        fbo->end();

        ofSetColor(255);
        fbo->draw(thposX,thposY,thdrawW,thdrawH);

        if(!hideEditor) {
            editor.draw();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    bool modifierPressed = ofxEditor::getSuperAsModifier() ? ofGetKeyPressed(OF_KEY_SUPER) : ofGetKeyPressed(OF_KEY_CONTROL);
    if(modifierPressed) {
        switch(key) {
        case 'e':
            scheme.clearScript();
            eval = true;
            return;
        case 'f':
            toggleWindowFullscreen();
            return;
        case 'l':
            editor.setLineWrapping(!editor.getLineWrapping());
            return;
        case 'n':
            editor.setLineNumbers(!editor.getLineNumbers());
            return;
        case 't':
            hideEditor = !hideEditor;
            return;
        case 'k':
            editor.setAutoFocus(!editor.getAutoFocus());
            return;
        }
    }

    // send regular key pressed to script if the editor is hidden
    if(!hideEditor) {
        editor.keyPressed(key);
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
void ofApp::windowResized(int w, int h){
    editor.resize(w, h);
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

    scheme.clearScript();
    scheme.setScriptPath(scriptFile);

    currentScriptFile.open(scriptFile);
    sketchContent = ofBufferFromFile(currentScriptFile.getAbsolutePath());

    watcher.removeAllPaths();
    watcher.addPath(scriptFile);

    // open script file into editor
    editor.openFile(scriptFile);

    scriptLoaded = true;
}

//--------------------------------------------------------------
void ofApp::scaleTextureToWindow(float texW, float texH, float winW, float winH){
    // wider texture than window
    if(texW/texH >= winW/winH){
        thdrawW           = winW;
        thdrawH           = (texH*winW) / texW;
        thposX            = 0;
        thposY            = (winH-thdrawH)/2.0f;
        //ofLog(OF_LOG_NOTICE," |wider texture than window|  Window: %fx%f, Texture[%fx%f] drawing %fx%f at %f,%f",winW,winH,texW,texH,thdrawW,thdrawH,thposX,thposY);
    // wider window than texture
    }else{
        thdrawW           = (texW*winH) / texH;
        thdrawH           = winH;
        thposX            = (winW-thdrawW)/2.0f;
        thposY            = 0;
        //ofLog(OF_LOG_NOTICE," |wider window than texture|  Window: %fx%f, Texture[%fx%f] drawing %fx%f at %f,%f",winW,winH,texW,texH,thdrawW,thdrawH,thposX,thposY);
    }

}

//--------------------------------------------------------------
void ofApp::toggleWindowFullscreen(){
    isFullscreen = !isFullscreen;
    ofToggleFullscreen();

    if(!isFullscreen){
        scaleTextureToWindow(fbo->getWidth(), fbo->getHeight(), 1280,720);
    }else{
        scaleTextureToWindow(fbo->getWidth(), fbo->getHeight(), ofGetScreenWidth(),ofGetScreenHeight());
    }
}
