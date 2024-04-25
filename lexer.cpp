#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include <regex>

using namespace std;

// Lexer (sequence of characters) -> sequence of tokens

// tokens
// Identifiers
// Strings
// Integers
// End of File

enum TokenType {
    // Single Characters
    SEMI,
    EQUALS,
    LEFT_PAREN,
    RIGHT_PAREN,
    PLUS,

    // Literals
    IDENTIFIER,
    INTEGER,
    STRING,

    // Key Words
    PRINT,
    LET,
    WHILE,
    IF,
    THEN,
    ELSE,
    END_OF_FILE
};

class Token {
    public:
        Token(TokenType t, std::string l, int ln) {
            type = t;
            lexeme = l;
            line = ln;
        }
        TokenType get_type() {
            return type;
        }
        std::string get_string() {
            return lexeme;
        }
        int get_line() {
            return line;
        }
    private:
        TokenType type;
        std::string lexeme;
        int line;
};

struct TokenInfo {
    TokenType token;
};

std::string token_to_string(Token token) {
    switch (token.get_type()) {
        case SEMI: return "SEMI";
        case EQUALS: return "EQUALS";
        case RIGHT_PAREN: return "RIGHT_PAREN";
        case LEFT_PAREN: return "LEFT_PAREN";
        case PLUS: return "PLUS";
        case IDENTIFIER: return "IDENT " + token.get_string();
        case INTEGER: return "INT " + token.get_string();
        case PRINT: return "PRINT";
        case LET: return "LET";
        case END_OF_FILE: return "EOF";
        default: return "UNIDENTIFIED TOKEN " + token.get_string();
    }
}

bool whitespace(char c) {
    return isspace(c) || c == '\n';
}

class Lexer {
    public:
        Lexer(std::string src) {
            source = src;
            start = 0;
            current = 0;
            line = 1;
        }

        bool in_bounds() {
            return current < source.size();
        }

        std::vector<Token> generate_tokens() {
            while (in_bounds()) {
                if (isspace(source[current])) {
                    current += 1;
                    start += 1;

                } else if (source[current] == '\n') {
                    current += 1;
                    start += 1;
                    line += 1;

                } else if (single_chars.find(source[current]) != single_chars.end()) {
                    Token new_token = Token(single_chars[source[current]], std::string(1, source[current]), line);
                    tokens.push_back(new_token);
                    current += 1;
                    start += 1;

                } else {
                    std::string curr_string = "";
                    unsigned curr_len = 0;
                    while (in_bounds() && !whitespace(source[current]) && (single_chars.find(source[current]) == single_chars.end())) {
                        char c = source[current];
                        curr_string.push_back(c);
                        curr_len += 1;
                        current += 1;
                        
                    }

                    static const std::regex integer_regex("[0-9]+");
                    static const std::regex identifier_regex("[a-z][a-zA-Z0-9_]*");
                    
                    if (keyword_tokens.find(curr_string) != keyword_tokens.end()) { 
                        Token new_token = Token(keyword_tokens[curr_string], curr_string, line);
                        tokens.push_back(new_token);
                    } else if (std::regex_match(curr_string, integer_regex)) {
                        Token new_token = Token(INTEGER, curr_string, line);
                        tokens.push_back(new_token);
                    } else if (std::regex_match(curr_string, identifier_regex)) {
                        Token new_token = Token(IDENTIFIER, curr_string, line);
                        tokens.push_back(new_token);
                    }
                        
                    start = current;

                    
                }
            }

            Token new_token = Token(END_OF_FILE, "EOF", line);
            tokens.push_back(new_token);
            return tokens;

        }
        
    private:
        std::map<char, TokenType> single_chars = {
            {';', SEMI},
            {'=', EQUALS},
            {'(', RIGHT_PAREN},
            {')', LEFT_PAREN},
            {'+', PLUS} 
        };

        std::map<std::string, TokenType> keyword_tokens = {
            {"let", LET},
            {"while", WHILE},
            {"print", PRINT},
        };

        std::vector<Token> tokens;
        std::string source;
        size_t start;
        size_t current;
        int line;
};

int main(int argc, char *argv[]) {
    // std::cout << "test" << std::endl;
    std::string input_str;
    std::ifstream program_file (argv[1]);
    // std::string buffer(std::istream_iterator<char>(program_file), std::istream_iterator<char>());
    
    std::string buffer;
    std::string line;
    while (std::getline(program_file, line)) {
        buffer += line + "\n"; // Append each line to the buffer with a newline character
    }
    
    // std::cout << buffer << std::endl;
    Lexer lex(buffer);
    std::vector<Token> tokens = lex.generate_tokens();
    std::cout << "[";
    for (size_t i = 0; i < tokens.size() - 1; i++) {
        std::cout << token_to_string(tokens[i]) << ", ";
    }
    std::cout << token_to_string(tokens[tokens.size() - 1]) << "]" << std::endl;
    return 0;
}