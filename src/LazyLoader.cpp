#include "LazyLoader.hpp"
#include "filesystem"
#include "DirectoryFile.hpp"
#include "DataFile.hpp"

LazyLoader::LazyLoader(const std::string& path) : file_path(path), loaded(false) {}

void LazyLoader::LoadChildren(std::vector<File*>& children) {
    if (loaded) return;

    namespace fs = std::filesystem;

    // Iterate over directories first
    for (const auto& entry : fs::directory_iterator(file_path)) {
        if (entry.is_directory()) {
            auto directory = std::make_unique<DirectoryFile>(
                entry.path().filename().string(), 
                entry.path().string()
            );
            children.push_back(directory.release());
        }
    }
    
    // Then iterate over files
    for (const auto& entry : fs::directory_iterator(file_path)) {
        if (!entry.is_directory()) {
            auto file = std::make_unique<DataFile>(
                entry.path().filename().string(), 
                entry.path().string()
            );
            children.push_back(file.release());
        }
    }

    loaded = true;
}
