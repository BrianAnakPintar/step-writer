#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <cstdint>
#include <ftxui/dom/node.hpp>
#include <string>
#include <vector>
#include "Row.h"

#include <tree_sitter/api.h>
extern "C" const TSLanguage *tree_sitter_cpp();

class Document {

private:
  std::string file_path;
  bool dirty;
  std::vector<Row> rows;
  void NewRow(int posY);
  std::string toString();
  uint32_t charCount();
  TSParser *parser;
  TSTree *tree;
  void highlightSyntax(TSNode node);


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
  bool isDirty();
};


#endif
