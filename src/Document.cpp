#include "Document.h"
#include "Row.h"
#include "tree_sitter/api.h"
#include <cstdint>
#include <fstream>
#include <utility>

Document::Document() { dirty = false; }

Document::Document(const std::string &path) : file_path(path) {
  dirty = false;
  // Try opening 3x.
  for (int i = 0; i < 3; i++) {
    if (Open(path))
      break;
  }

  parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_cpp());

  std::string docString;
  for (Row row : rows) {
    docString += row.getText() + '\n';
  }
  const char *source_code = docString.c_str();
  uint32_t source_code_len = charCount();
  tree = ts_parser_parse_string(parser, NULL, source_code, source_code_len);
  TSNode root = ts_tree_root_node(tree);

  highlightSyntax(root);
}

// Returns 1 if succeed
int Document::Open(const std::string &path) {
  std::ifstream file(path);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      Row row(line);
      rows.push_back(row);
    }
    file.close();
    return 1;
  }
  rows.push_back(Row("Error opening file."));
  return 0;
}

int Document::Save() {
  std::ofstream outFile(file_path);
  if (!outFile) {
    return 0;
  }
  for (Row r : rows) {
    outFile << r.getText() << std::endl;
  }
  outFile.close();

  return 1;
}

std::vector<Row> Document::GetRows() { return rows; }

int Document::GetRowsLength() { return rows.size(); }

void Document::Insert(int posX, int posY, char c) {
  dirty = true;
  rows[posY].insertText(posX, c);
}

void Document::Delete(int posX, int posY) {
  if (posX < 0) {
    // If delete at beginning.
    if (posY > 0) {
      rows[posY - 1].appendRow(rows[posY]);
      rows.erase(rows.begin() + posY);
    }
    return;
  }
  rows[posY].deleteChar(posX);
}

void Document::NewRow(int posY) {
  Row row;
  rows.insert(rows.begin() + posY, row);
}

void Document::ReturnKey(int posX, int posY) {
  Row r = rows[posY];
  rows[posY].removeString(posX + 1);
  std::string afterText = r.getText().substr(posX + 1);
  Row row(afterText);
  rows.insert(rows.begin() + posY + 1, row);
}

bool Document::isDirty() { return dirty; }

std::string Document::toString() {
  std::string docString;
  for (Row row : rows) {
    docString += row.getText() + '\n';
  }
  return docString;
}

uint32_t Document::charCount() {
  uint32_t total = 0;
  for (Row row : rows) {
    total += row.getLen();
  }

  return total;
}

// Trigger Warning: Contains C code.
void Document::highlightSyntax(TSNode node) {
  if (ts_node_is_null(node))
    return;

  const char *c_node_type = ts_node_type(node);
  std::string node_type(c_node_type);

  TSPoint start_point = ts_node_start_point(node);
  TSPoint end_point = ts_node_end_point(node);

  if (node_type == "string_literal" || node_type == "system_lib_string" || node_type == "character_literal") {
    std::pair<uint32_t, uint32_t> idx_pair(start_point.column, end_point.column);
    highlight_item item = {idx_pair, highlight_type::String};
    rows[start_point.row].add_highlight_item(item);

  } else if (node_type == "identifier") {
    std::pair<uint32_t, uint32_t> idx_pair(start_point.column, end_point.column);
    highlight_item item = {idx_pair, highlight_type::Identifier};
    rows[start_point.row].add_highlight_item(item);

  } else if (node_type == "number_literal") {
    std::pair<uint32_t, uint32_t> idx_pair(start_point.column, end_point.column);
    highlight_item item = {idx_pair, highlight_type::Number};
    rows[start_point.row].add_highlight_item(item);

  } else if (node_type == "primitive_type" || node_type == "namespace_identifier") {
    std::pair<uint32_t, uint32_t> idx_pair(start_point.column, end_point.column);
    highlight_item item = {idx_pair, highlight_type::Keyword};
    rows[start_point.row].add_highlight_item(item);

  } else if (node_type == "comment") {
    std::pair<uint32_t, uint32_t> idx_pair(start_point.column, end_point.column);
    highlight_item item = {idx_pair, highlight_type::Comment};
    rows[start_point.row].add_highlight_item(item);


  } else if (node_type == "return_statement") {
    std::pair<uint32_t, uint32_t> idx_pair(start_point.column, start_point.column + 6);
    highlight_item item = {idx_pair, highlight_type::Number};
    rows[start_point.row].add_highlight_item(item);


  } else if (node_type == "for_range_loop" || node_type == "for_statement") {
    std::pair<uint32_t, uint32_t> idx_pair(start_point.column, start_point.column + 3);
    highlight_item item = {idx_pair, highlight_type::Number};
    rows[start_point.row].add_highlight_item(item);

  }



  unsigned int child_count = ts_node_child_count(node);
  for (unsigned int i = 0; i < child_count; i++) {
    TSNode child = ts_node_child(node, i);
    highlightSyntax(child);
  }
}

