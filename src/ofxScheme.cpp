/*==============================================================================

    ofxScheme: A Scheme DSL live-coding embedded interface for openFrameworks

    Copyright (c) 2020 Emanuele Mazza aka n3m3da <emanuelemazza@d3cod3.org>

    ofxScheme is distributed under the MIT License.
    This gives everyone the freedoms to use ofxScheme in any context:
    commercial or non-commercial, public or private, open or closed source.

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

    See https://github.com/d3cod3/ofxScheme

==============================================================================*/

#include "ofxScheme.h"
#include "scm_safe_calls.h"

//--------------------------------------------------------------
// Scripting functions -----------------------------------------
void* register_functions(void* data){
    // Structure
    scm_c_define_gsubr("loop", 4, 0, 0, (scm_t_subr)(&ofxScheme::loop));

    // Mouse
    scm_c_define_gsubr("MOUSE_X", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_mouse_x));
    scm_c_define_gsubr("MOUSE_Y", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_mouse_y));

    // Window
    scm_c_define_gsubr("OUTPUT_WIDTH", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_window_width));
    scm_c_define_gsubr("OUTPUT_HEIGHT", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_window_height));

    // Time
    scm_c_define_gsubr("time", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_time));

    // External Data
    scm_c_define_gsubr("mosaic_data_inlet", 1, 0, 0, (scm_t_subr)(&ofxScheme::getExternalDataAt));

    // Graphics
    scm_c_define_gsubr("background", 3, 0, 0, (scm_t_subr)(&ofxScheme::background));
    scm_c_define_gsubr("background-alpha", 4, 0, 0, (scm_t_subr)(&ofxScheme::background_alpha));
    scm_c_define_gsubr("set-color", 3, 1, 0, (scm_t_subr)(&ofxScheme::set_color));
    scm_c_define_gsubr("fill", 0, 0, 0, (scm_t_subr)(&ofxScheme::fill));
    scm_c_define_gsubr("no-fill", 0, 0, 0, (scm_t_subr)(&ofxScheme::noFill));

    scm_c_define_gsubr("push", 0, 0, 0, (scm_t_subr)(&ofxScheme::push));
    scm_c_define_gsubr("pop", 0, 0, 0, (scm_t_subr)(&ofxScheme::pop));
    scm_c_define_gsubr("begin-shape", 0, 0, 0, (scm_t_subr)(&ofxScheme::beginShape));
    scm_c_define_gsubr("end-shape", 0, 0, 0, (scm_t_subr)(&ofxScheme::endShape));
    scm_c_define_gsubr("translate", 3, 0, 0, (scm_t_subr)(&ofxScheme::translate));
    scm_c_define_gsubr("rotate", 3, 0, 0, (scm_t_subr)(&ofxScheme::rotate));
    scm_c_define_gsubr("scale", 3, 0, 0, (scm_t_subr)(&ofxScheme::scale));

    scm_c_define_gsubr("line-width", 1, 0, 0, (scm_t_subr)(&ofxScheme::lineWidth));
    scm_c_define_gsubr("curve-res", 1, 0, 0, (scm_t_subr)(&ofxScheme::curveRes));

    scm_c_define_gsubr("draw-vertex", 3, 0, 0, (scm_t_subr)(&ofxScheme::vertex));
    scm_c_define_gsubr("draw-line", 4, 1, 0, (scm_t_subr)(&ofxScheme::line));
    scm_c_define_gsubr("draw-curve", 8, 0, 0, (scm_t_subr)(&ofxScheme::curve));
    scm_c_define_gsubr("draw-bezier", 8, 0, 0, (scm_t_subr)(&ofxScheme::bezier));
    scm_c_define_gsubr("draw-circle", 4, 0, 0, (scm_t_subr)(&ofxScheme::circle));
    scm_c_define_gsubr("draw-ellipse", 5, 0, 0, (scm_t_subr)(&ofxScheme::ellipse));
    scm_c_define_gsubr("draw-rectangle", 5, 0, 0, (scm_t_subr)(&ofxScheme::rectangle));
    scm_c_define_gsubr("draw-triangle", 6, 0, 0, (scm_t_subr)(&ofxScheme::triangle));
    scm_c_define_gsubr("draw-cube", 2, 0, 0, (scm_t_subr)(&ofxScheme::cube));
    scm_c_define_gsubr("draw-cone", 7, 0, 0, (scm_t_subr)(&ofxScheme::cone));
    scm_c_define_gsubr("draw-cylinder", 7, 0, 0, (scm_t_subr)(&ofxScheme::cylinder));
    scm_c_define_gsubr("draw-plane", 7, 0, 0, (scm_t_subr)(&ofxScheme::plane));
    scm_c_define_gsubr("draw-sphere", 2, 0, 0, (scm_t_subr)(&ofxScheme::sphere));
    scm_c_define_gsubr("draw-string", 3, 2, 0, (scm_t_subr)(&ofxScheme::bitmap_string));

    // Math
    scm_c_define_gsubr("clamp", 3, 0, 0, (scm_t_subr)(&ofxScheme::clamp));
    scm_c_define_gsubr("deg-to-rad", 1, 0, 0, (scm_t_subr)(&ofxScheme::degToRad));
    scm_c_define_gsubr("rad-to-deg", 1, 0, 0, (scm_t_subr)(&ofxScheme::radToDeg));
    scm_c_define_gsubr("dist", 6, 0, 0, (scm_t_subr)(&ofxScheme::dist));
    scm_c_define_gsubr("lerp", 3, 0, 0, (scm_t_subr)(&ofxScheme::lerp));
    scm_c_define_gsubr("map", 5, 0, 0, (scm_t_subr)(&ofxScheme::map));
    scm_c_define_gsubr("noise", 3, 0, 0, (scm_t_subr)(&ofxScheme::noise));

    // ofImage
    scm_c_define_gsubr("draw-image", 6, 1, 0, (scm_t_subr)(&ofxScheme::image));

    // ofVideoPlayer
    scm_c_define_gsubr("draw-video", 6, 1, 0, (scm_t_subr)(&ofxScheme::video));

    // oVideofGrabber
    scm_c_define_gsubr("draw-camera", 5, 1, 0, (scm_t_subr)(&ofxScheme::grabber));

    // Screen
    scm_c_define_gsubr("draw-screen", 4, 0, 0, (scm_t_subr)(&ofxScheme::outputTexture));

    return NULL;
}
//--------------------------------------------------------------


