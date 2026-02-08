# Dependency Gaps After Initial Migration

Snapshot date: 2026-02-08

The first migration pass intentionally moved language-owned code first. Some files still include runtime-owned headers from `t81-foundation`.

## Verified Smoke Build

Successful (compiled with `c++ -std=c++20 -Iinclude -c`):

- `src/frontend/lexer.cpp`
- `src/frontend/parser.cpp`
- `src/frontend/semantic_analyzer.cpp`
- `src/frontend/symbol_table.cpp`
- `src/tisc/pretty_printer.cpp`

Resolved in this pass:

- Added language-owned minimal contract headers:
  - `include/t81/tisc/ir.hpp`
  - `include/t81/tisc/program.hpp`
  - `include/t81/tisc/opcodes.hpp`
  - `include/t81/tisc/type_alias.hpp`
  - `include/t81/tisc/pretty_printer.hpp`
  - `include/t81/tensor.hpp`
  - `include/t81/enum_meta.hpp`
  - `include/t81/support/expected.hpp`

## Runtime-Coupled Test Dependencies

Several migrated tests currently include runtime/CLI headers that are not yet present in `t81-lang`:

- `t81/tisc/*`
- `t81/vm/*`
- `t81/cli/*`

Examples:

- `tests/roundtrip/e2e_option_result_test.cpp`
- `tests/roundtrip/e2e_advanced_features_test.cpp`
- `tests/roundtrip/cli_structural_types_test.cpp`

## Language-Only Test Check

- `tests/syntax/frontend_parser_test.cpp`: passes when compiled and run locally with frontend sources.
- `tests/syntax/frontend_lexer_test.cpp`: currently fails an assertion on token count and needs triage in this repository context.

## Next Extraction Slice

1. Decide whether `t81/tisc/ir.hpp` (and related IR interfaces) moves to `t81-lang` or to a shared contract package.
2. Split test lanes:
   - language-only tests (self-contained in `t81-lang`),
   - integration tests (require `t81-foundation`).
3. Add CI jobs that make this split explicit.
