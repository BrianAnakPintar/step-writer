#ifndef DATAFILE_H
#define DATAFILE_H
#include "File.hpp"

class DataFile : public File
{
private:
    

public:
    DataFile(const std::string& name, const std::string& path);
    ~DataFile();

    bool OpenFile() override;
    ftxui::Component RenderTree() override;
};

#endif