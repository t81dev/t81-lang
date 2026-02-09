#include "../common/test_utils.hpp"

#include <iostream>
#include <string>

int main() {
    const std::string int_float_widening = R"(
        fn main() -> T81Float {
            let big: T81BigInt = 123456;
            let result: T81Float = big + 1.25;
            return result;
        }
    )";
    expect_semantic_success(int_float_widening, "int_float_widening");

    const std::string bool_in_arithmetic = R"(
        fn main() -> i32 {
            return 1 + true;
        }
    )";
    expect_semantic_failure(bool_in_arithmetic, "bool_in_arithmetic");

    const std::string modulo_non_integer = R"(
        fn main() -> i32 {
            return 1.5 % 2.0;
        }
    )";
    expect_semantic_failure(modulo_non_integer, "modulo_non_integer");

    std::cout << "Semantic analyzer numeric rules tests passed!" << std::endl;
    return 0;
}
