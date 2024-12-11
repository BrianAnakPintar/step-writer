#include "DirectoryFile.hpp"
#include "DataFile.hpp"
#include <filesystem>
#include <memory>

DirectoryFile::DirectoryFile(const std::string& name, const std::string& path) 
    : File(name, path, FileType::Directory), 
      lazyLoader(std::make_unique<LazyLoader>(path)) {}

DirectoryFile::~DirectoryFile() = default;

bool DirectoryFile::OpenFile() {
    // No longer pre-loading all children
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
    
    // Lazy loading of children when the collapsible is expanded
    return Collapsible(file_name, Renderer([this] {
        std::vector<ftxui::Component> components;
        
        // Load children only when rendering
        lazyLoader->LoadChildren(child);
        
        for (auto& f : child) {
            auto cmp = f->RenderTree();
            components.push_back(cmp);
        }

        return Inner(std::move(components))->Render();
    }));
}
