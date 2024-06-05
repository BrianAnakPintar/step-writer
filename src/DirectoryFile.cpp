#include "DirectoryFile.hpp"
#include "DataFile.hpp"
#include <filesystem>

DirectoryFile::DirectoryFile(const std::string& name, const std::string& path) : File(name, path, FileType::Directory) {}

// bool DirectoryFile::OpenFile() {
//     namespace fs = std::filesystem;

//     // Assuming file_path is a std::string containing the path to the directory
//     for (const auto& entry : fs::directory_iterator(file_path)) {
//         File *file;
//         if (entry.is_directory()) {
//             // Entry is a directory
//             file = new DirectoryFile(entry.path().filename().string(), entry.path().string());
//             file->OpenFile();   // Just for funsies, fill the tree.
//         } else {
//             // Entry is not a directory
//             file = new DataFile(entry.path().filename().string(), entry.path().string());
//         }
//         child.push_back(file);
//     }
//     return 1;
// }

bool DirectoryFile::OpenFile() {
    namespace fs = std::filesystem;

    // Assuming file_path is a std::string containing the path to the directory
    // First, iterate over directories
    for (const auto& entry : fs::directory_iterator(file_path)) {
        if (entry.is_directory()) {
            DirectoryFile* directory = new DirectoryFile(entry.path().filename().string(), entry.path().string());
            directory->OpenFile();   // Just for funsies, fill the tree.
            child.push_back(directory);
        }
    }
    
    // Then, iterate over files
    for (const auto& entry : fs::directory_iterator(file_path)) {
        if (!entry.is_directory()) {
            DataFile* file = new DataFile(entry.path().filename().string(), entry.path().string());
            child.push_back(file);
        }
    }
    
    return true;
}


ftxui::Component Inner(std::vector<ftxui::Component> children) {
    using namespace ftxui;
    Component vlist = Container::Vertical(std::move(children));
    return Renderer(vlist, [vlist] {
        return hbox({
            text(" "),
            vlist->Render(),
        });
    });
}

ftxui::Component DirectoryFile::RenderTree() {
    using namespace ftxui;
    std::vector<ftxui::Component> components;

    for (auto& f : child) {
        auto cmp = f->RenderTree();
        components.push_back(cmp);
    }

    auto container = Collapsible(file_name, Inner(std::move(components))); // Move components vector when constructing Inner.
    return container;
}
