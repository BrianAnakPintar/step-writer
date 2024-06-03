#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include "Row.h"

class Document {

private:
    std::string file_path;
    bool dirty;
    std::vector<Row> rows;
    void NewRow(int posY);

public:
    Document(const std::string &path);
    Document();
    int Open(const std::string &path);
    int Save();
    std::vector<Row> GetRows();
    int GetRowsLength();
    void Insert(int posX, int posY, char c);
    void ReturnKey(int posX, int posY);
    void Delete(int posX, int posY);
};


#endif