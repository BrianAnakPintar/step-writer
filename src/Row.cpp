#include "../include/Row.h"

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
    text.erase(text.begin() + idx);
    len--;
}

void Row::appendRow(Row r) {
    text += r.getText();
    len += r.getLen();
}

void Row::removeString(int idx) {
    text = text.erase(idx);
    len = text.size();
}