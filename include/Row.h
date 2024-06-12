#ifndef STEP_WRITER_ROW_H
#define STEP_WRITER_ROW_H

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include "ftxui/component/component.hpp"

enum class highlight_type {
  Keyword,
  Operator,
  Identifier,
  Number,
  String,
  Comment,
  Unknown
};

struct highlight_item {
  std::pair<uint32_t, uint32_t> h_idx;
  highlight_type h_type;
};

class Row {
public:
    Row(std::string s);
    Row();

    std::string getText();
    int getLen();

    void insertText(int idx, char c);
    void removeString(int idx);
    void deleteChar(int idx);
    void appendRow(Row r);
    ftxui::Element Render();
    void add_highlight_item(highlight_item item);
    void clear_highlights();
    std::vector<highlight_item> getHighlights();

private:
    std::string text;
    int len;
    std::vector<highlight_item> highlight_items;

};

#endif //STEP_WRITER_ROW_H
