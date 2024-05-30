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
    editorMode = Normal;
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

    Box cBox;

    int max_number_length = std::to_string(document.GetRowsLength()).size(); // Get the length of the longest line number.
    numPadding = max_number_length;
    auto msgTmp = text(L"(" + std::to_wstring(cursorX) + L", " + std::to_wstring(cursorY) + L")" + std::to_wstring(Editor::GetInstance().GetScreen().dimy()));
    int viewportEnd = std::min(viewportStart_ + Editor::GetInstance().GetScreen().dimy() - 2, static_cast<int>(document.GetRowsLength()));

    for (int i = viewportStart_; i < viewportEnd; i++) {
        auto line_number = std::to_wstring(i+1);
        std::wstring padded_number = std::wstring(max_number_length - line_number.size(), L' ') + line_number;
        auto line_elem = hbox(
            text(padded_number + L"  ") | color(Color::Blue) | dim,
            text(document.GetRows()[i].getText())
        );
        elements.push_back(line_elem);
    }

//    auto debug = vbox(text(L"Debug Info:"),
//                      text(L"cursorX: " + std::to_wstring(cursorX)),
//                      text(L"cursorY: " + std::to_wstring(cursorY)),
//                      text(L"dimX: " + std::to_wstring(Editor::GetInstance().GetScreen().dimx())),
//                      text(L"dimY: " + std::to_wstring(Editor::GetInstance().GetScreen().dimy())),
//                      text(L"View-S: " + std::to_wstring(viewportStart_)),
//                      text(L"View-E: " + std::to_wstring(viewportEnd)) | frame)

    return hbox(vbox(elements) | frame);
}

bool TextFileViewer::NormalModeInputHandler(ftxui::Event event) {
    bool handled = true;
    if (event == ftxui::Event::Character('h') || event == ftxui::Event::ArrowLeft) {
        cursorX -= 1;
    }
    else if (event == ftxui::Event::Character('l') || event == ftxui::Event::ArrowRight) {
        cursorX += 1;
    }
    else if (event == ftxui::Event::Character('j') || event == ftxui::Event::ArrowDown) {
        cursorY += 1;
    }
    else if (event == ftxui::Event::Character('k') || event == ftxui::Event::ArrowUp) {
        cursorY -= 1;
    }
    else if (event == ftxui::Event::Character('i')) {
        editorMode = Insert;
    } else {
        handled = false;
    }

    UpdateCursor();
    return handled;
}

bool TextFileViewer::InsertModeInputHandler(ftxui::Event event) {
    bool handled = true;
    if (event == ftxui::Event::Escape) {
        editorMode = Normal;
    }
    else if (event == ftxui::Event::ArrowLeft) {
        cursorX -= 1;
    }
    else if (event == ftxui::Event::ArrowRight) {
        cursorX += 1;
    }
    else if (event == ftxui::Event::ArrowDown) {
        cursorY += 1;
    }
    else if (event == ftxui::Event::ArrowUp) {
        cursorY -= 1;
    }
    else if (event.is_character()) {
        // Manually set the appropriate indexes
        UpdateCursor();
        Editor& editor = Editor::GetInstance();
        document.Insert(cursorX - editor.left_size - numPadding - 3, cursorY - 1 + viewportStart_, event.character()[0]);
        cursorX++;
    }
    else if (event == ftxui::Event::Backspace) {
        UpdateCursor();
        Editor& editor = Editor::GetInstance();
        int newX = cursorX - editor.left_size - numPadding - 4;
        int newY = cursorY - 1;
        if (newX <= -1) {
            if (cursorY != 0)
                cursorX = document.GetRows()[cursorY-2].getLen() + editor.left_size + numPadding + 4;
            cursorY--;
        }
        document.Delete(newX, newY + viewportStart_);

        cursorX--;
    }
    else if (event == ftxui::Event::Return) {
        Editor& editor = Editor::GetInstance();
        document.ReturnKey(cursorX - editor.left_size - numPadding - 4, cursorY - 1 + viewportStart_);
        cursorY++;
        cursorX = cursorX - editor.left_size - numPadding - 4;

    } else {
        handled = false;
    }
    UpdateCursor();
    return handled;
}

bool TextFileViewer::HandleInput(ftxui::Event event) {
    if (editorMode == Normal)
        return NormalModeInputHandler(event);
    else if (editorMode == Insert)
        return InsertModeInputHandler(event);
    else if (editorMode == Visual)
        return false;
    return false;
}

void TextFileViewer::UpdateCursor() {
    auto& editor = Editor::GetInstance();

    // Bounds Checking to ensure that the cursor is at the appropriate place.
    if (cursorX < editor.left_size + numPadding + 3) {
        cursorX = editor.left_size + numPadding + 3;
    } else if (cursorX >= editor.GetScreen().dimx()) {
        cursorX = editor.GetScreen().dimx() - 1;
    } else if (cursorY <= 0) {
        cursorY = 1;
        ScrollUp();
    } else if (cursorY >= editor.GetScreen().dimy() - 1) {
        cursorY = editor.GetScreen().dimy() - 2;
        ScrollDown();
    }

    // Bad structure but screw it... Bounds Check for line
    int line_size = document.GetRows()[cursorY - 1 + viewportStart_].getLen();
    if (cursorX > editor.left_size + numPadding + 3 + line_size) {
        // cursorX = editor.left_size + numPadding + 3 + line_size;

        cursorX = editor.left_size + numPadding + 3;
        cursorY++;
        UpdateCursor();
    }
    if (cursorY + viewportStart_ > document.GetRowsLength())
        cursorY = std::min(document.GetRowsLength(), editor.GetScreen().dimy() - 1);

    if (editor.focusY <= 0)
        editor.focusY = 0;
    if (editor.focusX <= 0)
        editor.focusX = 0;

    ftxui::Screen::Cursor cursor = {cursorX + 1, cursorY};

    if (editorMode == Normal)
        cursor.shape = ftxui::Screen::Cursor::Block;
    else if (editorMode == Insert)
        cursor.shape = ftxui::Screen::Cursor::BarBlinking;

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