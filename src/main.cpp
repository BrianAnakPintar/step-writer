#include <iostream>
#include "../include/Editor.h"

int main(int argc, char *argv[]) {
    Editor& editor = Editor::GetInstance();
    editor.StartApplication();
    std::cout << "Sanity Check" << std::endl;
    return 0;
}
