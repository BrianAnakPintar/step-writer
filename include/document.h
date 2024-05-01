#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>

class Document {

private:
    std::string file_path;
    bool dirty;

public:
    Document(const std::string &path);
    Document();
    int open(const std::string &path);
    int close();
};


#endif