// ofxScheme Live Coding VARS ----------------------------------
std::map<int,int>                       loopIterators;
std::string                             scriptPath;
int                                     winW, winH;
float                                   mouseX, mouseY;

std::map<int,ofImage>                   images;
std::map<int,ofVideoPlayer>             videos;
std::map<int,ofVideoGrabber>            cams;
std::map<std::string,ofTrueTypeFont>    fonts;

ofTexture                               screenTexture;
std::vector<float>                      externalData;


//--------------------------------------------------------------
ofxScheme::ofxScheme(){

}

//--------------------------------------------------------------
ofxScheme::~ofxScheme(){

}

//--------------------------------------------------------------
void ofxScheme::setup(){
  scm_with_guile(&register_functions, NULL);

  setWindowDim(1280,720);

  screenTexture.allocate(ofGetScreenWidth(),ofGetScreenHeight(),GL_RGBA);

  externalData.assign(1,0.0f);
}

//--------------------------------------------------------------
void ofxScheme::update(){
    // update loaded videos
    for(std::map<int,ofVideoPlayer>::iterator it = videos.begin(); it != videos.end(); it++ ){
        if(it->second.isPlaying()){
            it->second.setVolume(0);
            it->second.update();
        }
    }
    // update video grabbers
    for(std::map<int,ofVideoGrabber>::iterator it = cams.begin(); it != cams.end(); it++ ){
        if(it->second.isInitialized()){
            it->second.update();
        }
    }
}

//--------------------------------------------------------------
const char* ofxScheme::evalScript(std::string scriptContent){
    return gdbscm_safe_eval_string(scriptContent.c_str());
    //scm_c_eval_string(scriptContent.c_str());
}

//--------------------------------------------------------------
void ofxScheme::clearScript(){
    loopIterators.clear();
    images.clear();
    videos.clear();
    cams.clear();
    fonts.clear();
}

