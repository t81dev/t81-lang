#include "../common/test_utils.hpp"

#include <iostream>
#include <string>

int main() {
    const std::string valid_option_result = R"(
        fn main() -> i32 {
            let maybe: Option[i32] = Some(1);
            let value: i32 = match (maybe) {
                Some(v) => v;
                None => 0;
            };

            let result: Result[i32, T81String] = Ok(value);
            let output: i32 = match (result) {
                Ok(v) => v;
                Err(_) => 0;
            };
            return output;
        }
    )";
    expect_semantic_success(valid_option_result, "valid_option_result");

    const std::string none_without_context = R"(
        fn main() -> i32 {
            let missing = None();
            return 0;
        }
    )";
    expect_semantic_failure(none_without_context, "none_without_context",
                            "requires a contextual Option[T] type");

    const std::string err_wrong_type = R"(
        fn main() -> Result[i32, T81String] {
            return Err(7);
        }
    )";
    expect_semantic_failure(err_wrong_type, "err_wrong_type");

    std::cout << "Semantic analyzer option/result tests passed!" << std::endl;
    return 0;
}
