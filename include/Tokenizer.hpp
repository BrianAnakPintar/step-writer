#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <unordered_set>

enum class TokenType {
  Keyword,
  Operator,
  Identifier,
  Number,
  String,
  Comment,
  Unknown
};

struct Token {
  std::string text;
  TokenType type;
};

class Tokenizer {
private:
    // Syntax highlighting rules
    const std::unordered_set<std::string> file_types = {
        ".c", ".cpp", ".h", ".hpp"
    };

    const std::unordered_set<std::string> keywords = {
        "int", "float", "return", "if", "else", "while", "for", "class", "public", "private"
    };

    const std::unordered_set<char> operators = {
    '+', '-', '*', '/', '=', '>', '<', '!', '&', '|'
    };

    Tokenizer() {};

public:
    Tokenizer(const Tokenizer&) = delete;
    void operator=(const Tokenizer&) = delete;
    static Tokenizer& GetInstance();
    std::vector<Token> Tokenize(const std::string& line);
};

#endif
