#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <dirent.h>
#include <unistd.h>

#include "TextFileViewer.h"
#include "File.hpp"

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"


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
    TextFileViewer *tfv;
    ftxui::Component mainScreen;

    // Private Functions
    File* ListFiles(const std::string& path);
    void StartMenuUI();
    // Opens a file
    void OpenFile(std::vector<std::string> files, int idx, std::string& base_path);

    bool SanityChecks(ftxui::Event event);

    std::string status_EditorMode;
    std::string status_msg;

    void HomeButton(int type);
    ftxui::Component buttonsHelper(ftxui::Component &menu, ftxui::Component &workspace, ftxui::Component &settings);

public:
    // Opens a file/folder in the editor.
    void OpenFileEditor(const std::string &path);

    // The first function called, the point of entry.
    void StartApplication(std::string &path);

    // Constructor for an empty project.
    Editor(const Editor&) = delete;
    void operator=(const Editor&) = delete;
    static Editor& GetInstance();
    ftxui::ScreenInteractive& GetScreen();

    void ChangeEditorStatus(std::string s);
    void SetEditorMsg(std::string s);

    void UpdateDoc(const std::string& path);

    // Variables
    bool quitSignal;
    bool showTerminal;
    int left_size = 20;

    int focusX, focusY;
};

#endif
