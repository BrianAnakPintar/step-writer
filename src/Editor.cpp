#include "Editor.h"
#include "Utils.h"
#include "DirectoryFile.hpp"
#include "DataFile.hpp"

// TODO: REFACTORING NOTES, APPLY MVC PATTERN.

// Similar to the `ls` command that outputs all files in a directory. 
// It takes a path string, and outputs a vector of strings of all files in the path.
File* Editor::ListFiles(const std::string& path) {
    File *project_file = new DirectoryFile("project", path);
    if (project_file->OpenFile())
        return project_file;
    return project_file;
}

std::string msg = "Use the button the left to open";

void Editor::OpenFile(std::vector<std::string> files, int idx, std::string& base_path) {
    // TODO Check if it's a directory. If so, then populate the collapsible. Otherwise open the file.

    std::string fileName = base_path + "/" + files[idx];
    tfv->UpdateDocument(fileName);
}

Editor& Editor::GetInstance() {
    static Editor instance;
    return instance;
}

ftxui::ScreenInteractive& Editor::GetScreen() {
    return screen;
}

// Definition of the modal component. The details are not important.
ftxui::Component ModalComponent(std::function<void()> do_nothing,
                                std::function<void()> hide_modal) {
    using namespace ftxui;
    auto component = Container::Vertical({
        Button("Do nothing", do_nothing),
        Button("Quit modal", hide_modal),
    });
    // Polish how the two buttons are rendered:
    component |= Renderer([&](Element inner) {
        return vbox({
                text("Modal component "),
                separator(),
                inner,
            })
            | size(WIDTH, GREATER_THAN, 30)
            | border;
    });
    return component;
}

bool Editor::SanityChecks(ftxui::Event event) {
    if (editorState == Workspace) {
        if (event == ftxui::Event::Special({16})) {   // If Ctrl + P is pressed
            if (tfv->isDirty()) {
                if (tfv->SaveFile())
                    editorStateInt = 0;
            }  
            return true;
        }
    }
    
    return false;
}

void Editor::StartApplication() {
    using namespace ftxui;
    Elements elements;

    // For MainMenu Screen.
    for (std::string s : title_small) {
        elements.push_back(text(s) | color(Color::Blue));
    }

    auto buttons = Container::Vertical({
        Button("Open File", [&] {HomeButton(0);}, CustomButton()) | center,
        Button("Settings", [&] {HomeButton(1);}, CustomButton()) | center,
        Button("Quit", [&] {HomeButton(2);}, CustomButton()) | center,
        
    });
    // ======================
    // Render.
    auto MainMenuComponent = Renderer(buttons, [&] {
        return vbox({
            vbox(elements),
            text("") | center,
            text("") | center,
            buttons->Render() | center,
        }) | center;
    });


    // For Main Editor.
    char buffer[FILENAME_MAX];
    getcwd(buffer, FILENAME_MAX);
    std::string currentDirectory(buffer);

    // Access the parent directory
    fs::path parentPath = fs::path(currentDirectory).parent_path();
    // parentPath /= "src";

    std::string parentDir = parentPath.string();

    File* root = ListFiles(parentDir);

    auto x = ftxui::Container::Vertical({});

    tfv = new TextFileViewer("");

    mainScreen = ftxui::Renderer([&](bool focused) {
        if (focused) {
            tfv->SignalEditorModeChange();
            return tfv->Render();
        }
        else {
            status_EditorMode = "EXPLORER";
            return tfv->Render();
        }
    });

    mainScreen |= ftxui::CatchEvent([&](ftxui::Event event) {
        return tfv->HandleInput(event);
    });

    explorer = root->RenderTree();
    auto t = Renderer(explorer, [&] {
        return explorer->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, GetScreen().dimy()-3);
    });

    mainScreen = ResizableSplitLeft(t, mainScreen, &left_size);

    // TODO: Delete Later
    status_msg = "Press CTRL + P to go back to main menu. | ʕ•́ᴥ•̀ʔっ";
    auto status_bar = Renderer([&] {
        return hbox(text(status_EditorMode) | color(Color::Grey0) | bgcolor(Color::NavajoWhite1) | bold,
                    color(Color::Grey100, text(" " + status_msg + " ")));
    });
    
    auto composition = Container::Vertical({mainScreen, status_bar});

    showTerminal = false;
    auto show_modal = [&] { showTerminal = true; };
    auto hide_modal = [&] { showTerminal = false; };
    auto modal_component = ModalComponent(hide_modal, hide_modal);

    // Create a vertical layout with mainScreen and status_bar
    auto main_with_status_bar = Renderer([&] {
        return vbox({
            mainScreen->Render() | flex_grow,
            hbox({
                status_bar->Render(),
            }) | size(HEIGHT, EQUAL, 1) | bgcolor(Color::Blue)
        });
    });

    auto EditorComponent = ftxui::Renderer(composition, [&] { return window(text("Editor.cpp") | color(Color::Blue) | hcenter | bold, main_with_status_bar->Render());});
    EditorComponent |= Modal(modal_component, &showTerminal);

    EditorComponent |= ftxui::CatchEvent([&](ftxui::Event event) {
        return SanityChecks(event);
    });


    auto final_renderer = Container::Tab({
    MainMenuComponent,
    EditorComponent
    }, &editorStateInt); // Add bgcolor here if you want "customization"

    screen.Loop(final_renderer);

    // Deallocation.
    delete tfv;
}

void Editor::ChangeEditorStatus(std::string s) {
    status_EditorMode = s;
}

void Editor::SetEditorMsg(std::string s) {
    status_msg = s;
}

void Editor::HomeButton(int type) {
    if (type == 0) {
        editorState = Workspace;
        editorStateInt = 1;
    }
        
    else if (type == 1) {
        editorState = Settings;
        editorStateInt = 2;
    }
    else {
        screen.Exit();
    }
}

void Editor::UpdateDoc(const std::string& path) {
    if (tfv == nullptr)
        return;
    tfv->UpdateDocument(path);
}
