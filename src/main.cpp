#include <filesystem>
#include <iostream>
#include <string>
#include "Editor.h"

int main(int argc, char *argv[]) {
    Editor& editor = Editor::GetInstance();
    if (argc == 2) {
        std::string path = argv[1];
        fs::path filepath = fs::absolute(path);
        std::cout << "Path: " << filepath << std::endl;
        editor.StartApplication(path);
        std::cout << "Good Bye!" << std::endl;
    } else {
        std::cout << "Invalid arguments. Use ./step-writer <path>" << std::endl;
    }
    return 0;
}

