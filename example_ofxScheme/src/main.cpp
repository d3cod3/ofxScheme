#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

    ofGLFWWindowSettings settings;
    settings.setGLVersion(3,2);
    settings.setSize(1280,720);
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = true;
    settings.decorated = true;
    settings.stencilBits = 0;

    shared_ptr<ofAppGLFWWindow> appWindow = dynamic_pointer_cast<ofAppGLFWWindow>(ofCreateWindow(settings));
    shared_ptr<ofApp> app(new ofApp);

    ofRunApp(appWindow, app);
    ofRunMainLoop();

    // done
    return EXIT_SUCCESS;

}
