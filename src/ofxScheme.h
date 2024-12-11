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

#pragma once

#include "ofMain.h"

#include <libguile.h>

class ofxScheme {

public:

    ofxScheme();
    ~ofxScheme();

    void setup();
    void update();
    const char* evalScript(string scriptContent);
    void clearScript();

    void setWindowDim(int w, int h);
    void setMouse(float x, float y);
    void setScriptPath(string abspath);


    //---------------------------------------- API
    // structure
    static SCM loop(SCM index, SCM start, SCM end, SCM increment);
    // Mouse
    static SCM get_mouse_x();
    static SCM get_mouse_y();
    // Window
    static SCM get_window_width();
    static SCM get_window_height();
    // Time
    static SCM get_time();
    // Graphics
    static SCM background(SCM r, SCM g, SCM b);
    static SCM background_alpha(SCM r, SCM g, SCM b, SCM a);
    static SCM set_color(SCM r, SCM g, SCM b, SCM a);
    static SCM fill();
    static SCM noFill();
    static SCM push();
    static SCM pop();
    static SCM beginShape();
    static SCM endShape();
    static SCM translate(SCM x, SCM y, SCM z);
    static SCM rotate(SCM x, SCM y, SCM z);
    static SCM scale(SCM x, SCM y, SCM z);
    static SCM vertex(SCM x, SCM y, SCM z);
    static SCM line(SCM x1, SCM y1, SCM x2, SCM y2, SCM w);
    static SCM curve(SCM x1, SCM y1, SCM x2, SCM y2,SCM x3, SCM y3, SCM x4, SCM y4);
    static SCM bezier(SCM x1, SCM y1, SCM x2, SCM y2,SCM x3, SCM y3, SCM x4, SCM y4);
    static SCM lineWidth(SCM lw);
    static SCM curveRes(SCM cr);
    static SCM circle(SCM x, SCM y, SCM r, SCM res);
    static SCM ellipse(SCM x, SCM y, SCM rx, SCM ry, SCM res);
    static SCM rectangle(SCM x, SCM y, SCM w, SCM h, SCM r);
    static SCM triangle(SCM x1, SCM y1, SCM x2, SCM y2, SCM x3, SCM y3);
    static SCM cube(SCM s, SCM res);
    static SCM cone(SCM x, SCM y, SCM z, SCM r, SCM h, SCM rSeg, SCM hSeg);
    static SCM cylinder(SCM x, SCM y, SCM z, SCM r, SCM h, SCM rSeg, SCM hSeg);
    static SCM plane(SCM x, SCM y, SCM z, SCM w, SCM h, SCM resX, SCM resY);
    static SCM sphere(SCM s, SCM res);
    static SCM bitmap_string(SCM text, SCM x, SCM y, SCM font, SCM fontsize);
    // Math
    static SCM clamp(SCM v, SCM min, SCM max);
    static SCM degToRad(SCM deg);
    static SCM radToDeg(SCM rad);
    static SCM dist(SCM x1, SCM y1, SCM z1, SCM x2, SCM y2, SCM z2);
    static SCM lerp(SCM start, SCM stop, SCM amnt);
    static SCM map(SCM v, SCM imin, SCM imax, SCM omin, SCM omax);
    static SCM noise(SCM x, SCM y, SCM z);
    // ofImage
    static SCM image(SCM index, SCM path, SCM x, SCM y, SCM w, SCM h);
    // ofVideoPlayer
    static SCM video(SCM index, SCM path, SCM x, SCM y, SCM w, SCM h);
    // oVideofGrabber
    static SCM grabber(SCM index, SCM x, SCM y, SCM w, SCM h);

    //---------------------------------------- API


};
