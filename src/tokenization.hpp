#pragma once

#include <string>
#include <vector>
#include <optional>
#include <iostream>

enum class TokenType {
    exit,
    _int,
    semi,
    l_paren,
    r_paren,
    ident,
    set,
    eq
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

class Tokenizer {
    public:
        inline explicit Tokenizer(std::string& src)
            : m_src(std::move(src))
        {
        }

        inline std::vector<Token> tokenize() {
            std::vector<Token> tokens;
            std::string buf;
            while(peek().has_value()) {
                if (std::isalpha(peek().value())) {
                    buf.push_back(consume());
                    while (peek().has_value() && std::isalnum(peek().value())) {
                        buf.push_back(consume());
                    }
                    if (buf == "exit") {
                        tokens.push_back({.type = TokenType::exit});
                        buf.clear();
                        continue;
                    } 
                    else if (buf == "set"){
                        tokens.push_back({.type = TokenType::set});
                        buf.clear();
                        continue;
                    } else {
                        tokens.push_back({.type = TokenType::ident, .value = buf});
                        buf.clear();
                        continue;
                    }
                }
                else if (std::isdigit(peek().value())) {
                    buf.push_back(consume());
                    while (peek().has_value() && std::isdigit(peek().value())) {
                        buf.push_back(consume());
                    }
                    tokens.push_back({.type = TokenType::_int, .value = buf});
                    buf.clear();
                    continue;
                }
                else if (peek().value() == '(') {
                    consume();
                    tokens.push_back({.type = TokenType::l_paren});
                    continue;
                }
                else if (peek().value() == ')') {
                    consume();
                    tokens.push_back({.type = TokenType::r_paren});
                    continue;
                }
                else if (peek().value() == ';') {
                    consume();
                    tokens.push_back({.type = TokenType::semi});
                    continue;
                }
                else if (peek().value() == '=') {
                    consume();
                    tokens.push_back({.type = TokenType::eq});
                    continue;
                }
                else if (std::isspace(peek().value())) {
                    consume();
                    continue;
                } else {
                    std::cerr << "i hate you" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            m_index = 0;
            return tokens;
        }

    private:

        [[nodiscard]] inline std::optional<char> peek(int offset = 0) const {
            if (m_index + offset >= m_src.length()) {
                return {};
            } else {
                return m_src.at(m_index + offset);
            }
        }

        inline char consume() {
            return m_src.at(m_index++);
            /* this is stupid but it works
            return m_src.at(m_index)
            then m_index++
            so it increments AFTER it gets the thing
            i think
            idk */
        }

        const std::string m_src;
        int m_index = 0;
};