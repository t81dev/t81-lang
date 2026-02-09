# Core Datatype Stabilization Plan

Snapshot date: 2026-02-09

Objective: promote every core datatype listed in `t81-foundation/include/t81/core/README.md` into stable `t81-lang` syntax + compile coverage, preserving deterministic/trit-first behavior.

## Stability Definition (must pass all)

A datatype is considered stable in `t81-lang` only when all conditions are met:

1. Surface syntax is documented in `docs/spec/language-scope.md` (or linked spec page).
2. Parser coverage exists in syntax tests.
3. Semantic coverage exists in semantics tests.
4. IR/codegen coverage exists where applicable.
5. At least one example that uses the datatype is compile-verified in `scripts/check-examples-build.sh`.
6. Output determinism is preserved (`make all` including parser determinism lane).

## Execution Order

Order is by dependency and existing signal strength, not by category name.

### Phase A: Core language reliability

- Fix identifier corruption/duplication paths observed in semantic diagnostics.
- Fix call expression lowering stability (`CallExpr` missing result failures).
- Stabilize string literal and typed-`let` behavior in the build path.

Exit criteria:

- `examples/00_hello_world.t81` compiles with stdout/string usage.
- `examples/string_demo.t81` compiles in examples lane.

### Phase B: Structural + flow core

- Promote `Option` and `Result` from frontend-tested to compile-verified.
- Stabilize tutorial-grade `record`/`enum` examples for compile lane.
- Add compile-verified examples for match-heavy control flow.

Exit criteria:

- Option/Result lesson added to `scripts/check-examples-build.sh`.
- Record/enum lesson added to `scripts/check-examples-build.sh`.

### Phase C: Numeric trit-first families

- Promote `T81BigInt`, `T81Float`, `T81Fraction`, and selected fixed/complex primitives.
- Add compile-verified examples per primitive family.
- Add deterministic IR assertions for arithmetic opcode families.

Exit criteria:

- Primitive family examples compile in examples lane.
- Matching semantics/IR tests exist for each promoted family.

### Phase D: Containers, tensors, and advanced core

- Promote `Vector`, `Matrix`, `Tensor`, `Graph`.
- Then promote remaining core/system datatypes (`T81Bytes`, `T81Symbol`, `T81Time`, etc.) once language surface syntax is specified.

Exit criteria:

- Each promoted type has at least one compile-verified example and parser/semantic coverage.

## Program Gate

- `docs/migration/core-datatype-support-matrix.md` is the status source of truth.
- Status moves allowed:
  - `Runtime/core only` -> `Frontend-tested`
  - `Frontend-tested` -> `Compile-verified`
- A status move to `Compile-verified` requires a corresponding `scripts/check-examples-build.sh` entry.

## Immediate Next Slice

1. Land Phase A reliability fixes (names, calls, strings/stdout).
2. Re-enable compile verification for `examples/00_hello_world.t81`.
3. Promote Option/Result lesson into compile lane.
