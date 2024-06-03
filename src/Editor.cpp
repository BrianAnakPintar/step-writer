#include "../include/Editor.h"
#include "../include/Utils.h"

/* REFACTORING | Notes for later.

I am planning to make things work for now.
The design in which I am making and writing these things is horrible.

Ideally, we would use the MVC pattern where we split our program into
2 parts, the UI and the main functionality. This would greatly enhance our code!
Additionally, I would also advise myself to add unit tests ASAP. The longer I delay,
the more I might regret this ;-;

Next, there are lots of redundant code here with unused function, try to get rid of them.
Also use proper function calls where necessary.

*/

// Similar to the `ls` command that outputs all files in a directory. 
// It takes a path string, and outputs a vector of strings of all files in the path.
std::vector<std::string> Editor::ListFiles(const std::string& path) {
    std::vector<std::string> items;
    DIR *dir = opendir(path.c_str());
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr) {
            items.push_back(entry->d_name);
        }
        closedir(dir);
    }
    return items;
}

std::string msg = "Use the button the left to open";

void Editor::OpenFile(std::vector<std::string> files, int idx, std::string& base_path) {
    std::string fileName = base_path + "/" + files[idx];
    tfv->UpdateDocument(fileName);
}

// Gets the current directory and lists all item.
void Editor::LoadCurrFolder() {
    char buffer[FILENAME_MAX];
    getcwd(buffer, FILENAME_MAX);
    std::string currentDirectory(buffer);

    // Access the parent directory
    fs::path parentPath = fs::path(currentDirectory).parent_path();

    std::string parentDir = parentPath.string();

    std::vector<std::string> items = ListFiles(parentDir);
    int selected = 0;

    ftxui::MenuOption option;
    option.on_enter = [&] { OpenFile(items, selected, parentDir); };
    explorer = ftxui::Menu(&items, &selected, option);

    auto middle = ftxui::Renderer([&] {return ftxui::vbox({
            ftxui::text(msg),
        }) | ftxui::center;} );
    ScreenHelper(middle);
}

void Editor::ScreenHelper(ftxui::Component middle) {
    mainScreen = middle; 
    mainScreen = ResizableSplitLeft(explorer, mainScreen, &left_size);

    screen.Clear();
    screen.ExitLoopClosure();
    auto renderer = ftxui::Renderer(mainScreen, [&] { return mainScreen->Render() | ftxui::border; });

    renderer |= ftxui::CatchEvent([&](ftxui::Event event) {
        return SanityChecks(event);
    });

    screen.Loop(renderer);
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
    if (event.is_mouse())
        return true;
    if (editorState == Workspace) {
        if (event == ftxui::Event::Character('q')) {
            editorStateInt = 0;
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

    std::string parentDir = parentPath.string();

    std::vector<std::string> items = ListFiles(parentDir);
    int selected = 0;

    ftxui::MenuOption option;
    option.on_enter = [&] { OpenFile(items, selected, parentDir); };
    explorer = ftxui::Menu(&items, &selected, option);

    tfv = new TextFileViewer("");

    // mainScreen = ftxui::Renderer([&] {return ftxui::vbox({
    //         ftxui::text(msg),
    //     }) | ftxui::center;} );

    mainScreen = ftxui::Renderer([&](bool focused) {
        if (focused) {
            return tfv->Render();
        }
        else
            return tfv->Render();
    });

    mainScreen |= ftxui::CatchEvent([&](ftxui::Event event) {
        return tfv->HandleInput(event);
    });

    auto status_bar = hbox(text(" NORMAL ") | bgcolor(Color::NavajoWhite1));

    mainScreen = ResizableSplitLeft(explorer, mainScreen, &left_size);
    showTerminal = false;
    auto show_modal = [&] { showTerminal = true; };
    auto hide_modal = [&] { showTerminal = false; };
    auto modal_component = ModalComponent(hide_modal, hide_modal);

    // Create a vertical layout with mainScreen and status_bar
    auto main_with_status_bar = Renderer([&] {
        return vbox({
            mainScreen->Render() | flex_grow,
            hbox({
                status_bar,
            }) | size(HEIGHT, EQUAL, 1) | bgcolor(Color::Blue)
        });
    });

    auto EditorComponent = ftxui::Renderer(mainScreen, [&] { return window(text("Editor.cpp") | color(Color::Blue) | hcenter | bold, main_with_status_bar->Render());});
    EditorComponent |= Modal(modal_component, &showTerminal);

    EditorComponent |= ftxui::CatchEvent([&](ftxui::Event event) {
        return SanityChecks(event);
    });


    auto final_renderer = Container::Tab({
    MainMenuComponent,
    EditorComponent
    }, &editorStateInt);

    screen.Loop(final_renderer);

    // Deallocation.
    delete tfv;
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