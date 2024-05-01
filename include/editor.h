#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include "document.h"
#include <dirent.h>
#include <unistd.h>
#include <fstream>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border

class Editor {
    
private:
    // Private Vars
    Document document;
    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
    ftxui::Component explorer;
    ftxui::Component mainScreen;

    // Private Functions
    void LoadCurrFolder();
    std::vector<std::string> ListFiles(const std::string& path);
    void StartMenuUI();
    void OpenFile(std::vector<std::string> files, int idx);

public:
    // Constructor which opens a file/folder in the editor.
    Editor(const std::string &path);

    // Constructor for an empty project.
    Editor();
};

#endif