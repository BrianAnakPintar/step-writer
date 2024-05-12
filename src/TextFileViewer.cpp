#include "../include/TextFileViewer.h"
#include "../include/Editor.h"
#include "ftxui/component/event.hpp"
#include "ftxui/screen/screen.hpp"

TextFileViewer::TextFileViewer(const std::string& file_path) : file_path_(file_path) {
    Editor& editor = Editor::GetInstance();
    cursorX = editor.left_size + 1;
    cursorY = 0;
    LoadFile();
}

ftx::Element TextFileViewer::Render() {
    using namespace ftx;
    Elements elements;
    elements.push_back(text(L"Cursor Position: (" + std::to_wstring(cursorX) + L", " + std::to_wstring(cursorY) + L")"));
    elements.push_back(text(s));
    int max_number_length = std::to_string(file_content_.size()).size(); // Get the length of the longest line number.

    for (int i = 0; i < file_content_.size(); i++) {
        auto line_number = std::to_wstring(i+1);
        std::wstring padded_number = std::wstring(max_number_length - line_number.size(), L' ') + line_number;
        auto line_elem = hbox(
            text(padded_number + L"  ") | color(Color::Blue) | dim,
            text(file_content_[i])
        );
        elements.push_back(line_elem);
    }
    

    return vbox(elements) | vscroll_indicator | frame;
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
    auto& editor = Editor::GetInstance();

    if (event == ftxui::Event::Character('h')) {
        cursorX -= 1;
        ftxui::Screen::Cursor cursor;
        cursor.x = cursorX;
        cursor.y = cursorY;

        cursor.shape = ftxui::Screen::Cursor::Shape::Block;

        editor.GetScreen().SetCursor(cursor);
        return true;
    }
    if (event == ftxui::Event::Character('l')) {
        cursorX += 1;
        ftxui::Screen::Cursor cursor;
        cursor.x = cursorX;
        cursor.y = cursorY;

        cursor.shape = ftxui::Screen::Cursor::Shape::Block;
        editor.GetScreen().SetCursor(cursor);

        return true;
    }
    if (event == ftxui::Event::Character('j')) {
        cursorY += 1;
        ftxui::Screen::Cursor cursor;
        cursor.x = cursorX;
        cursor.y = cursorY;

        cursor.shape = ftxui::Screen::Cursor::Shape::Block;
        editor.GetScreen().SetCursor(cursor);

        return true;
    }
    if (event == ftxui::Event::Character('k')) {
        cursorY -= 1;
        ftxui::Screen::Cursor cursor;
        cursor.x = cursorX;
        cursor.y = cursorY;

        cursor.shape = ftxui::Screen::Cursor::Shape::Block;
        editor.GetScreen().SetCursor(cursor);

        return true;
    }

    return false;
}