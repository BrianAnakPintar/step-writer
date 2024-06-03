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

// Returns 1 if succ
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
    return -1;
}

int Document::Save() {
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
    dirty = true;
    rows[posY].insertText(posX, c);
}

void Document::Delete(int posX, int posY) {
    if (posX < 0) {
        // If delete at beginning.
        if (posY > 0) {
            rows[posY-1].appendRow(rows[posY]);
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
    rows[posY].removeString(posX+1);
    std::string afterText = r.getText().substr(posX+1);
    Row row(afterText);
    rows.insert(rows.begin() + posY+1, row);
}