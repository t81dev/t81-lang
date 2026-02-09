#include "../common/test_utils.hpp"

#include <iostream>
#include <string>

int main() {
    const std::string simple_vector = R"(
        fn main() -> i32 {
            let v = [1, 2, 3];
            return 0;
        }
    )";
    expect_semantic_success(simple_vector, "simple_vector");

    const std::string widened_vector = R"(
        fn main() -> i32 {
            let v = [1, 2.5];
            return 0;
        }
    )";
    expect_semantic_success(widened_vector, "widened_vector");

    const std::string empty_no_context = R"(
        fn main() -> i32 {
            let v = [];
            return 0;
        }
    )";
    expect_semantic_failure(empty_no_context, "empty_no_context");

    std::cout << "Semantic analyzer vector literal tests passed!" << std::endl;
    return 0;
}
