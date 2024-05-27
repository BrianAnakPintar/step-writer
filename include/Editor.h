#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "TextFileViewer.h"


#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

class Editor {
private:
    // Private Vars
    static Editor instance;
    enum EditorState {
        MainMenu,
        Workspace,
        Settings
    };
    EditorState editorState;
    int editorStateInt; // Redundant but lets just see if it works.

    Editor() {}

    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
    ftxui::Component explorer;
    TextFileViewer* tfv;
    ftxui::Component mainScreen;

    // Private Functions
    void LoadCurrFolder();
    std::vector<std::string> ListFiles(const std::string& path);
    void StartMenuUI();
    void OpenFile(std::vector<std::string> files, int idx, std::string& base_path);
    void ScreenHelper(ftxui::Component);

    bool SanityChecks(ftxui::Event event);

    void HomeButton(int type);
    ftxui::Component buttonsHelper(ftxui::Component &menu, ftxui::Component &workspace, ftxui::Component &settings);

public:
    // Opens a file/folder in the editor.
    void OpenFileEditor(const std::string &path);

    // The first function called, the point of entry.
    void StartApplication();

    // Constructor for an empty project.
    Editor(const Editor&) = delete;
    void operator=(const Editor&) = delete;
    static Editor& GetInstance();
    ftxui::ScreenInteractive& GetScreen();

    // Variables
    bool quitSignal;
    int left_size = 20;

    int focusX, focusY;
};

#endif