//--------------------------------------------------------------
void ofxScheme::setWindowDim(int w, int h){
    winW = w;
    winH = h;
}

//--------------------------------------------------------------
void ofxScheme::setMouse(float x, float y){
    mouseX = x;
    mouseY = y;
}

//--------------------------------------------------------------
void ofxScheme::setScriptPath(std::string abspath){
    ofFile tempFileScript(abspath);
    //scriptPath = tempFileScript.getEnclosingDirectory().substr(0,tempFileScript.getEnclosingDirectory().size());
    scriptPath = abspath+"/";
    ofLog(OF_LOG_NOTICE,"Live coding files folder set to: %s",scriptPath.c_str());
}

//--------------------------------------------------------------
void ofxScheme::setScreenTexture(ofTexture tex){
    screenTexture = tex;
}

//--------------------------------------------------------------
void ofxScheme::setExternalData(std::vector<float> data){
    externalData = data;
}

//-------------------------------------------------------------- SCHEME DSL LANGUAGE
//--------------------------------------------------------------
//--------------------------------------------------------------

// ------------------------------------------------------------- Structure
SCM ofxScheme::loop(SCM index, SCM start, SCM end, SCM increment){


    if(loopIterators.find(scm_to_int16(index)) == loopIterators.end()){
        loopIterators[scm_to_int16(index)] = 0;
    }

    if(loopIterators[scm_to_int16(index)] < scm_to_int16(end)){
        loopIterators[scm_to_int16(index)] += scm_to_int16(increment);
    }else{
        loopIterators[scm_to_int16(index)] = scm_to_int16(start);
    }

    return  scm_from_int16(loopIterators[scm_to_int16(index)]);
}

// ------------------------------------------------------------- Mouse
SCM ofxScheme::get_mouse_x(){
    return scm_from_int16(mouseX);
}

SCM ofxScheme::get_mouse_y(){
    return scm_from_int16(mouseY);
}


// ------------------------------------------------------------- Window
SCM ofxScheme::get_window_width(){
    return  scm_from_int16( winW );
}

SCM ofxScheme::get_window_height(){
    return  scm_from_int16( winH );
}

// ------------------------------------------------------------- Time
SCM ofxScheme::get_time(){
    return  scm_from_int16( ofGetElapsedTimeMillis() );
}

// ------------------------------------------------------------- External Data
SCM ofxScheme::getExternalDataAt(SCM i){
    if(externalData.size() > scm_to_int16(i)){
        return scm_from_double(externalData.at(scm_to_int16(i)));
    }else{
        return scm_from_double(0.0f);
    }

}

