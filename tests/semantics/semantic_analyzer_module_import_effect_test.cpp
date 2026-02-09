#include "../common/test_utils.hpp"

#include <iostream>
#include <string>

int main() {
    const std::string valid = R"(
        module core.lang;
        import core.math;

        @effect
        @tier(2)
        fn main() -> i32 {
            let a: bool = true || false && false;
            if (a && true) {
                return 1;
            }
            return 0;
        }
    )";
    expect_semantic_success(valid, "module_import_effect_valid");

    const std::string duplicate_import = R"(
        module core.lang;
        import core.math;
        import core.math;

        fn main() -> i32 {
            return 0;
        }
    )";
    expect_semantic_failure(duplicate_import, "duplicate_import", "Duplicate import");

    const std::string pure_calls_effect = R"(
        @effect
        fn write_log(v: i32) -> i32 {
            return v;
        }

        fn main() -> i32 {
            return write_log(1);
        }
    )";
    expect_semantic_failure(pure_calls_effect, "pure_calls_effect",
                            "Pure function cannot call effectful function");

    const std::string effect_calls_effect = R"(
        @effect
        fn write_log(v: i32) -> i32 {
            return v;
        }

        @effect
        fn main() -> i32 {
            return write_log(1);
        }
    )";
    expect_semantic_success(effect_calls_effect, "effect_calls_effect");

    std::cout << "Semantic analyzer module/import/effect tests passed!" << std::endl;
    return 0;
}
