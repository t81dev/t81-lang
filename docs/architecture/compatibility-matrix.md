# Compatibility Matrix

Snapshot date: 2026-02-08

This matrix defines the expected contract between `t81-lang` compiler artifacts and `t81-foundation` runtime components.

## Versioning Model

- `t81-lang` owns language syntax/semantics versioning.
- `t81-foundation` owns runtime execution contract versioning.
- Cross-repo compatibility is explicit and tested.

## Contract Surface

| Surface | Producer | Consumer | Compatibility Rule |
| :--- | :--- | :--- | :--- |
| T81Lang grammar + AST shape | `t81-lang` | `t81-lang` tooling | No runtime dependency; versioned in language releases. |
| Typed semantic model | `t81-lang` | `t81-lang` codegen | Deterministic diagnostics and canonicalization required. |
| TISC IR text/binary encoding | `t81-lang` | `t81-foundation` (TISC loader/VM) | Must match accepted IR schema versions published by `t81-foundation`. |
| Bytecode/program metadata manifest | `t81-lang` | `t81-foundation` (HanoiVM/Axion) | Must include declared format version and deterministic provenance hash fields. |
| Axion policy handoff metadata | `t81-lang` | `t81-foundation` Axion kernel | Required fields cannot be silently dropped or renamed in minor versions. |

## Local Contract Layer

`t81-lang` now ships a minimal language-owned contract layer under `include/t81/tisc/` and related headers (`include/t81/tensor.hpp`, `include/t81/enum_meta.hpp`) to keep frontend compilation independent from full runtime internals. This layer is intentionally limited to compiler-facing IR/types and does not claim runtime parity.

## Current State

- Migrated language frontend/tests are now hosted in `t81-lang`.
- Several end-to-end tests still reference runtime and CLI headers owned by `t81-foundation`.
- During transition, these tests should run in one of two modes:
  - standalone language mode (parser/semantic tests),
  - integration mode (with `t81-foundation` headers/libs available).

## Breakage Policy

A change is breaking and requires a major compatibility bump if it:

- changes opcode/operand semantics consumed by HanoiVM,
- changes binary layout consumed by TISC/bytecode loaders,
- removes required metadata fields for Axion/runtime validation,
- invalidates deterministic replay/provenance guarantees.

## Minimum CI Gate (target)

1. `t81-lang`: parser/semantic determinism suite passes.
2. `t81-lang` + `t81-foundation`: integration suite passes for accepted artifact versions.
3. Compatibility matrix is updated for any schema/opcode/manifest change.
