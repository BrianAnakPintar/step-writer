#include "../include/TextFileViewer.h"

TextFileViewer::TextFileViewer(const std::string& file_path) : file_path_(file_path) {
    LoadFile();
}

ftx::Element TextFileViewer::Render() {
    using namespace ftx;
    Elements elements;
    for (const auto& line : file_content_) {
        elements.push_back(text(line) | flex);
    }
    return vbox(std::move(elements));
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
