# ROADMAP

Snapshot date: 2026-02-09

## M0: Foundation (current)

- Finalize language scope and non-goals.
- Lock compiler architecture and deterministic guarantees.
- Add CI skeleton and minimal command surface.
- Complete migration of language assets from `t81-foundation` into `t81-lang`.

## M0.5: Repository Split Execution

- Inventory existing language assets currently inside `t81-foundation` (`examples`, parser/frontend code, language docs, tests).
- Move and rehome assets into `t81-lang` preserving commit history where feasible.
- Replace moved files in `t81-foundation` with pointers to `t81-lang` canonical paths.
- Add cross-repo compatibility matrix and transition notes.
- Validate no behavior regression in `t81-foundation` integration tests after extraction.

Current status (2026-02-09):

- Completed: initial asset migration import + migration script + traceability manifest.
- Completed: first runnable `t81-lang parse` command and parser determinism snapshot lane.
- Completed: module/import parser MVP, function annotation (`@effect`, `@tier`) parser MVP, and logical `&&`/`||` precedence wiring.
- Completed: expanded semantic test stabilization in language-core lane.
- In progress: decoupling migrated tests from runtime-owned internals and wiring split CI lanes.

## M1: Frontend Core

- Lexer and parser for baseline syntax.
- AST with stable serialization/canonicalization.
- Deterministic formatter.
- Parser/property tests.

## M2: Semantics

- Symbol resolution and module graph loading.
- Type checking for balanced-ternary primitives.
- Deterministic diagnostics and error codes.

## M3: TISC IR

- Lower AST/typed IR to TISC IR.
- Stable IR textual and binary representation.
- Conformance tests against `t81-foundation` contracts.

## M4: Bytecode + Runtime Handoff

- Emit HanoiVM bytecode.
- Attach provenance hashes and manifest metadata.
- Axion safety policy integration points.

## M5: Tooling + Ecosystem

- Python bindings/CLI integration path (`t81-python`).
- Example programs (`t81-examples`).
- Benchmark baselines (`t81-benchmarks`).
- Documentation sync (`t81-docs`).

## M6: Hardening

- Differential determinism tests across platforms.
- Fuzzing for parser/typechecker/codegen.
- Versioning and compatibility policy.
- Establish independent language release train and changelog discipline.
