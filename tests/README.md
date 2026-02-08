# tests

Current migrated test tracks:

- parser determinism tests
- AST canonical serialization snapshots
- semantic checker rule tests
- TISC IR and bytecode golden tests
- cross-platform reproducibility checks

## Suite Layout

- `tests/syntax/`: lexer/parser/fuzz grammar cases
- `tests/semantics/`: semantic analyzer and type system checks
- `tests/roundtrip/`: IR generation, e2e, and integration-style flows
- `tests/common/`: shared test helpers

## Transition Note

Some `e2e_*` and `cli_*` tests currently include runtime/CLI headers from `t81-foundation` (`t81/vm/*`, `t81/tisc/*`, `t81/cli/*`). During the split transition, run:

- language-only suites in this repo (frontend/parser/semantic),
- integration suites with `t81-foundation` available in include/library paths.
