# Compatibility Matrix

Snapshot date: 2026-02-08

This matrix defines the expected contract between `t81-lang` compiler artifacts and runtime components in `t81-vm`.

## Versioning Model

- `t81-lang` owns language syntax/semantics versioning.
- `t81-vm` owns runtime execution contract versioning.
- `t81-foundation` remains upstream normative source for spec language during migration.
- Cross-repo compatibility is explicit and tested.

Current runtime contract baseline:

- `t81-vm` active tagged baseline: `runtime-contract-v0.4`
- `t81-vm` current migration pin (`main`): `fc9d55eba258aa2d17d7778e29e186f0bf62e2d8`
- `t81-vm` contract version at migration pin: `2026-02-08-v4`
- `runtime-contract-v0.4` status: tagged and active
- Contract file: `t81-vm/docs/contracts/vm-compatibility.json`
- Local marker: `contracts/runtime-contract.json`

## Contract Surface

| Surface | Producer | Consumer | Compatibility Rule |
| :--- | :--- | :--- | :--- |
| T81Lang grammar + AST shape | `t81-lang` | `t81-lang` tooling | No runtime dependency; versioned in language releases. |
| Typed semantic model | `t81-lang` | `t81-lang` codegen | Deterministic diagnostics and canonicalization required. |
| TISC IR text/binary encoding | `t81-lang` | `t81-vm` (TISC loader/VM) | Must match accepted IR schema versions published by `t81-vm` contract artifacts. |
| Bytecode/program metadata manifest | `t81-lang` | `t81-vm` (HanoiVM/Axion runtime handoff) | Must include declared format version and deterministic provenance hash fields. |
| Axion policy handoff metadata | `t81-lang` | `t81-vm` | Required fields cannot be silently dropped or renamed in minor versions. |

## Local Contract Layer

`t81-lang` now ships a minimal language-owned contract layer under `include/t81/tisc/` and related headers (`include/t81/tensor.hpp`, `include/t81/enum_meta.hpp`) to keep frontend compilation independent from full runtime internals. This layer is intentionally limited to compiler-facing IR/types and does not claim runtime parity.

## Current State

- Migrated language frontend/tests are now hosted in `t81-lang`.
- Several end-to-end tests still reference runtime and CLI headers owned by `t81-foundation` and are being moved toward `t81-vm`.
- During transition, these tests should run in one of two modes:
  - standalone language mode (parser/semantic tests),
  - integration mode (with `t81-foundation` headers/libs available).

## Breakage Policy

A change is breaking and requires a major compatibility bump if it:

- changes opcode/operand semantics consumed by HanoiVM,
- changes binary layout consumed by TISC/bytecode loaders,
- removes required metadata fields for Axion/runtime validation,
- invalidates deterministic replay/provenance guarantees.

## Release Cadence Rule

- Any change that alters VM-facing opcode/format/manifest expectations must:
  1. bump `t81-vm` contract version,
  2. update this matrix in the same cycle,
  3. pass `scripts/check-vm-compat.py` against the new contract.

## Minimum CI Gate (target)

1. `t81-lang`: language-core suite passes (`scripts/check-lang-core.sh`).
2. `t81-lang` + `t81-vm`: artifact roundtrip passes (`scripts/check-compiler-roundtrip.sh`).
3. `t81-lang` + `t81-vm`: runtime compatibility gate passes (`scripts/check-vm-compat.py`).
4. Compatibility matrix is updated for any schema/opcode/manifest change.
