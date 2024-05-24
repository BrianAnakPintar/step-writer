#include "../include/TextFileViewer.h"
#include "../include/Editor.h"
#include "ftxui/component/event.hpp"
#include "ftxui/screen/screen.hpp"

TextFileViewer::TextFileViewer(const std::string& file_path) : file_path_(file_path) {
    Editor& editor = Editor::GetInstance();
    cursorX = editor.left_size + 1;
    cursorY = 1;
    viewportStart_ = 0;
    document = Document(file_path);
}

bool TextFileViewer::UpdateDocument(const std::string& path) {
    file_path_ = path;
    Editor& editor = Editor::GetInstance();
    cursorX = editor.left_size + 1;
    cursorY = 1;
    viewportStart_ = 0;
    document = Document(path);
    // TODO: We have to add a Fallback/Failure when Document does not successfully open a file.
    return true;
}

ftx::Element TextFileViewer::Render() {
    using namespace ftx;
    Elements elements;

    int max_number_length = std::to_string(document.GetRowsLength()).size(); // Get the length of the longest line number.
    numPadding = max_number_length;

    int viewportEnd = std::min(viewportStart_ + Editor::GetInstance().GetScreen().dimy() - 2, static_cast<int>(document.GetRowsLength()));

    for (int i = viewportStart_; i < viewportEnd; i++) {
        auto line_number = std::to_wstring(i+1);
        std::wstring padded_number = std::wstring(max_number_length - line_number.size(), L' ') + line_number;
        auto line_elem = hbox(
            text(padded_number + L"  ") | color(Color::Blue) | dim,
            text(document.GetRows()[i])
        );
        elements.push_back(line_elem);
    }
    

    return vbox(elements) | frame;
}

bool TextFileViewer::HandleInput(ftxui::Event event) {
    if (event == ftxui::Event::Character('h')) {
        cursorX -= 1;
        UpdateCursor();
        return true;
    }
    if (event == ftxui::Event::Character('l')) {
        cursorX += 1;
        UpdateCursor();
        return true;
    }
    if (event == ftxui::Event::Character('j')) {
        cursorY += 1;
        UpdateCursor();
        return true;
    }
    if (event == ftxui::Event::Character('k')) {
        cursorY -= 1;
        UpdateCursor();
        return true;
    }

    UpdateCursor();
    return false;
}

void TextFileViewer::UpdateCursor() {
    ftxui::Screen::Cursor cursor;
    auto& editor = Editor::GetInstance();

    // Bounds Checking to ensure that the cursor is at the appropriate place.
    if (cursorX < editor.left_size + numPadding + 3) {
        cursorX = editor.left_size + numPadding + 3;
    } else if (cursorX >= editor.GetScreen().dimx()) {
        cursorX = editor.GetScreen().dimx() - 1;
    } else if (cursorY <= 0) {
        cursorY = 1;
        ScrollUp();
    } else if (cursorY >= editor.GetScreen().dimy()) {
        cursorY = editor.GetScreen().dimy() - 1;
        ScrollDown();
    }

    if (editor.focusY <= 0)
        editor.focusY = 0;
    if (editor.focusX <= 0)
        editor.focusX = 0;

    cursor.x = cursorX;
    cursor.y = cursorY;

    cursor.shape = ftxui::Screen::Cursor::Shape::Block;

    editor.GetScreen().SetCursor(cursor);
}

void TextFileViewer::ScrollDown() {
    // Increment the viewport start to scroll down
    viewportStart_++;
    // Limit the viewport start to prevent scrolling past the end of the file
    if (viewportStart_ > static_cast<int>(document.GetRowsLength()) - Editor::GetInstance().GetScreen().dimy() + 2) {
        viewportStart_ = static_cast<int>(document.GetRowsLength()) - Editor::GetInstance().GetScreen().dimy() + 2;
    }
}

void TextFileViewer::ScrollUp() {
    // Decrement the viewport start to scroll up
    viewportStart_--;
    // Limit the viewport start to prevent scrolling past the beginning of the file
    if (viewportStart_ < 0) {
        viewportStart_ = 0;
    }
}