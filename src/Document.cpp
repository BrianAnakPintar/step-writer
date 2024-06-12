#include "Document.h"
#include "Row.h"
#include "tree_sitter/api.h"
#include <cstdint>
#include <fstream>
#include <utility>

#include "iostream"

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

  TSInputEdit edit;
  uint32_t start_byte = 0;
  for (uint32_t i = 0; i < posY; i++) {
    start_byte += rows[i].getLen();
  }
  start_byte += (uint32_t) posX;

  edit.start_byte = start_byte;
  edit.old_end_byte = start_byte;
  edit.new_end_byte = start_byte + 1;

  edit.start_point = {(uint32_t) posY, (uint32_t) posX};
  edit.old_end_point = {(uint32_t) posY, (uint32_t) posX};
  edit.new_end_point = {(uint32_t) posY, (uint32_t) posX+1};
  ts_tree_edit(tree, &edit);
  updateSyntaxHighlightVector(tree);
}

// Helper function to get the byte offset from row and column position
uint32_t Document::GetByteOffset(uint32_t row, uint32_t column) {
  int byteOffset = 0;
  for (int i = 0; i < row; ++i) {
    byteOffset += rows[i].getLen();
  }
  byteOffset += column; 
  return byteOffset;
}

void Document::Delete(int posX, int posY) {
  if (posX < 0) {
    // If delete at beginning.
    if (posY > 0) {
      uint32_t prev_line_byte = GetByteOffset(posY, 0); 
      uint32_t prev_line_size = rows[posY-1].getLen();
      rows[posY - 1].appendRow(rows[posY]);
      rows.erase(rows.begin() + posY);
      
      TSInputEdit edit;
      edit.start_byte = prev_line_byte; 
      edit.old_end_byte = prev_line_byte + 1;
      edit.new_end_byte = prev_line_byte;

      edit.start_point = {(uint32_t)posY-1, prev_line_size};
      edit.old_end_point = {(uint32_t)posY, 0};
      edit.new_end_point = {(uint32_t)posY-1, prev_line_size};
      ts_tree_edit(tree, &edit);
      updateSyntaxHighlightVector(tree);
    }
    return;
  }
  rows[posY].deleteChar(posX);
  // TODO: TSInputEdit here.
  TSInputEdit edit;
  uint32_t start_byte = 0;
  for (uint32_t i = 0; i < posY; i++) {
    start_byte += rows[i].getLen();
  }
  start_byte += (uint32_t)posX;

  edit.start_byte = start_byte;
  edit.old_end_byte = start_byte + 1;
  edit.new_end_byte = start_byte;

  edit.start_point = {(uint32_t)posY, (uint32_t)posX};
  edit.old_end_point = {(uint32_t)posY, (uint32_t)posX + 1};
  edit.new_end_point = {(uint32_t)posY, (uint32_t)posX};
  ts_tree_edit(tree, &edit);
  updateSyntaxHighlightVector(tree);
}

void Document::ReturnKey(int posX, int posY) {
  Row r = rows[posY];
  uint32_t eol = rows[posY].getLen();
  uint32_t start_byte = GetByteOffset(posY, posX);
  rows[posY].removeString(posX + 1);
  std::string afterText = r.getText().substr(posX + 1);
  Row row(afterText);
  rows.insert(rows.begin() + posY + 1, row);
  
  // Update Tree-sitter parse tree
  TSInputEdit edit;
  edit.start_byte = start_byte;
  edit.old_end_byte = start_byte;  // No text is deleted
  edit.new_end_byte = GetByteOffset(posY+1, posX+1); // Adding one byte for the newline character
  
  edit.start_point = {static_cast<uint32_t>(posY), static_cast<uint32_t>(posX)};
  edit.old_end_point = {static_cast<uint32_t>(posY), static_cast<uint32_t>(posX + 1)};
  edit.new_end_point = {static_cast<uint32_t>(posY + 1), 0};

  ts_tree_edit(tree, &edit);
  updateSyntaxHighlightVector(tree);
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
    total += row.getLen() + 1;
  }

  return total;
}

/*
* Traverse and sets the appropriate highlight from node.
* Note: Contains C code.
*/
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

void Document::updateSyntaxHighlightVector(TSTree* old_tree) {
  std::string docString;
  for (Row &row : rows) {
    docString += row.getText() + '\n';
    row.clear_highlights();
  }
  const char *source_code = docString.c_str();
  uint32_t source_code_len = charCount();
  tree = ts_parser_parse_string(parser, old_tree, source_code, source_code_len);
  TSNode root = ts_tree_root_node(tree);

  highlightSyntax(root);
}
