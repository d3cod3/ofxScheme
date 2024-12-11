#pragma once

#include "ofMain.h"
#include "PathWatcher.h"
#include "MosaicTheme.h"

#include "ofxScheme.h"
#include "ofxGLEditor.h"
#include "ofxImGui.h"

// this is a simple live coding example using ofxScheme with ofxGLEditor
//
// app key commands:
// MOD -> CTRL
//
// MOD + e: execute script or selected text
// MOD + l: toggle line wrapping
// MOD + n: toggle line numbers
// MOD + f: toggle fullscreen
// MOD + k: toggle auto focus zooming
// MOD + t: show/hide editor
// MOD + 1 to MOD + 9: switch to editor 1 - 9
// MOD + b: blow up the cursor
// MOD + a: select all text in the current editor
// MOD + a + SHIFT: clear all text in the current editor
// MOD + c: copy from system clipboard
// MOD + v: paste to system clipboard
// MOD + z: undo last key input action
// MOD + y: redo last key input action
//
// see ofxGLEditor.h for editor key commands
//

class ofApp : public ofBaseApp, public ofxGLEditorListener{

public:
    void setup();
    void update();
    void draw();
    void drawGUI();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);

    /// ofxGLEditor events
    void saveFileEvent(int &whichEditor);
    void openFileEvent(int &whichEditor);
    void executeScriptEvent(int &whichEditor);
    void evalReplEvent(const string &text);

    // Filepath watcher callback
    void pathChanged(const PathWatcher::Event &event);

    void loadScript(string scriptFile);

    // Console
    void            setupCommands();
    void            sendCommand(string &command);
    string          getCommandMatch(string text);
    bool            containString(string str1, string str2);

    void scaleTextureToWindow(float texW, float texH, float winW, float winH);
    void toggleWindowFullscreen();

    ofxScheme           scheme;
    std::string         scriptBuffer;
    ofFbo               *fbo;
    bool                isFullscreen;
    bool                eval;
    bool                scriptError;
    float               thposX, thposY, thdrawW, thdrawH;

    ofxGLEditor             editor;
    ofxEditorSyntax         syntax;
    ofxEditorColorScheme    colorScheme;
    bool hideEditor;

    ofxImGui::Gui                       gui;
    MosaicTheme                         *mainTheme;
    ofDirectory                         scriptFolder;
    shared_ptr<MosaicLoggerChannel>     mosaicLoggerChannel;
    vector<MosaicCommand>               commandsList;
    vector<string>                      ofxSchemeAPI;
    vector<string>                      ofxSchemeAPIDesc;
    bool                                showGUI;

    PathWatcher         watcher;
    ofFile              currentScriptFile;
    ofBuffer            sketchContent;
    string              filepath;
    bool                needToLoadScript;
    bool                scriptLoaded;

};
