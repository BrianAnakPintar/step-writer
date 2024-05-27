#include "../include/Document.h"

Document::Document() {
    dirty = false;
}

Document::Document(const std::string &path) {
    dirty = false;
    // Try opening 3x.
    for (int i = 0; i < 3; i++) {
        if (Open(path))
            break;
    }

}

int Document::Open(const std::string &path) {
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
//            rows.push_back(line);
            Row row(line);
            rows.push_back(row);
        }
        file.close();
    } else {
          Row row("Failed to open file: " + path);
          rows.push_back(row);
//        rows.push_back("Failed to open file: " + path);
    }

    return 1;
}

int Document::Close() {
    // TODO:

    return 1;
}

std::vector<Row> Document::GetRows() {
    return rows;
}

int Document::GetRowsLength() {
    return rows.size();
}

void Document::Insert(int posX, int posY, char  c) {
    rows[posY].insertText(posX, c);
}

void Document::Delete(int posX, int posY) {
    rows[posY].deleteChar(posX);
}

void Document::NewRow(int posY) {
    Row row;
    rows.insert(rows.begin() + posY, row);
}