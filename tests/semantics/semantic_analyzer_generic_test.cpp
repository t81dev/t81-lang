#include "../common/test_utils.hpp"

#include <iostream>
#include <string>

int main() {
    const std::string matching_tensor = R"(
        fn main() -> i32 {
            var a: Tensor[T81Int, 2, 3];
            var b: Tensor[T81Int, 2, 3];
            b = a;
            return 0;
        }
    )";
    expect_semantic_success(matching_tensor, "matching_tensor");

    const std::string runtime_constant = R"(
        let RANK: i32 = 3;
        fn main() -> i32 {
            var parametric: Tensor[T81Int, RANK];
            return 0;
        }
    )";
    expect_semantic_success(runtime_constant, "runtime_constant");

    const std::string generic_alias = R"(
        type Box[T, N] = Tensor[T, N];
        fn main() -> i32 {
            let inferred: Box[i32, 4] = Box[i32, 4];
            return 0;
        }
    )";
    expect_semantic_success(generic_alias, "generic_alias");

    std::cout << "Semantic analyzer generic tests passed!" << std::endl;
    return 0;
}
