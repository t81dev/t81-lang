#include "../common/test_utils.hpp"

#include <iostream>
#include <string>

int main() {
    const std::string simple_record = R"(
        record Point {
            x: i32;
            y: i32;
        };

        fn main() -> i32 {
            let p: Point = Point { x: 1, y: 2 };
            return 0;
        }
    )";
    expect_semantic_success(simple_record, "simple_record");

    const std::string missing_field = R"(
        record Point {
            x: i32;
            y: i32;
        };

        fn main() -> i32 {
            let p: Point = Point { x: 1 };
            return 0;
        }
    )";
    expect_semantic_failure(missing_field, "missing_field", "missing field 'y'");

    const std::string enum_duplicate_variant = R"(
        enum Mode {
            Start;
            Start;
        };

        fn main() -> i32 {
            return 0;
        }
    )";
    expect_semantic_failure(enum_duplicate_variant, "enum_duplicate_variant", "already exists in enum");

    std::cout << "Semantic analyzer record/enum tests passed!" << std::endl;
    return 0;
}
