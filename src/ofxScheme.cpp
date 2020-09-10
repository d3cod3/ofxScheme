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
    scm_c_define_gsubr("translate", 3, 0, 0, (scm_t_subr)(&ofxScheme::translate));
    scm_c_define_gsubr("rotate", 3, 0, 0, (scm_t_subr)(&ofxScheme::rotate));
    scm_c_define_gsubr("scale", 3, 0, 0, (scm_t_subr)(&ofxScheme::scale));
    scm_c_define_gsubr("draw-circle", 4, 0, 0, (scm_t_subr)(&ofxScheme::circle));
    scm_c_define_gsubr("draw-cube", 1, 0, 0, (scm_t_subr)(&ofxScheme::cube));
    return NULL;
}

map<int,int>        loopIterators;

//--------------------------------------------------------------
ofxScheme::ofxScheme(){

}

//--------------------------------------------------------------
ofxScheme::~ofxScheme(){

}

//--------------------------------------------------------------
void ofxScheme::setup(){
  scm_with_guile(&register_functions, NULL);
}


//--------------------------------------------------------------
void ofxScheme::evalScript(string scriptContent){
  scm_c_eval_string(scriptContent.c_str());
}

//--------------------------------------------------------------
void ofxScheme::clearScript(){
    loopIterators.clear();
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
    return  scm_from_int16( ofGetWindowWidth() );
}

SCM ofxScheme::get_window_height(){
    return  scm_from_int16( ofGetWindowHeight() );
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
    ofDrawRectangle(0,0,ofGetWindowWidth(),ofGetWindowHeight());
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


SCM ofxScheme::circle(SCM x, SCM y, SCM r, SCM res){
    ofSetCircleResolution(scm_to_int16(res));
    ofDrawCircle(scm_to_double(x),scm_to_double(y),scm_to_double(r));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::cube(SCM s){
    ofDrawBox(scm_to_double(s));
    return SCM_UNSPECIFIED;
}


//-------------------------------------------------------------- SCHEME DSL LANGUAGE
//--------------------------------------------------------------
//--------------------------------------------------------------
