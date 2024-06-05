#include "DataFile.hpp"
#include "Editor.h"

DataFile::DataFile(const std::string& name, const std::string& path) : File(name, path, FileType::Data) {

}

bool DataFile::OpenFile() {
    // Actually we don't have to implement this to make it work.
    Editor& editor = Editor::GetInstance();
    editor.UpdateDoc(file_path);
    return true;
}

ftxui::ButtonOption Cool() {
    using namespace ftxui;

    ButtonOption option;
    option.transform = [](const EntryState& s) {
        const std::string t = s.focused ? "> " + s.label + " "  //
                                        : "  " + s.label + " ";
        if (s.focused)
            return text(t) | color(Color::Blue);
        return text(t);
    };
    return option;
}


ftxui::Component DataFile::RenderTree() {
    using namespace ftxui;
    return Button(file_name, [this] { OpenFile(); }, Cool()); // Capture 'this' by value instead of reference.
}