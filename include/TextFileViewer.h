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

private:
    void UpdateCursor();
    void ScrollDown();
    void ScrollUp();

    std::string file_path_;

    int cursorX, cursorY;
    int numPadding;

    std::vector<ftxui::Event> keys;
    
    int viewportStart_; // Track the start position of the viewport
};

#endif // TEXT_FILE_VIEWER_HPP
