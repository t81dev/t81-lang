#include "t81/frontend/ir_generator.hpp"
#include "t81/frontend/lexer.hpp"
#include "t81/frontend/parser.hpp"
#include "t81/frontend/semantic_analyzer.hpp"
#include "t81/tisc/ir.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace t81::frontend;
using namespace t81::tisc::ir;

int main() {
    const std::string source = R"(
        @effect
        @tier(3)
        fn side_effect(v: i32) -> bool {
            return v > 0;
        }

        @effect
        fn main() -> i32 {
            let a: bool = false && (1 < 2);
            let b: bool = true || (1 < 2);
            return 0;
        }
    )";

    Lexer lexer(source);
    Parser parser(lexer);
    auto stmts = parser.parse();
    assert(!parser.had_error());

    SemanticAnalyzer analyzer(stmts);
    analyzer.analyze();
    assert(!analyzer.had_error());

    IRGenerator generator;
    generator.attach_semantic_analyzer(&analyzer);
    auto program = generator.generate(stmts);

    bool has_jz = false;
    bool has_jnz = false;
    for (const auto& inst : program.instructions()) {
        if (inst.opcode == Opcode::JZ) has_jz = true;
        if (inst.opcode == Opcode::JNZ) has_jnz = true;
    }

    assert(has_jz && "logical && should emit JZ short-circuit branch");
    assert(has_jnz && "logical || should emit JNZ short-circuit branch");

    bool saw_effectful = false;
    bool saw_tier = false;
    for (const auto& meta : program.function_metadata()) {
        if (meta.name == "side_effect") {
            if (meta.is_effectful) {
                saw_effectful = true;
            }
            if (meta.tier.has_value() && *meta.tier == 3) {
                saw_tier = true;
            }
        }
    }
    assert(saw_effectful && "expected @effect function metadata");
    assert(saw_tier && "expected @tier metadata");

    std::cout << "Frontend IR logical short-circuit test passed!" << std::endl;
    return 0;
}
