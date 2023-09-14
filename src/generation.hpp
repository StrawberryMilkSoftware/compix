#pragma once

#include "parser.hpp"

class Generator {
    public:
        inline explicit Generator(NodeExit root)
            : m_root(std::move(root))
        {}
        // no idea why i have to do it like this but whatever...

        [[nodiscard]] std::string generate() const {
            std::stringstream output;
            output << "global _start\n_start:\n";
            output << "    mov rax, 60\n";
            output << "    mov rdi, " << m_root.expr._int.value.value() << "\n";
            output << "    syscall";
            return output.str();
        }

    private:
        const NodeExit m_root;
};