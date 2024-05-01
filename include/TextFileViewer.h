// TextFileViewer.hpp
#ifndef TEXT_FILE_VIEWER_HPP
#define TEXT_FILE_VIEWER_HPP

#include <ftxui/component/component.hpp>
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

private:
    void LoadFile();

    std::string file_path_;
    std::vector<std::string> file_content_;
};

#endif // TEXT_FILE_VIEWER_HPP
