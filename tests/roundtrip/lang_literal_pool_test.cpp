#include "t81/frontend/ast_printer.hpp"
#include "t81/frontend/lexer.hpp"
#include "t81/frontend/parser.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace t81::frontend;

namespace {

std::string parse_to_canonical_ast(const std::string& source) {
    Lexer lexer(source);
    Parser parser(lexer, "lang_literal_pool_test");
    auto statements = parser.parse();
    if (parser.had_error()) {
        return {};
    }

    CanonicalAstPrinter printer;
    std::string output;
    for (const auto& stmt : statements) {
        if (!stmt) {
            continue;
        }
        if (!output.empty()) {
            output.push_back('\n');
        }
        output += printer.print(*stmt);
    }
    return output;
}

} // namespace

int main() {
    const std::string source = R"(
        fn main() -> i32 {
            let i: i32 = 123;
            let f: T81Float = 4.5;
            return i;
        }
    )";

    const std::string first = parse_to_canonical_ast(source);
    const std::string second = parse_to_canonical_ast(source);
    assert(!first.empty());
    assert(first == second);

    const std::string expected =
        "(fn main ( ) -> i32 (block (let i: i32 = 123) (let f: T81Float = 4.5) (return i)))";
    if (first != expected) {
        std::cerr << "Literal roundtrip determinism test failed\n";
        std::cerr << "Expected: " << expected << "\n";
        std::cerr << "Actual:   " << first << "\n";
        return 1;
    }

    return 0;
}
