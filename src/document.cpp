#include "../include/Document.h"

Document::Document() {
    dirty = false;
    rows.push_back("Hello World UwU.");
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
            rows.push_back(line);
        }
        file.close();
    } else {
        rows.push_back("Failed to open file: " + path);
    }

    return 1;
}

int Document::Close() {
    // TODO:

    return 1;
}

std::vector<std::string> Document::GetRows() {
    return rows;
}

int Document::GetRowsLength() {
    return rows.size();
}

