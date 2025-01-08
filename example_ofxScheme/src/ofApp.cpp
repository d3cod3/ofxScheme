#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // OF STUFF
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableAntiAliasing();
    ofSetEscapeQuitsApp(false);

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
    editor.setup(this);

    hideEditor = false;
    // load scheme syntax
    syntax = new ofxEditorSyntax();
    syntax->loadFile("schemeSyntax.xml");
    editor.getSettings().addSyntax("Scheme", syntax);
    for(size_t i =0;i<=9;i++){
        editor.setLangSyntax("Scheme",i);
    }
    // syntax highlighter colors
    colorScheme.loadFile("colorScheme.xml");
    editor.setColorScheme(&colorScheme);
    cursorColor = ofColor(255, 255, 0, 200);

    ofxSchemeAPI = {"loop","MOUSE_X","MOUSE_Y","OUTPUT_WIDTH","OUTPUT_HEIGHT",
                    "background","background-alpha","set-color","fill","no-fill",
                    "push","pop","begin-shape","end-shape","translate","rotate",
                    "scale","line-width","curve-res","draw-vertex","draw-line","draw-curve",
                    "draw-bezier","draw-circle","draw-ellipse","draw-rectangle",
                    "draw-triangle","draw-cube","draw-cone","draw-cylinder","draw-plane",
                    "draw-sphere","draw-string","clamp","deg-to-rad","rad-to-deg",
                    "dist","lerp","map","noise","draw-image","draw-video","draw-camera","draw-screen"};
    ofxSchemeAPIDesc.assign(ofxSchemeAPI.size(),"");
    ofxSchemeAPIDesc[0] = "(loop index  start  end increment)  -->  use loop as you would use every iterative structure ( for, while, etc... )";
    ofxSchemeAPIDesc[1] = "(MOUSE_X)  -->  Returns Mouse X position on screen";
    ofxSchemeAPIDesc[2] = "(MOUSE_Y)  -->  Returns Mouse Y position on screen";
    ofxSchemeAPIDesc[3] = "(OUTPUT_WIDTH)  -->  Returns Output Texture width ( horizontal resolution )";
    ofxSchemeAPIDesc[4] = "(OUTPUT_HEIGHT)  -->  Returns Output Texture height ( vertical resolution )";
    ofxSchemeAPIDesc[5] = "(background r g b)  -->  set background color in RGB ( values range [0.0 - 1.0] )";
    ofxSchemeAPIDesc[6] = "(background-alpha r g b a)  -->  set background color in RGBA  ( values range [0.0 - 1.0] )";
    ofxSchemeAPIDesc[7] = "(set-color r g b) | (set-color r g b a)  -->  set current drawing color in RGB or RGBA  ( values range [0.0 - 1.0] )";
    ofxSchemeAPIDesc[8] = "(fill)  -->  set fill drawing geometry";
    ofxSchemeAPIDesc[9] = "(no-fill)  -->  set no fill ( only draw perimeters ) drawing geometry";
    ofxSchemeAPIDesc[10] = "(push)  --> push matrix";
    ofxSchemeAPIDesc[11] = "(pop)  --> pop matrix";
    ofxSchemeAPIDesc[12] = "(begin-shape)  --> start polygon drawing using vertexes ( draw-vertex )";
    ofxSchemeAPIDesc[13] = "(end-shape)  --> close polygon drawing using vertexes ( draw-vertex )";
    ofxSchemeAPIDesc[14] = "(translate x y z)  --> translate current matrix point-of-view";
    ofxSchemeAPIDesc[15] = "(rotate x y z)  --> rotate current matrix point-of-view";
    ofxSchemeAPIDesc[16] = "(scale x y z)  --> scale current matrix point-of-view";
    ofxSchemeAPIDesc[17] = "(line-width width)  --> set drawing lines width";
    ofxSchemeAPIDesc[18] = "(curve-res res)  --> set drawing curves resolution";
    ofxSchemeAPIDesc[19] = "(draw-vertex x y z)  --> draw a vertex at the specified position ( it should be used between begin-shape - end-shape )";
    ofxSchemeAPIDesc[20] = "(draw-line x1 y1 x2 y2) | (draw-line x1 y1 x2 y2 width)  --> draw a line betweeen the two specified points, with the specified line width";
    ofxSchemeAPIDesc[21] = "(draw-curve x1  y1 x2 y2 x3 y3 x4 y4)  --> draw a curve betweeen the two specified points ( x2 and x3 ), shaped by the control points ( x1 and x4 )";
    ofxSchemeAPIDesc[22] = "(draw-bezier x1 y1 x2 y2 x3 y3 x4 y4)  --> draw a bexier curve betweeen the two specified points ( x2 and x3 ), shaped by the control points ( x1 and x4 )";
    ofxSchemeAPIDesc[23] = "(draw-circle x y radio res)  --> draw a circle at the specified position, with the specified radio and resolution";
    ofxSchemeAPIDesc[24] = "(draw-ellipse x y radioX radioY res)  --> draw an ellipse at the specified position, with the specified radios and resolution";
    ofxSchemeAPIDesc[25] = "(draw-rectangle x y w h) | (draw-rectanglex x y w h rounding)  --> draw a rectangle at the specified position, with the specified width and height and rounding";
    ofxSchemeAPIDesc[26] = "(draw-triangle x1 y1 x2 y2 x3 y3)  --> draw a triangle between the specified points";
    ofxSchemeAPIDesc[27] = "(draw-cube size res) --> draw a cube at origin position( 0, 0 ) ( use translate to change origin ) of the specified size and with the specified resolution";
    ofxSchemeAPIDesc[28] = "(draw-cone x y z radius height rRes hRes) --> draw a cone at the specified position, with the specified radius and height with specified resolutions";
    ofxSchemeAPIDesc[29] = "(draw-cylinder x y z radius height rRes hRes) --> draw a cylinder at the specified position, with the specified radius and height with specified resolutions";
    ofxSchemeAPIDesc[30] = "(draw-plane x y z w h resX resY) --> draw a plane at the specified position, with the specified width and height and with specified resolutions";
    ofxSchemeAPIDesc[31] = "(draw-sphere size res) --> draw a sphere at origin position( 0, 0 ) ( use translate to change origin ) of the specified size and with the specified resolution";
    ofxSchemeAPIDesc[32] = "(draw-string 'text' x y) | (draw-string 'text' x y 'font' fontsize) --> draw a text at the specified position, can be set the font file ( path must be relative to script path) and font size";
    ofxSchemeAPIDesc[33] = "(clamp value min max)  --> restricts a value to be within a specified range defined by values min and max";
    ofxSchemeAPIDesc[34] = "(deg-to-rad degree)  --> convert degrees to radians";
    ofxSchemeAPIDesc[35] = "(rad-to-deg radians)  --> convert radians to degrees";
    ofxSchemeAPIDesc[36] = "(dist x1 y1 z1 x2 y2 z2)  --> calculates the 3D distance between two points";
    ofxSchemeAPIDesc[37] = "(lerp start stop amount)  --> linearly interpolate a value within a range";
    ofxSchemeAPIDesc[38] = "(map value minIN maxIN minOUT maxOUT)  --> given a value and an input range, map the value to an output range";
    ofxSchemeAPIDesc[39] = "(noise x y z)  --> calculates a three dimensional Simplex Noise value between 0.0...1.0";
    ofxSchemeAPIDesc[40] = "(draw-image index 'path' x y w h) | (draw-image index 'path' x y w h lockproportion)  -->  draw image file with specified path ( path must be relative to script path), at specified position with specified dimensions, choosing to maintain proportions or not";
    ofxSchemeAPIDesc[41] = "(draw-video index 'path' x y w h) | (draw-video index 'path' x y w h lockproportion)  -->  draw video file with specified path ( path must be relative to script path), at specified position with specified dimensions, choosing to maintain proportions or not";
    ofxSchemeAPIDesc[42] = "(draw-camera index x y w h) | (draw-camera index x y w h lockproportion)  -->  draw camera with specified index ( check console ), at specified position with specified dimensions, choosing to maintain proportions or not";
    ofxSchemeAPIDesc[43] = "(draw-screen x y w h)  -->  draw output screen at specified position with specified dimensions ( useful for feedbacks )";

    // open default script
    needToLoadScript = true;
    scriptLoaded = false;

    // setup gui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.MouseDrawCursor = false;

    mainTheme = new MosaicTheme();
    gui.setup(mainTheme,false);
    showGUI = false;

    // console
    setupCommands();
    mosaicLoggerChannel = shared_ptr<MosaicLoggerChannel>(new MosaicLoggerChannel());
    mosaicLoggerChannel->setRetinaScale(1.0);
    ofAddListener(mosaicLoggerChannel->commandEvent ,this,&ofApp::sendCommand);

    ofSetLoggerChannel(mosaicLoggerChannel);

    eval = true;
    scriptError = false;
}

