#include "Row.h"
#include <cstdint>
#include <string>
#include <vector>


Row::Row(std::string s) {
    text = s;
    len = s.size();
}

Row::Row() {
    len = 0;
}

std::vector<highlight_item> Row::getHighlights() {
  return highlight_items;
}

std::string Row::getText() {
    return text;
}

int Row::getLen() {
    return len;
}

void Row::insertText(int idx, char c) {
    if (text.empty())
        text = "";
    text.insert(idx, 1, c);
    len++;
}

void Row::deleteChar(int idx) {
    // Normal Case
    if (len == 0 || idx >= len) {
        return;
    }
    text.erase(text.begin() + idx);
    len--;
}

void Row::appendRow(Row r) {
    text += r.getText();
    len += r.getLen();
}

void Row::removeString(int idx) {
    if (idx > len || idx < 0)
        return;
    text = text.erase(idx);
    len = text.size();
}

void Row::add_highlight_item(highlight_item item) {
  highlight_items.push_back(item);
}

void Row::clear_highlights() {
  highlight_items.clear();
}

// Renders with syntax highlighting. Returns an hbox component where texts fill row.
ftxui::Element Row::Render() {
  using namespace ftxui;

  uint32_t last_index = 0;
  Elements elements;

  for (const highlight_item& item : highlight_items) {
    // Add the part of the string before the highlighted part.
    uint32_t start = item.h_idx.first;
    uint32_t end = item.h_idx.second;
    if (item.h_idx.first > last_index) {
      elements.push_back(ftxui::text(text.substr(last_index, start - last_index)));
    }
    // Add the highlighted part.
    if (item.h_type == highlight_type::String)
      elements.push_back(ftxui::text(text.substr(start, end - start)) | color(Color::Green));
    else if (item.h_type == highlight_type::Identifier)
      elements.push_back(ftxui::text(text.substr(start, end - start)) | color(Color::Blue));
    else if (item.h_type == highlight_type::Number)
      elements.push_back(ftxui::text(text.substr(start, end - start)) | color(Color::Red));
    else if (item.h_type == highlight_type::Keyword)
      elements.push_back(ftxui::text(text.substr(start, end - start)) | color(Color::Yellow));
    else if (item.h_type == highlight_type::Comment)
      elements.push_back(ftxui::text(text.substr(start, end - start)) | color(Color::Grey35));

    last_index = end;
  }
  // Add the rest of the string after the last highlighted part.
  if (last_index < text.size()) {
    elements.push_back(ftxui::text(text.substr(last_index)));
  }

  return hbox(std::move(elements));
}
