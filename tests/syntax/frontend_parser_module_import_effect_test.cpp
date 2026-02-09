#include "../common/test_utils.hpp"
#include "t81/frontend/parser.hpp"

#include <cassert>
#include <iostream>

using namespace t81::frontend;

int main() {
    const std::string source = R"(
        module core.lang;
        import core.math;

        @effect
        @tier(2)
        fn main() -> i32 {
            let x: bool = true || false && false;
            return 0;
        }
    )";

    Lexer lexer(source);
    Parser parser(lexer);
    auto stmts = parser.parse();
    assert(!parser.had_error());
    assert(stmts.size() == 3);

    AstPrinter printer;
    const std::string module = printer.print(*stmts[0]);
    const std::string import_stmt = printer.print(*stmts[1]);
    const std::string function = printer.print(*stmts[2]);

    const std::string expected_module = "(module core.lang)";
    const std::string expected_import = "(import core.math)";
    const std::string expected_fn =
        "(fn @effect @tier(2) main ( ) -> i32 (block (let x: bool = (|| true (&& false false))) (return 0)))";

    if (module != expected_module || import_stmt != expected_import || function != expected_fn) {
        std::cerr << "Parser module/import/effect test failed\n";
        std::cerr << "module:   " << module << "\n";
        std::cerr << "import:   " << import_stmt << "\n";
        std::cerr << "function: " << function << "\n";
        return 1;
    }

    std::cout << "Parser module/import/effect test passed!\n";
    return 0;
}
