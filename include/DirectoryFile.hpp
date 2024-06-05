#ifndef DIRECTORYFILE_H 
#define DIRECTORYFILE_H
#include "File.hpp"

#include <vector>

class DirectoryFile : public File
{
private:
    std::vector<File*> child;
public:
    DirectoryFile(const std::string& name, const std::string& path);
    ~DirectoryFile();

    bool OpenFile() override;
    ftxui::Component RenderTree() override;
};

#endif