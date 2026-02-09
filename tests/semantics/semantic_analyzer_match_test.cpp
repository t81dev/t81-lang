#include "../common/test_utils.hpp"

#include <iostream>
#include <string>

int main() {
    const std::string option_match_success = R"(
        fn main() -> i32 {
            let maybe: Option[i32] = Some(10);
            let value: i32 = match (maybe) {
                Some(v) => v + 1;
                None => 0;
            };
            return value;
        }
    )";
    expect_semantic_success(option_match_success, "option_match_success");

    const std::string option_match_missing_none = R"(
        fn main() -> i32 {
            let maybe: Option[i32] = Some(1);
            match (maybe) {
                Some(v) => v;
            };
            return 0;
        }
    )";
    expect_semantic_failure(option_match_missing_none, "option_match_missing_none",
                            "requires 'None' arm");

    const std::string enum_match_success = R"(
        enum Signal {
            Red;
            Green;
            Data(i32);
        };

        fn main() -> i32 {
            var signal: Signal;
            let value: i32 = match (signal) {
                Red => 1;
                Green => 2;
                Data(v) => v;
            };
            return value;
        }
    )";
    expect_semantic_success(enum_match_success, "enum_match_success");

    const std::string guard_non_bool_failure = R"(
        fn main() -> i32 {
            let maybe: Option[i32] = Some(5);
            match (maybe) {
                Some(v) if v + 1 => v;
                None => 0;
            };
            return 0;
        }
    )";
    expect_semantic_failure(guard_non_bool_failure, "guard_non_bool_failure",
                            "Condition must be bool");

    std::cout << "Semantic analyzer match tests passed!" << std::endl;
    return 0;
}
