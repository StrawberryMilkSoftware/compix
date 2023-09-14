#pragma once
#include "tokenization.hpp"
#include <vector>

struct NodeExpr {
    Token _int;
};
struct NodeExit {
    NodeExpr expr;
};

class Parser {
    public:
        inline explicit Parser(std::vector<Token> tokens) 
            : m_tokens(std::move(tokens))
        {}

        std::optional<NodeExpr> parse_expr() {
            if (peek().has_value() && peek().value().type == TokenType::_int) {
                return NodeExpr{._int = consume()};
            } else {
                return {};
            }
        }

        std::optional<NodeExit> parse() {
            std::optional<NodeExit> exit_node;
            while(peek().has_value()) {
                if (peek().value().type == TokenType::exit) {
                    consume();
                    if (auto node_expr = parse_expr()) {
                        // if node_expr has a value
                        exit_node = NodeExit{ .expr = node_expr.value() };
                    } 
                    else {
                        std::cerr << "Invalid expression" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    if (peek().has_value() || peek().value().type == TokenType::semi) {
                        consume();
                    } else {
                        std::cerr << "Invalid expression" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
            }
            m_index = 0;
            return exit_node;
        }
    private:
        [[nodiscard]] inline std::optional<Token> peek(int ahead = 1) const {
            if (m_index + ahead > m_tokens.size()) {
                return {};
            } else {
                return m_tokens.at(m_index);
            }
        }

        inline Token consume() {
            return m_tokens.at(m_index++);
            /* this is stupid but it works
            return m_src.at(m_index)
            then m_index++
            so it increments AFTER it gets the thing
            i think
            idk */
        }


        const std::vector<Token> m_tokens;
        size_t m_index = 0;
};