#ifndef FILE_H
#define FILE_H

#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"   // for ButtonOption

class File
{
public:
    enum class FileType {
        Directory,
        Data,
    };
    
private:
    FileType file_type;

protected:
    std::string file_name;
    std::string file_path;

    // Protected constructor to allow derived classes to set the file type
    File(const std::string& name, const std::string& path, FileType type) : file_name(name), 
                                                                            file_path(path),
                                                                            file_type(type) {}
    std::string GetPath() const { return file_path; };

public:
    virtual bool OpenFile() = 0;
    virtual ftxui::Component RenderTree() = 0;

    std::string GetFileName() const { return file_name; }
    FileType GetFileType() const { return file_type; }
};
#endif
