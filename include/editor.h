#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include "document.h"
#include <dirent.h>
#include <unistd.h>
#include <fstream>

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"


class Editor {
    
private:
    // Private Vars
    static Editor instance;
    static std::mutex mutex;
    Editor() {}

    Document document;
    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
    ftxui::Component explorer;
    ftxui::Component mainScreen;

    // Private Functions
    void LoadCurrFolder();
    std::vector<std::string> ListFiles(const std::string& path);
    void StartMenuUI();
    void OpenFile(std::vector<std::string> files, int idx);
    void ScreenHelper(ftxui::Component);

    bool SanityChecks(ftxui::Event event);

public:
    // Opens a file/folder in the editor.
    void OpenFileEditor(const std::string &path);
    // Constructor for an empty project.
    Editor(const Editor&) = delete;
    void operator=(const Editor&) = delete;
    static Editor& GetInstance();

    // Variables
    bool quitSignal;
};

#endif