// ------------------------------------------------------------- Graphics
SCM ofxScheme::background(SCM r, SCM g, SCM b){
    ofBackground(255 * scm_to_double(r),255 * scm_to_double(g),255 * scm_to_double(b));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::background_alpha(SCM r, SCM g, SCM b, SCM a){
    ofSetColor(255 * scm_to_double(r),255 * scm_to_double(g),255 * scm_to_double(b),255 * scm_to_double(a));
    ofDrawRectangle(0,0,winW,winH);
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::set_color(SCM r, SCM g, SCM b, SCM a){
    if (scm_is_eq (a, SCM_UNDEFINED)){
        ofSetColor(255 * scm_to_double(r),255 * scm_to_double(g),255 * scm_to_double(b),255);
    }else{
        ofSetColor(255 * scm_to_double(r),255 * scm_to_double(g),255 * scm_to_double(b),255 * scm_to_double(a));
    }

    return SCM_UNSPECIFIED;
}

SCM ofxScheme::fill(){
    ofFill();
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::noFill(){
    ofNoFill();
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::push(){
    ofPushMatrix();
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::pop(){
    ofPopMatrix();
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::beginShape(){
    ofBeginShape();
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::endShape(){
    ofEndShape();
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::translate(SCM x, SCM y, SCM z){
    ofTranslate(scm_to_double(x),scm_to_double(y),scm_to_double(z));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::rotate(SCM x, SCM y, SCM z){
    ofRotateXDeg(scm_to_double(x));
    ofRotateYDeg(scm_to_double(y));
    ofRotateZDeg(scm_to_double(z));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::scale(SCM x, SCM y, SCM z){
    ofScale(scm_to_double(x),scm_to_double(y),scm_to_double(z));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::vertex(SCM x, SCM y, SCM z){
    ofVertex(scm_to_double(x),scm_to_double(y),scm_to_double(z));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::line(SCM x1, SCM y1, SCM x2, SCM y2, SCM w){
    if (scm_is_eq (w, SCM_UNDEFINED)){
        ofSetLineWidth(1);
    }else{
        ofSetLineWidth(scm_to_int16(w));
    }
    ofDrawLine(scm_to_double(x1),scm_to_double(y1),scm_to_double(x2),scm_to_double(y2));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::curve(SCM x1, SCM y1, SCM x2, SCM y2,SCM x3, SCM y3, SCM x4, SCM y4){
    ofDrawCurve(scm_to_double(x1),scm_to_double(y1),scm_to_double(x2),scm_to_double(y2),scm_to_double(x3),scm_to_double(y3),scm_to_double(x4),scm_to_double(y4));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::bezier(SCM x1, SCM y1, SCM x2, SCM y2,SCM x3, SCM y3, SCM x4, SCM y4){
    ofDrawBezier(scm_to_double(x1),scm_to_double(y1),scm_to_double(x2),scm_to_double(y2),scm_to_double(x3),scm_to_double(y3),scm_to_double(x4),scm_to_double(y4));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::lineWidth(SCM lw){
    ofSetLineWidth(scm_to_double(lw));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::curveRes(SCM cr){
    ofSetCurveResolution(scm_to_double(cr));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::circle(SCM x, SCM y, SCM r, SCM res){
    ofSetCircleResolution(scm_to_int16(res));
    ofDrawCircle(scm_to_double(x),scm_to_double(y),scm_to_double(r));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::ellipse(SCM x, SCM y, SCM rx, SCM ry, SCM res){
    ofSetCircleResolution(scm_to_int16(res));
    ofDrawEllipse(scm_to_double(x),scm_to_double(y),scm_to_double(rx),scm_to_double(ry));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::rectangle(SCM x, SCM y, SCM w, SCM h, SCM r){
    if (scm_is_eq (r, SCM_UNDEFINED)){
        ofDrawRectangle(scm_to_double(x),scm_to_double(y),scm_to_double(w),scm_to_double(h));
    }else{
        ofDrawRectRounded(scm_to_double(x),scm_to_double(y),scm_to_double(w),scm_to_double(h),scm_to_double(r));
    }

    return SCM_UNSPECIFIED;
}

SCM ofxScheme::triangle(SCM x1, SCM y1, SCM x2, SCM y2, SCM x3, SCM y3){
    ofDrawTriangle(scm_to_double(x1),scm_to_double(y1),scm_to_double(x2),scm_to_double(y2),scm_to_double(x3),scm_to_double(y3));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::cube(SCM s, SCM res){
    ofSetBoxResolution(scm_to_int16(res));
    ofDrawBox(scm_to_double(s));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::cone(SCM x, SCM y, SCM z, SCM r, SCM h, SCM rSeg, SCM hSeg){
    ofSetConeResolution(scm_to_int16(rSeg),scm_to_int16(hSeg));
    ofDrawCone(scm_to_double(x),scm_to_double(y),scm_to_double(z),scm_to_double(r),scm_to_double(h));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::cylinder(SCM x, SCM y, SCM z, SCM r, SCM h, SCM rSeg, SCM hSeg){
    ofSetCylinderResolution(scm_to_int16(rSeg),scm_to_int16(hSeg));
    ofDrawCylinder(scm_to_double(x),scm_to_double(y),scm_to_double(z),scm_to_double(r),scm_to_double(h));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::plane(SCM x, SCM y, SCM z, SCM w, SCM h, SCM resX, SCM resY){
    ofSetPlaneResolution(scm_to_int16(resX),scm_to_int16(resY));
    ofDrawPlane(scm_to_double(x),scm_to_double(y),scm_to_double(z),scm_to_double(w),scm_to_double(h));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::sphere(SCM s, SCM res){
    ofSetSphereResolution(scm_to_int16(res));
    ofDrawSphere(scm_to_double(s));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::bitmap_string(SCM text, SCM x, SCM y, SCM font, SCM fontsize){
    std::string temp = "";
    int fs = 0;
    if (scm_is_eq (font, SCM_UNDEFINED)){
        temp = "default";
        ofDrawBitmapString(scm_to_latin1_string(text),scm_to_double(x),scm_to_double(y));
    }else{
        temp = scm_to_locale_string(font);
        if (scm_is_eq (fontsize, SCM_UNDEFINED)){
            fs = 12;
        }else{
            fs = scm_to_int16(fontsize);
        }

        if(fonts.find(temp) == fonts.end()){
            ofTrueTypeFont ttf;
            std::string abspath = scriptPath+temp;
            ttf.load(abspath,fs);

            fonts[temp] = ttf;
        }else{
            fonts[temp].drawString(scm_to_latin1_string(text),scm_to_double(x),scm_to_double(y));
        }

    }

    return SCM_UNSPECIFIED;
}

// ------------------------------------------------------------- Math
SCM ofxScheme::clamp(SCM v, SCM min, SCM max){
    return scm_from_double(ofClamp(scm_to_double(v),scm_to_double(min),scm_to_double(max)));
}

SCM ofxScheme::degToRad(SCM deg){
    return scm_from_double(ofDegToRad(scm_to_double(deg)));
}

SCM ofxScheme::radToDeg(SCM rad){
    return scm_from_double(ofRadToDeg(scm_to_double(rad)));
}

SCM ofxScheme::dist(SCM x1, SCM y1, SCM z1, SCM x2, SCM y2, SCM z2){
    return scm_from_double(ofDist(scm_to_double(x1),scm_to_double(y1),scm_to_double(z1),scm_to_double(x2),scm_to_double(y2),scm_to_double(z2)));
}

SCM ofxScheme::lerp(SCM start, SCM stop, SCM amnt){
    return scm_from_double(ofLerp(scm_to_double(start),scm_to_double(stop),scm_to_double(amnt)));
}

SCM ofxScheme::map(SCM v, SCM imin, SCM imax, SCM omin, SCM omax){
    return scm_from_double(ofMap(scm_to_double(v),scm_to_double(imin),scm_to_double(imax),scm_to_double(omin),scm_to_double(omax)));
}

SCM ofxScheme::noise(SCM x, SCM y, SCM z){
    return scm_from_double(ofNoise(scm_to_double(x),scm_to_double(y),scm_to_double(z)));
}


// ------------------------------------------------------------- ofImage
SCM ofxScheme::image(SCM index, SCM path, SCM x, SCM y, SCM w, SCM h, SCM lockproportion){

    if(images.find(scm_to_int16(index)) == images.end()){
        ofImage temp;
        std::string abspath = scriptPath+scm_to_locale_string(path);
        temp.load(abspath);

        images[scm_to_int16(index)] = temp;
    }else{
        if (scm_is_eq (lockproportion, SCM_UNDEFINED)){
            images[scm_to_int16(index)].draw(scm_to_double(x),scm_to_double(y),scm_to_double(w),scm_to_double(h));
        }else{
            float thdrawW,thdrawH,thposX,thposY;
            // wider image than drawing dimensions ( w x h )
            if(images[scm_to_int16(index)].getWidth()/images[scm_to_int16(index)].getHeight() >= scm_to_double(w)/scm_to_double(h)){
                thdrawW           = scm_to_double(w);
                thdrawH           = (images[scm_to_int16(index)].getHeight()*scm_to_double(w)) / images[scm_to_int16(index)].getWidth();
                thposX            = 0;
                thposY            = (scm_to_double(h)-thdrawH)/2.0f;
            // wider drawing dimensions ( w x h ) than image
            }else{
                thdrawW           = (images[scm_to_int16(index)].getWidth()*scm_to_double(h)) / images[scm_to_int16(index)].getHeight();
                thdrawH           = scm_to_double(h);
                thposX            = (scm_to_double(w)-thdrawW)/2.0f;
                thposY            = 0;
            }
            images[scm_to_int16(index)].draw(thposX+scm_to_double(x),thposY+scm_to_double(y),thdrawW,thdrawH);
        }

    }

    return SCM_UNSPECIFIED;
}

// ------------------------------------------------------------- ofVideoPlayer
SCM ofxScheme::video(SCM index, SCM path, SCM x, SCM y, SCM w, SCM h, SCM lockproportion){

    if(videos.find(scm_to_int16(index)) == videos.end()){
        ofVideoPlayer temp;
        std::string abspath = scriptPath+scm_to_locale_string(path);
        temp.load(abspath);
        temp.setVolume(0);
        temp.setLoopState(OF_LOOP_NORMAL);
        temp.play();

        videos[scm_to_int16(index)] = temp;
    }else{
        if (scm_is_eq (lockproportion, SCM_UNDEFINED)){
            videos[scm_to_int16(index)].draw(scm_to_double(x),scm_to_double(y),scm_to_double(w),scm_to_double(h));
        }else{
            float thdrawW,thdrawH,thposX,thposY;
            // wider image than drawing dimensions ( w x h )
            if(videos[scm_to_int16(index)].getWidth()/videos[scm_to_int16(index)].getHeight() >= scm_to_double(w)/scm_to_double(h)){
                thdrawW           = scm_to_double(w);
                thdrawH           = (videos[scm_to_int16(index)].getHeight()*scm_to_double(w)) / videos[scm_to_int16(index)].getWidth();
                thposX            = 0;
                thposY            = (scm_to_double(h)-thdrawH)/2.0f;
            // wider drawing dimensions ( w x h ) than image
            }else{
                thdrawW           = (videos[scm_to_int16(index)].getWidth()*scm_to_double(h)) / videos[scm_to_int16(index)].getHeight();
                thdrawH           = scm_to_double(h);
                thposX            = (scm_to_double(w)-thdrawW)/2.0f;
                thposY            = 0;
            }
            videos[scm_to_int16(index)].draw(thposX+scm_to_double(x),thposY+scm_to_double(y),thdrawW,thdrawH);
        }

    }

    return SCM_UNSPECIFIED;
}

// ------------------------------------------------------------- ofVideoGrabber
SCM ofxScheme::grabber(SCM index, SCM x, SCM y, SCM w, SCM h, SCM lockproportion){

    if(cams.find(scm_to_int16(index)) == cams.end()){
        ofVideoGrabber temp;
        temp.setDeviceID(scm_to_int16(index));
        temp.setup(1920, 1080);

        cams[scm_to_int16(index)] = temp;
    }else{
        if (scm_is_eq (lockproportion, SCM_UNDEFINED)){
            cams[scm_to_int16(index)].draw(scm_to_double(x),scm_to_double(y),scm_to_double(w),scm_to_double(h));
        }else{
            float thdrawW,thdrawH,thposX,thposY;
            // wider image than drawing dimensions ( w x h )
            if(cams[scm_to_int16(index)].getWidth()/cams[scm_to_int16(index)].getHeight() >= scm_to_double(w)/scm_to_double(h)){
                thdrawW           = scm_to_double(w);
                thdrawH           = (cams[scm_to_int16(index)].getHeight()*scm_to_double(w)) / cams[scm_to_int16(index)].getWidth();
                thposX            = 0;
                thposY            = (scm_to_double(h)-thdrawH)/2.0f;
            // wider drawing dimensions ( w x h ) than image
            }else{
                thdrawW           = (cams[scm_to_int16(index)].getWidth()*scm_to_double(h)) / cams[scm_to_int16(index)].getHeight();
                thdrawH           = scm_to_double(h);
                thposX            = (scm_to_double(w)-thdrawW)/2.0f;
                thposY            = 0;
            }
            cams[scm_to_int16(index)].draw(thposX+scm_to_double(x),thposY+scm_to_double(y),thdrawW,thdrawH);
        }

    }

    return SCM_UNSPECIFIED;
}

// ------------------------------------------------------------- Screen
SCM ofxScheme::outputTexture(SCM x, SCM y, SCM w, SCM h){
    screenTexture.draw(scm_to_double(x),scm_to_double(y),scm_to_double(w),scm_to_double(h));

    return SCM_UNSPECIFIED;
}

//-------------------------------------------------------------- SCHEME DSL LANGUAGE
//--------------------------------------------------------------
//--------------------------------------------------------------
