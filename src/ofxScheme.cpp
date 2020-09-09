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
    scm_c_define_gsubr("OUTPUT_WIDTH", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_window_width));
    scm_c_define_gsubr("OUTPUT_HEIGHT", 0, 0, 0, (scm_t_subr)(&ofxScheme::get_window_height));
    scm_c_define_gsubr("background", 3, 0, 0, (scm_t_subr)(&ofxScheme::background));
    scm_c_define_gsubr("background-alpha", 4, 0, 0, (scm_t_subr)(&ofxScheme::background_alpha));
    scm_c_define_gsubr("set-color",  3, 0, 0, (scm_t_subr)(&ofxScheme::set_color));
    scm_c_define_gsubr("circle",     3, 0, 0, (scm_t_subr)(&ofxScheme::circle));
    return NULL;
}

map<int,int>     loopIterators;

//--------------------------------------------------------------
ofxScheme::ofxScheme(){
    for(int i=0;i<1000;i++){
        loopIterators[i] = 0;
    }
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


//-------------------------------------------------------------- SCHEME DSL LANGUAGE
//--------------------------------------------------------------
//--------------------------------------------------------------
SCM ofxScheme::loop(SCM index, SCM start, SCM end, SCM increment){
    int ind = scm_to_int16(index);
    int i = scm_to_int16(increment);
    int s = scm_to_int16(start);

    if(loopIterators[ind] < scm_to_int16(end)){
        loopIterators[ind] += i;
    }else{
        loopIterators[ind] = s;
    }

    return  scm_from_int16(loopIterators[ind]);
}

SCM ofxScheme::get_window_width(){
    return  scm_from_int16( ofGetWindowWidth() );
}

SCM ofxScheme::get_window_height(){
    return  scm_from_int16( ofGetWindowHeight() );
}

SCM ofxScheme::background(SCM r, SCM g, SCM b){
    ofBackground(255 * scm_to_double(r),255 * scm_to_double(g),255 * scm_to_double(b));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::background_alpha(SCM r, SCM g, SCM b, SCM a){
    ofSetColor(255 * scm_to_double(r),255 * scm_to_double(g),255 * scm_to_double(b),255 * scm_to_double(a));
    ofDrawRectangle(0,0,ofGetWindowWidth(),ofGetWindowHeight());
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::set_color(SCM r, SCM g, SCM b){
    ofSetColor(255 * scm_to_double(r),255 * scm_to_double(g),255 * scm_to_double(b));
    return SCM_UNSPECIFIED;
}

SCM ofxScheme::circle(SCM x, SCM y, SCM r){
    ofFill();
    ofSetCircleResolution(50);
    ofDrawCircle(scm_to_double(x),scm_to_double(y),scm_to_double(r));
    return SCM_UNSPECIFIED;
}


//-------------------------------------------------------------- SCHEME DSL LANGUAGE
//--------------------------------------------------------------
//--------------------------------------------------------------
