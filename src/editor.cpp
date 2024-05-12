#include "../include/Editor.h"
#include "../include/TextFileViewer.h"

/* REFACTORING | Planned Things.

1. Convert the Editor into a Singleton which should also contain various variable declarations.

2. Refactor Duplicate Code. Create Clean and Nice to read code Especially for UI stuff.

3. Make shit look nice.

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

void Editor::OpenFile(std::vector<std::string> files, int idx) {
    std::string fileName = files[idx];

    std::ifstream file(fileName);
    if (!file.is_open()) {
        msg = "Error Opening File. Try Again";
        return;
    }

    std::vector<std::string> fileContent;
    std::string line;
    while (std::getline(file, line)) {
        fileContent.push_back(line);
    }
    file.close();

    auto tfv = ftxui::Make<TextFileViewer>(fileName);
    auto viewer = ftxui::Renderer([&](bool focused) {
        if (focused) {
            return tfv->Render();
        }
        else
            return ftxui::text(" Focusable renderer() ");
    });

    viewer |= ftxui::CatchEvent([&](ftxui::Event event) {
        return tfv->HandleInput(event);
    });

    mainScreen = viewer;

    ScreenHelper(mainScreen);
}

// Gets the current directory and lists all item.
void Editor::LoadCurrFolder() {
    char buffer[FILENAME_MAX];
    getcwd(buffer, FILENAME_MAX);
    std::string currentDirectory(buffer);
    std::vector<std::string> items = ListFiles(currentDirectory);
    int selected = 0;

    ftxui::MenuOption option;
    option.on_enter = [&] { OpenFile(items, selected); };
    explorer = ftxui::Menu(&items, &selected, option);

    auto middle = ftxui::Renderer([&] {return ftxui::vbox({
            ftxui::text(msg),
        }) | ftxui::center;} );
    ScreenHelper(middle);
}

void Editor::StartMenuUI() {
    std::string label = "Click to quit";
    ftxui::Component button = ftxui::Button(&label, [&] { LoadCurrFolder(); });

    auto middle = ftxui::Renderer([&] {return ftxui::vbox({
        ftxui::text(msg),
    }) | ftxui::center;} );

    explorer = button;
    ScreenHelper(middle);
}
void Editor::ScreenHelper(ftxui::Component middle) {
    mainScreen = middle; 
    mainScreen = ResizableSplitLeft(explorer, mainScreen, &left_size);

    screen.Clear();
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

void Editor::OpenFileEditor(const std::string &path) {
    StartMenuUI();
}

bool Editor::SanityChecks(ftxui::Event event) {
    if (event == ftxui::Event::Character('q')) {
        screen.ExitLoopClosure();
        return true;
    }

    return false;
}