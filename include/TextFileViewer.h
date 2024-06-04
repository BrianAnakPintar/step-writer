// TextFileViewer.hpp
#ifndef TEXT_FILE_VIEWER_HPP
#define TEXT_FILE_VIEWER_HPP

#include <ftxui/component/component.hpp>
#include "ftxui/component/event.hpp"
#include "Document.h"
#include <memory>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace ftx = ftxui;
namespace fs = std::filesystem;

class TextFileViewer : public ftx::ComponentBase {
public:
    TextFileViewer(const std::string& file_path);
    ftx::Element Render() override;
    bool UpdateDocument(const std::string& path);
    bool HandleInput(ftxui::Event event);
    Document document;

    enum EditorModes {
        Normal,
        Insert,
        Visual,
    };
    EditorModes editorMode;
    void SignalEditorModeChange();

private:
    void UpdateCursor();
    void ScrollDown();
    void ScrollUp();
    int GetBottomY();
    int GetRightX();
    int GetTransformedX(int x);
    bool NormalModeInputHandler(ftxui::Event event);
    bool InsertModeInputHandler(ftxui::Event event);
    std::string file_path;
    int cursorX, cursorY;
    int numPadding;

    std::vector<ftxui::Event> keys;
    
    int viewportStart; // Track the start position of the viewport


};

#endif // TEXT_FILE_VIEWER_HPP
