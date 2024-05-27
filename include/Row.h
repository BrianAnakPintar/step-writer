#ifndef STEP_WRITER_ROW_H
#define STEP_WRITER_ROW_H

#include <string>

class Row {
public:
    Row(std::string s);
    Row();
    std::string getText();
    int getLen();
    void insertText(int idx, char c);
    void deleteChar(int idx);
private:
    std::string text;
    int len;
};

#endif //STEP_WRITER_ROW_H
