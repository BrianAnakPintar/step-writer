#include "../include/TextFileViewer.h"
#include "../include/Editor.h"

TextFileViewer::TextFileViewer(const std::string& file_path) : file_path_(file_path) {
    cursorX = 0;
    cursorY = 0;
    LoadFile();
}

ftx::Element TextFileViewer::Render() {
    using namespace ftx;
    Elements elements;
    elements.push_back(text(L"Cursor Position: (" + std::to_wstring(cursorX) + L", " + std::to_wstring(cursorY) + L")"));
    elements.push_back(text(s));
    for (const auto& line : file_content_) {
        elements.push_back(text(line) | flex);
    }

    return vbox(elements);
}

void TextFileViewer::LoadFile() {
    std::ifstream file(file_path_);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            file_content_.push_back(line);
        }
        file.close();
    } else {
        file_content_.push_back("Failed to open file: " + file_path_);
    }
}

bool TextFileViewer::HandleInput(ftxui::Event event) {
    if (event == ftxui::Event::Character('h')) {
        cursorX -= 1;
        return true;
    }
    if (event == ftxui::Event::Character('l')) {
        cursorX += 1;
        return true;
    }
    if (event == ftxui::Event::Character('j')) {
        cursorY += 1;
        return true;
    }
    if (event == ftxui::Event::Character('k')) {
        cursorY -= 1;
        return true;
    }

    return false;
}