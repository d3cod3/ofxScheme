#pragma once

#include "ofMain.h"
#include "PathWatcher.h"

#include "ofxScheme.h"

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

    ofxScheme   scheme;

    PathWatcher         watcher;
    ofFile              currentScriptFile;
    ofBuffer            sketchContent;
    string              filepath;
    bool                needToLoadScript;
    bool                scriptLoaded;

};
