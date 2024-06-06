#include "Tokenizer.hpp"
#include <cctype>

Tokenizer& Tokenizer::GetInstance() {
  static Tokenizer instance;
  return instance;
}

std::vector<Token> Tokenizer::Tokenize(const std::string& line) {
    std::vector<Token> tokens;
    std::string token;
    TokenType type = TokenType::Unknown;

    for (char ch : line) {
        if (isspace(ch)) {
            if (!token.empty()) {
                tokens.push_back({token, type});
                token.clear();
            }
            continue;
        }

        if (isalpha(ch) || (ch == '_' && !token.empty())) {
            type = TokenType::Identifier;
            token += ch;
        } else if (isdigit(ch)) {
            if (type == TokenType::Unknown) type = TokenType::Number;
            token += ch;
        } else if (ch == '"' || ch == '\'') {
            type = TokenType::String;
            token += ch;
            tokens.push_back({token, type});
            token.clear();
            type = TokenType::Unknown;
        } else if (operators.find(ch) != operators.end()) {
            if (!token.empty()) {
                tokens.push_back({token, type});
                token.clear();
            }
            type = TokenType::Operator;
            token += ch;
            tokens.push_back({token, type});
            token.clear();
            type = TokenType::Unknown;
        } else {
            if (!token.empty()) {
                tokens.push_back({token, type});
                token.clear();
            }
            type = TokenType::Unknown;
            token += ch;
        }
    }

    if (!token.empty()) {
        tokens.push_back({token, type});
    }

    for (auto& token : tokens) {
        if (keywords.find(token.text) != keywords.end()) {
            token.type = TokenType::Keyword;
        }
    }

    return tokens;
}
