#include "../include/editor.h"
#include "../include/TextFileViewer.h"


Editor::Editor() {
    StartMenuUI();
}

Editor::Editor(const std::string &path) {
    StartMenuUI();
}

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
    screen.ExitLoopClosure();
    auto viewer = ftxui::Make<TextFileViewer>(fileName);
    mainScreen = viewer;
    int left_size = 20;
    mainScreen = ResizableSplitLeft(explorer, mainScreen, &left_size);
    auto renderer = ftxui::Renderer(mainScreen, [&] { return mainScreen->Render() | ftxui::border; });
    screen.Loop(renderer);
}

// Gets the current directory and lists all item.
void Editor::LoadCurrFolder() {
    char buffer[FILENAME_MAX];
    getcwd(buffer, FILENAME_MAX);
    std::string currentDirectory(buffer);
    std::vector<std::string> items = ListFiles(currentDirectory);
    int selected = 0;

    screen.ExitLoopClosure();
    ftxui::MenuOption option;
    option.on_enter = [&] { OpenFile(items, selected); };
    explorer = ftxui::Menu(&items, &selected, option);

    auto middle = ftxui::Renderer([&] {return ftxui::vbox({
            ftxui::text(msg),
        }) | ftxui::center;} );
    int left_size = 20;
    mainScreen = middle;
    mainScreen = ResizableSplitLeft(explorer, mainScreen, &left_size);
    auto renderer = ftxui::Renderer(mainScreen, [&] { return mainScreen->Render() | ftxui::border; });
    screen.Loop(renderer);
}

void Editor::StartMenuUI() {
    std::string label = "Click to quit";
    ftxui::Component button = ftxui::Button(&label, [&] { LoadCurrFolder(); });

    auto middle = ftxui::Renderer([&] {return ftxui::vbox({
        ftxui::text(msg),
    }) | ftxui::center;} );

    explorer = button;
    int left_size = 20;
    mainScreen = middle;
    mainScreen = ResizableSplitLeft(explorer, mainScreen, &left_size);
    auto renderer = ftxui::Renderer(mainScreen, [&] { return mainScreen->Render() | ftxui::border; });
    screen.Loop(renderer);
}