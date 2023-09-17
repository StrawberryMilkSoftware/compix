#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "generation.hpp"


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Please provide an input and output file." << std::endl;
        std::cerr << "compix <INPUTFILE.CPX> <OUTPUTFILE>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    Tokenizer tokenizer(contents);
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));;
    std::optional<NodeProg> prog = parser.parse_prog();

    if (!prog.has_value()) {
        std::cerr << "Invalid program!" << std::endl;
        exit(EXIT_FAILURE);
    }

    Generator generator(prog.value());

    {
        std::fstream file("out.asm", std::ios::out);
        file << generator.gen_prog();
    }

    std::string outputName(argv[2]);
    std::string link_str = "ld -o " + outputName + " out.o";

    system("nasm -felf64 out.asm");
    system(link_str.c_str());
    system("rm out.o");
    system("rm out.asm");

    return EXIT_SUCCESS;
}