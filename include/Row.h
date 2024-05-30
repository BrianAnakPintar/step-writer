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
    void removeString(int idx);
    void deleteChar(int idx);
    void appendRow(Row r);

private:
    std::string text;
    int len;
};

#endif //STEP_WRITER_ROW_H
