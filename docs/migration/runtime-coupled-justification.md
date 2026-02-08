# Runtime-Coupled Test Justification

Snapshot date: 2026-02-08

Tracking issue: `t81-lang#3`

This document is the explicit rationale ledger for every entry in
`tests/roundtrip/runtime-coupled-tests.txt`.

- tests/roundtrip/cli_option_result_test.cpp: Uses `t81/cli/driver.hpp` to verify CLI-level integration behavior.
- tests/roundtrip/cli_record_enum_test.cpp: Uses `t81/cli/driver.hpp` for record/enum flow through CLI integration.
- tests/roundtrip/cli_structural_types_test.cpp: Uses `t81/cli/driver.hpp` and `t81/tisc/binary_io.hpp` for CLI + binary path coverage.
- tests/roundtrip/e2e_advanced_features_test.cpp: Uses `t81/tisc/binary_emitter.hpp` and `t81/vm/*` to execute emitted programs.
- tests/roundtrip/e2e_arithmetic_test.cpp: Uses `t81/tisc/binary_emitter.hpp` and `t81/vm/vm.hpp` for runtime-validated arithmetic semantics.
- tests/roundtrip/e2e_if_statement_test.cpp: Uses `t81/tisc/binary_emitter.hpp` and `t81/vm/vm.hpp` for control-flow runtime validation.
- tests/roundtrip/e2e_let_statement_test.cpp: Uses `t81/tisc/binary_emitter.hpp` and `t81/vm/vm.hpp` for emitted program execution.
- tests/roundtrip/e2e_loop_statement_test.cpp: Uses `t81/tisc/binary_emitter.hpp` and `t81/vm/*` for loop behavior at runtime.
- tests/roundtrip/e2e_match_expression_test.cpp: Uses `t81/tisc/binary_emitter.hpp` and `t81/vm/*` for match lowering execution checks.
- tests/roundtrip/e2e_option_result_function_test.cpp: Uses `t81/tisc/binary_emitter.hpp` and `t81/vm/*` for option/result runtime path.
- tests/roundtrip/e2e_option_result_test.cpp: Uses `t81/tisc/binary_emitter.hpp` and `t81/vm/*` for option/result runtime path.
- tests/roundtrip/e2e_option_type_test.cpp: Uses `t81/tisc/binary_emitter.hpp` and `t81/vm/vm.hpp` to validate option type emission/execution.
- tests/roundtrip/lang_literal_pool_test.cpp: Uses `t81/vm/vm.hpp` to validate emitted literal pool execution behavior.

## Shrink Rule

When a test no longer includes runtime/CLI headers, remove it from the runtime-coupled manifest and delete its corresponding line above in the same change.
