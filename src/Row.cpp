#include "../include/Row.h"
#include "Tokenizer.hpp"
#include <vector>

Row::Row(std::string s) {
    text = s;
    len = s.size();
}

Row::Row() {
    len = 0;
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

// Renders with syntax highlighting. Returns an hbox component where texts fill row.
ftxui::Element Row::Render() {
  using namespace ftxui;
  Tokenizer& tokenizer = Tokenizer::GetInstance();
  std::vector<Token> tokens = tokenizer.Tokenize(getText());

  Elements elements; 

  for (const Token& token : tokens) {
    std::string token_text_with_space = token.text + " ";

    switch (token.type) {
      case TokenType::Keyword:
        elements.push_back(ftxui::text(token_text_with_space) | color(Color::SkyBlue1));
        break;
      case TokenType::Operator:
        elements.push_back(ftxui::text(token_text_with_space) | color(Color::IndianRed1));
        break;
      case TokenType::Number:
        elements.push_back(ftxui::text(token_text_with_space) | color(Color::Yellow));
        break;
      case TokenType::String:
        elements.push_back(ftxui::text(token_text_with_space) | color(Color::DarkSeaGreen4));
        break;
      default:
        elements.push_back(ftxui::text(token_text_with_space));
        break;
    }
  }

  return hbox(std::move(elements));
}