//--------------------------------------------------------------
void ofApp::update(){

    scheme.setMouse((ofGetMouseX() - thposX)/thdrawW * fbo->getWidth(),(ofGetMouseY() - thposY)/thdrawH * fbo->getHeight());
    scheme.update();
    scheme.setScreenTexture(fbo->getTexture());

    if(needToLoadScript){
        needToLoadScript = false;
        loadBuffers();
    }

    if(eval){
        eval = false;
        scriptBuffer = editor.getText();
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

        if(!scriptError){
            const char* res = scheme.evalScript(scriptBuffer);
            if(res != nullptr){
                scriptError = true;
                ofLog(OF_LOG_ERROR,"%s",res);
                editor.getSettings().setCursorColor(ofColor::red);
            }
        }

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

    if(showGUI){
        drawGUI();
    }
}

//--------------------------------------------------------------
void ofApp::drawGUI(){

    gui.begin();
    {

        // Console
        ImGui::SetNextWindowPos(ImVec2(0,ofGetWindowHeight()/3 * 2), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(ofGetWindowWidth(),ofGetWindowHeight()/3), ImGuiCond_Always);

        bool open = showGUI;

        mosaicLoggerChannel->Draw("Console", &open);

    }

    gui.end();

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    bool modifierPressed = ofxEditor::getSuperAsModifier() ? ofGetKeyPressed(OF_KEY_SUPER) : ofGetKeyPressed(OF_KEY_CONTROL);
    if(modifierPressed) {
        switch(key) {
        case 'o':
            // TODO
            return;
        case 's':
            filepath = ofToDataPath(ofToString(editor.getCurrentEditor())+".scm",true);
            editor.saveFile(filepath,editor.getCurrentEditor());
            return;
        case 'e':
            scheme.clearScript();
            eval = true;
            scriptError = false;
            editor.getSettings().setCursorColor(cursorColor);
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
        case 'g':
            showGUI = !showGUI;
            mosaicLoggerChannel->looseFocus = true;
            mosaicLoggerChannel->log_command = "";
            return;

        }
    }

    // send regular key pressed to script if the editor is hidden
    if(!hideEditor && !showGUI && ofGetWindowPtr() != nullptr) {
        editor.keyPressed(key);
    }

#if defined(TARGET_LINUX) || defined(TARGET_WIN32)
    if(modifierPressed && key == 113){ // exit -- > MOD-Q
        scheme.clearScript();
        scheme.evalScript("");
        ofExit(0);
    }
#endif

    //ofLog(OF_LOG_NOTICE,"%i",key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == OF_KEY_TAB && showGUI){
        string tc = mosaicLoggerChannel->log_command;

        ImGui::SetWindowFocus(NULL); // change focus to nowhere
        mosaicLoggerChannel->setCommand(getCommandMatch(tc)); // autocomplete command
        mosaicLoggerChannel->recoverFocus = true; // recover focus on console input command
    }
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
void ofApp::openFileEvent(int &whichEditor) {
    // received an editor open via CTRL/Super + o

    ofLogNotice() << "received open event for editor " << whichEditor
        << " with filename " << editor.getEditorFilename(whichEditor);
}

//--------------------------------------------------------------
void ofApp::saveFileEvent(int &whichEditor) {
    // received an editor save via CTRL/Super + s or CTRL/Super + d

    ofLogNotice() << "received save event for editor " << whichEditor
        << " with filename " << editor.getEditorFilename(whichEditor);
}

//--------------------------------------------------------------
void ofApp::loadBuffers(){

    for(size_t i =0;i<=9;i++){
        filepath = ofToDataPath(ofToString(i)+".scm",true);
        if(i == 0){
            ofFile temp(filepath);
            string filesFolder = temp.getEnclosingDirectory()+"/files";
            scriptFolder.listDir(filesFolder);
            scheme.clearScript();
            scheme.setScriptPath(scriptFolder.getAbsolutePath());
            currentScriptFile.open(filepath);
            sketchContent = ofBufferFromFile(currentScriptFile.getAbsolutePath());
        }
        // open script files into editor buffers
        editor.openFile(filepath,i);
    }

    scriptLoaded = true;
}

//--------------------------------------------------------------
void ofApp::setupCommands(){
    commandsList.assign(100,MosaicCommand());
    int index = 0;
    for(size_t i=0;i<ofxSchemeAPI.size();i++){
        commandsList[i].command = ofxSchemeAPI.at(i);
        commandsList[i].description = ofxSchemeAPIDesc.at(i);
    }
    index = ofxSchemeAPI.size();
    commandsList[index].command = "help";
    commandsList[index].description = "Print the list with all available commands";
    commandsList[index+1].command = "list_files";
    commandsList[index+1].description = "List all files in script folder";
    commandsList[index+2].command = "api_methods";
    commandsList[index+2].description = "List all OF scheme live-coding methods";
    commandsList[index+3].command = "random_example";
    commandsList[index+3].description = "Load and run a random example";
    commandsList[index+4].command = "browser";
    commandsList[index+4].description = "Open your predefined internet browser";
    commandsList[index+5].command = "exit";
    commandsList[index+5].description = "Quit Scheme Live Coding";
}

//--------------------------------------------------------------
void ofApp::sendCommand(string &command){
    // go to the end of logger
    mosaicLoggerChannel->console.SetCursorPosition(TextEditor::Coordinates((int)mosaicLoggerChannel->console.GetTotalLines(), 0));

    bool foundCommand = false;
    for(size_t i=0;i<ofxSchemeAPI.size();i++){
        if(command == ofxSchemeAPI.at(i)){
            ofLog(OF_LOG_NOTICE,"%s usage: %s",ofxSchemeAPI.at(i).c_str(),ofxSchemeAPIDesc.at(i).c_str());
            foundCommand = true;
        }
    }

    // run command
    if(command == "help"){
        ofLog(OF_LOG_NOTICE,"%s","-- The list of all available commands");
        for(size_t i=ofxSchemeAPI.size();i<commandsList.size();i++){
            if(commandsList[i].command != ""){
                ofLog(OF_LOG_NOTICE,"%s - %s",commandsList[i].command.c_str(),commandsList[i].description.c_str());
            }
        }
    }else if(command == "list_files"){
        if(scriptFolder.getFiles().size() > 0){
            ofLog(OF_LOG_NOTICE,"%s","-- The list of files inside scheme live-coding files/ folder");
            for(size_t i=0;i<scriptFolder.getFiles().size();i++){
                if(scriptFolder.getFile(i).isDirectory()){
                    ofLog(OF_LOG_NOTICE,"%s/",scriptFolder.getFile(i).getFileName().c_str());
                }else{
                    ofLog(OF_LOG_NOTICE,"%s",scriptFolder.getFile(i).getFileName().c_str());
                }
            }
        }else{
            ofLog(OF_LOG_NOTICE,"%s","scheme live-coding files/ folder is empty");
        }

    }else if(command == "api_methods"){
        ofLog(OF_LOG_NOTICE,"%s","-- The List all OF scheme live-coding methods");
        string temp = "\n\n";
        for(size_t i=0;i<ofxSchemeAPI.size();i++){
            temp += ofxSchemeAPI.at(i);
            if(i<ofxSchemeAPI.size()-1){
                temp += ",";
            }
            if(i != 0 && i % 5 == 0){
                temp += "\n";
            }
        }
        ofLog(OF_LOG_NOTICE,"%s\n",temp.c_str());
    }else if(command == "random_example"){
        scheme.clearScript();
        ofDirectory temp;
        temp.listDir(ofToDataPath("examples/"));

        // TODO, add more examples and choose a random one
        currentScriptFile.open(temp.getFile(0).getAbsolutePath());

        sketchContent = ofBufferFromFile(currentScriptFile.getAbsolutePath());
        editor.openFile(temp.getFile(0).getAbsolutePath(),editor.getCurrentEditor());
        eval = true;
        scriptError = false;
        editor.getSettings().setCursorColor(cursorColor);

        ofLog(OF_LOG_NOTICE,"Loaded random example: %s",temp.getFile(0).getFileName().c_str());

    }else if(command == "browser"){
        ofLaunchBrowser("https://www.duckduckgo.com");
    }else if(command == "exit"){
        scheme.clearScript();
        scheme.evalScript("");
        ofExit(0);
    }else if(!foundCommand){
        ofLog(OF_LOG_NOTICE,"%s","[error] Command not found!");
    }
}

//--------------------------------------------------------------
string ofApp::getCommandMatch(string text){
    for(size_t i=0;i<commandsList.size();i++){
        if (containString(commandsList[i].command, text)) {
            return commandsList[i].command;
        }
    }

    return text;
}

//--------------------------------------------------------------
bool ofApp::containString(string str1, string str2) {
    return (str1.find(str2) != string::npos);
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
        editor.resize(1280,720);
    }else{
        scaleTextureToWindow(fbo->getWidth(), fbo->getHeight(), ofGetScreenWidth(),ofGetScreenHeight());
        editor.resize(ofGetScreenWidth(),ofGetScreenHeight());
    }

}
