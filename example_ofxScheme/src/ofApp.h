#pragma once

#include "ofMain.h"
#include "PathWatcher.h"

#include "ofxScheme.h"
#include "ofxEditor.h"
#include "ofxImGui.h"

// this is a simple live coding example using ofxScheme with
//
// app key commands:
//    MOD -> CTRL or Super (Windows Key, Mac CMD)
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

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);

    // Filepath watcher callback
    void pathChanged(const PathWatcher::Event &event);

    void loadScript(string scriptFile);

    void scaleTextureToWindow(float texW, float texH, float winW, float winH);
    void toggleWindowFullscreen();

    ofxScheme           scheme;
    std::string         scriptBuffer;
    ofFbo               *fbo;
    bool                isFullscreen;
    bool                eval;
    float               thposX, thposY, thdrawW, thdrawH;

    ofxEditor editor;
    ofxEditorSyntax syntax;
    ofxEditorColorScheme colorScheme;
    bool hideEditor;

    ofxImGui::Gui       gui;

    PathWatcher         watcher;
    ofFile              currentScriptFile;
    ofBuffer            sketchContent;
    string              filepath;
    bool                needToLoadScript;
    bool                scriptLoaded;

};
