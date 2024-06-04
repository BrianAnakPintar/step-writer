#include "TextFileViewer.h"
#include "Editor.h"
#include "ftxui/component/event.hpp"
#include "ftxui/screen/screen.hpp"

TextFileViewer::TextFileViewer(const std::string& file_path) : file_path(file_path) {
    Editor& editor = Editor::GetInstance();
    cursorX = 0;
    cursorY = 0;
    viewportStart = 0;
    document = Document(file_path);
    editorMode = Normal;
}

bool TextFileViewer::UpdateDocument(const std::string& path) {
    file_path = path;
    Editor& editor = Editor::GetInstance();
    cursorX = 0;
    cursorY = 0;
    viewportStart = 0;
    document = Document(path);
    // TODO: We have to add a Fallback/Failure when Document does not successfully open a file.
    return true;
}

// Returns the smallest y-value a cursorY can have.
int TextFileViewer::GetBottomY() {
    // -3 Because we have the status bar AND the border (up and down).
    return Editor::GetInstance().GetScreen().dimy() - 4;
}

// Returns the largest value cursorX can have.
int TextFileViewer::GetRightX() {
    Editor& editor = Editor::GetInstance();
    return editor.GetScreen().dimx() - editor.left_size;
}

// Accepts a cursorX (starts at 0) and returns the real "Physical" x that cursor should update to.
int TextFileViewer::GetTransformedX(int x) {
    return Editor::GetInstance().left_size + numPadding + 4 + x;
}

ftx::Element TextFileViewer::Render() {
    using namespace ftx;
    Elements elements;

    numPadding = std::to_string(document.GetRowsLength()).size(); // Get the length of the longest line number.
    int viewportEnd = std::min(viewportStart + GetBottomY() + 1, static_cast<int>(document.GetRowsLength()));

    for (int i = viewportStart; i < viewportEnd; i++) {
        auto line_number = std::to_wstring(i+1);
        std::wstring padded_number = std::wstring(numPadding - line_number.size(), L' ') + line_number;
        auto line_elem = hbox(
            text(padded_number + L"  ") | color(Color::Blue) | dim,
            text(document.GetRows()[i].getText())
        );
        elements.push_back(line_elem);
    }
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
    } 
    else if (event == ftxui::Event::Character(' ')) {
        Editor& editor = Editor::GetInstance();
        editor.showTerminal = !editor.showTerminal;
    }
    else {
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
        document.Insert(cursorX, cursorY + viewportStart, event.character()[0]);
        cursorX++;
    }
    else if (event == ftxui::Event::Backspace) {
        UpdateCursor();
        Editor& editor = Editor::GetInstance();
        int newX = cursorX;
        int newY = cursorY;

        if (newY == 0 && newX == 0)
            return true;

        if (newX <= 0) {
            if (cursorY != 0)
                cursorX = document.GetRows()[cursorY-1].getLen();
            cursorY--;
        } else {
            cursorX--;
        }
        document.Delete(newX - 1, newY + viewportStart);
    }
    else if (event == ftxui::Event::Return) {
        Editor& editor = Editor::GetInstance();
        document.ReturnKey(cursorX - 1, cursorY + viewportStart);
        cursorY++;
        cursorX = 0;    // Should include an "Indentation Level in Row."
    }
    else if (event == ftxui::Event::Tab) {
        // This is a very bad way of doing things. But we'll just go with this implementation for now.
        UpdateCursor();
        Editor& editor = Editor::GetInstance();
        document.Insert(cursorX, cursorY + viewportStart, ' ');
        document.Insert(cursorX, cursorY + viewportStart, ' ');
        cursorX += 2;
    }
    else {
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
    if (cursorX < 0) {
        cursorX = 0;
    } else if (cursorX > GetRightX()) {
        cursorX = GetRightX();
    } else if (cursorY < 0) {
        cursorY = 0;
        ScrollUp();
    } else if (cursorY > GetBottomY()) {
        cursorY = GetBottomY();
        ScrollDown();
    }

    // Bad structure but screw it... Bounds Check for line
    int line_size = document.GetRows()[cursorY + viewportStart].getLen();
    if (cursorX > line_size) {
        cursorX = line_size;
    }

    if (cursorY + viewportStart > document.GetRowsLength())
        cursorY = std::min(document.GetRowsLength(), GetBottomY());

    // Deals with cursor shape & finalize.
    ftxui::Screen::Cursor cursor = {GetTransformedX(cursorX), cursorY + 1};
    if (editorMode == Normal)
        cursor.shape = ftxui::Screen::Cursor::Block;
    else if (editorMode == Insert)
        cursor.shape = ftxui::Screen::Cursor::BarBlinking;

    editor.GetScreen().SetCursor(cursor);
}

void TextFileViewer::ScrollDown() {
    // Increment the viewport start to scroll down
    viewportStart++;
    // Limit the viewport start to prevent scrolling past the end of the file
    if (viewportStart > static_cast<int>(document.GetRowsLength()) - Editor::GetInstance().GetScreen().dimy() + 3) {
        viewportStart = static_cast<int>(document.GetRowsLength()) - Editor::GetInstance().GetScreen().dimy() + 3;
    }
}

void TextFileViewer::ScrollUp() {
    // Decrement the viewport start to scroll up
    viewportStart--;
    // Limit the viewport start to prevent scrolling past the beginning of the file
    if (viewportStart < 0) {
        viewportStart = 0;
    }
}

void TextFileViewer::SignalEditorModeChange() {
    std::string final_str;
    switch (editorMode) {
        case Normal:
            final_str = "NORMAL";
            break;
        case Insert:
            final_str = "INSERT";
            break;
        case Visual:
            final_str = "VISUAL";
            break;
        default:
            final_str = "ERROR";
            break;
    }
    final_str = " " + final_str + " ";
    Editor& editor = Editor::GetInstance();
    editor.ChangeEditorStatus(final_str);
}