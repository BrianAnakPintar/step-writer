#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <filesystem>
#include <fstream>
#include <vector>

class Document {

private:
    std::string file_path;
    bool dirty;
    std::vector<std::string> rows;

public:
    Document(const std::string &path);
    Document();
    int Open(const std::string &path);
    int Close();
    std::vector<std::string> GetRows();
    int GetRowsLength();
};


#endif