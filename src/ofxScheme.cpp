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

//--------------------------------------------------------------
// Scripting functions -----------------------------------------
void* register_functions(void* data){
    scm_c_define_gsubr("loop", 4, 0, 0, (scm_t_subr)(&ofxScheme::loop));

    scm_c_define_gsubr("MOUSE_X", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_mouse_x));
    scm_c_define_gsubr("MOUSE_Y", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_mouse_y));

    scm_c_define_gsubr("OUTPUT_WIDTH", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_window_width));
    scm_c_define_gsubr("OUTPUT_HEIGHT", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_window_height));

    scm_c_define_gsubr("time", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_time));

    scm_c_define_gsubr("background", 3, 0, 0, (scm_t_subr)(&ofxScheme::background));
    scm_c_define_gsubr("background-alpha", 4, 0, 0, (scm_t_subr)(&ofxScheme::background_alpha));
    scm_c_define_gsubr("set-color", 4, 0, 0, (scm_t_subr)(&ofxScheme::set_color));
    scm_c_define_gsubr("fill", 0, 0, 0, (scm_t_subr)(&ofxScheme::fill));
    scm_c_define_gsubr("noFill", 0, 0, 0, (scm_t_subr)(&ofxScheme::noFill));

    scm_c_define_gsubr("push", 0, 0, 0, (scm_t_subr)(&ofxScheme::push));
    scm_c_define_gsubr("pop", 0, 0, 0, (scm_t_subr)(&ofxScheme::pop));
    scm_c_define_gsubr("begin-shape", 0, 0, 0, (scm_t_subr)(&ofxScheme::beginShape));
    scm_c_define_gsubr("end-shape", 0, 0, 0, (scm_t_subr)(&ofxScheme::endShape));
    scm_c_define_gsubr("translate", 3, 0, 0, (scm_t_subr)(&ofxScheme::translate));
    scm_c_define_gsubr("rotate", 3, 0, 0, (scm_t_subr)(&ofxScheme::rotate));
    scm_c_define_gsubr("scale", 3, 0, 0, (scm_t_subr)(&ofxScheme::scale));

    scm_c_define_gsubr("draw-vertex", 3, 0, 0, (scm_t_subr)(&ofxScheme::vertex));
    scm_c_define_gsubr("draw-line", 5, 0, 0, (scm_t_subr)(&ofxScheme::line));
    scm_c_define_gsubr("draw-curve", 8, 0, 0, (scm_t_subr)(&ofxScheme::curve));
    scm_c_define_gsubr("draw-bezier", 8, 0, 0, (scm_t_subr)(&ofxScheme::bezier));
    scm_c_define_gsubr("draw-circle", 4, 0, 0, (scm_t_subr)(&ofxScheme::circle));
    scm_c_define_gsubr("draw-ellipse", 5, 0, 0, (scm_t_subr)(&ofxScheme::ellipse));
    scm_c_define_gsubr("draw-rectangle", 5, 0, 0, (scm_t_subr)(&ofxScheme::rectangle));
    scm_c_define_gsubr("draw-cube", 1, 0, 0, (scm_t_subr)(&ofxScheme::cube));
    scm_c_define_gsubr("draw-sphere", 2, 0, 0, (scm_t_subr)(&ofxScheme::sphere));
    scm_c_define_gsubr("draw-string", 3, 0, 0, (scm_t_subr)(&ofxScheme::bitmap_string));


    return NULL;
}
//--------------------------------------------------------------


// Scheme Live Coding VARS -------------------------------------
map<int,int>        loopIterators;
int                 winW, winH;


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
}


//--------------------------------------------------------------
void ofxScheme::evalScript(string scriptContent){
  scm_c_eval_string(scriptContent.c_str());
}

//--------------------------------------------------------------
void ofxScheme::clearScript(){
    loopIterators.clear();
}

//--------------------------------------------------------------
void ofxScheme::setWindowDim(int w, int h){
    winW = w;
    winH = h;
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
    return scm_from_int16(ofGetMouseX());
}

SCM ofxScheme::get_mouse_y(){
    return scm_from_int16(ofGetMouseY());
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
    ofSetColor(255 * scm_to_double(r),255 * scm_to_double(g),255 * scm_to_double(b),255 * scm_to_double(a));
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
    ofSetLineWidth(scm_to_int16(w));
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
    ofDrawRectRounded(scm_to_double(x),scm_to_double(y),scm_to_double(w),scm_to_double(h),scm_to_double(r));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::cube(SCM s){
    ofDrawBox(scm_to_double(s));
    return SCM_UNSPECIFIED;
}
SCM ofxScheme::sphere(SCM s, SCM res){
    ofSetSphereResolution(scm_to_int16(res));
    ofDrawSphere(scm_to_double(s));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::bitmap_string(SCM text, SCM x, SCM y){
    ofDrawBitmapString(scm_to_latin1_string(text),scm_to_double(x),scm_to_double(y));
    return SCM_UNSPECIFIED;
}



//-------------------------------------------------------------- SCHEME DSL LANGUAGE
//--------------------------------------------------------------
//--------------------------------------------------------------
