/*==============================================================================

    Mosaic: Live Visual Patching Creative-Coding Platform

    Copyright (c) 2018 Emanuele Mazza aka n3m3da <emanuelemazza@d3cod3.org>

    Mosaic is distributed under the MIT License. This gives everyone the
    freedoms to use Mosaic in any context: commercial or non-commercial,
    public or private, open or closed source.

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

    See https://github.com/d3cod3/Mosaic for documentation

==============================================================================*/

#pragma once

#include "BaseTheme.h"
#include "ofMain.h"

#include "TextEditor.h"
#include "imgui_stdlib.h"

struct MosaicCommand{
    std::string command;
    std::string description;
};

class MosaicTheme : public ofxImGui::BaseTheme{

public:

    void setup() override;

    void fixForRetinaScreen();


};

class MosaicLoggerChannel : public ofBaseLoggerChannel
{
public:

    ImVector<char*>             Items;
    TextEditor                  console;
    std::string                 fake;

    bool                        scrollToBottom;
    bool                        recoverFocus;
    bool                        looseFocus;
    std::string                 log_command;
    float                       retinaScale;

    ofEvent<std::string>        commandEvent;

    MosaicLoggerChannel() {
        scrollToBottom = true;
        recoverFocus = false;
        looseFocus = false;
        log_command = "";
        retinaScale = 1.0f;

        console.SetShowWhitespaces(false);
        console.SetText("");
        console.SetReadOnly(true);
        console.SetShowLineNumbers(false);
        console.SetPalette(TextEditor::GetConsolePalette());
        console.SetLanguageDefinition(TextEditor::LanguageDefinition::SimpleConsole());
    }

    void setRetinaScale(float s){
        retinaScale = s;
    }

    void setCommand(string comm){
        log_command = comm;
    }

    void log( ofLogLevel level, const std::string & module, const std::string & message ){
        std::ostringstream oss;
        oss << ofGetTimestampString("%H:%M:%S:%i") << " ";
        if(ofGetLogLevelName(level, true) != "notice "){
            oss << "[" << ofGetLogLevelName(level, true) << "] ";
        }
        if (module != "") {
            oss << module << ": ";
        }
        oss << message << endl;

        AddLog("%s\n", oss.str().c_str());
    }
    void log( ofLogLevel level, const std::string & module, const char* format, ... ) OF_PRINTF_ATTR( 4, 5 ){
        va_list args;
        va_start(args, format);
        log(level, module, format, args);
        va_end(args);
    }
    void log( ofLogLevel level, const std::string & module, const char* format, va_list args ){
        // Compose the message.
        std::ostringstream oss;
        oss << ofGetTimestampString("%H:%M:%S:%i") << " ";
        if(ofGetLogLevelName(level, true) != "notice "){
            oss << "[" << ofGetLogLevelName(level, true) << "] ";
        }
        if (module != "") {
            oss << module << ": ";
        }

        oss << ofVAArgsToString(format, args) << endl;

        AddLog("%s\n", oss.str().c_str());
    }

    void Clear()     {
        for (int i = 0; i < Items.Size; i++){
            free(Items[i]);
        }
        Items.clear();

        console.SetText("");
    }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2){
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf)-1] = 0;
        va_end(args);
        Items.push_back(strdup(buf));
        console.InsertText(strdup(buf));
        scrollToBottom = true;
    }

    string GetLastLog() {
        if(Items.empty()) return std::string("");

        std::string str(Items.back());
        return str;
    }

    void Draw(const char* title, bool* active){

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        if (!ImGui::Begin(title,active,window_flags))
        {
            ImGui::End();
            return;
        }
        if (ImGui::Button("Clear")) Clear();

        if(looseFocus){
            looseFocus = false;
            ImGui::SetKeyboardFocusHere();
        }
        ImGui::SameLine();
        ImGui::PushItemWidth(1);
        ImGui::InputTextWithHint("##empty","",&fake,ImGuiInputTextFlags_ReadOnly);
        ImGui::PopItemWidth();

        ImGui::Separator();
        if (ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar) ){

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing

            ImVec4 col_default_text = ImGui::GetStyleColorVec4(ImGuiCol_Text);

            console.Render("Console",ImVec2(-1.0, -36*retinaScale));
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if(recoverFocus){
                recoverFocus = false;
                ImGui::SetKeyboardFocusHere();
            }
            ImGui::PushStyleColor(ImGuiCol_TextSelectedBg,ImVec4(1,1,1,0.05));
            ImGui::PushItemWidth(-1);
            if(ImGui::InputTextWithHint("###command","Send a command...",&log_command,ImGuiInputTextFlags_EnterReturnsTrue)){
                if(log_command != ""){
                    ofNotifyEvent(commandEvent,log_command);

                    // clear previuos command
                    log_command = "";
                    // focus on input command again
                    ImGui::SetKeyboardFocusHere(-1);
                }
            }
            ImGui::PopItemWidth();
            ImGui::PopStyleColor();


            if(scrollToBottom){
                scrollToBottom = false;
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::PopStyleVar(1);

        }

        ImGui::EndChild();

        ImGui::End();
    }

};
