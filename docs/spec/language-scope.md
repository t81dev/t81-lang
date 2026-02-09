# T81Lang Scope

## Goals

- Deterministic source-to-bytecode compilation.
- First-class balanced ternary values (`-1`, `0`, `+1`) and tryte-oriented types.
- Explicit control over side effects and nondeterminism boundaries.
- Clear compatibility contract with TISC IR and HanoiVM.

## Non-Goals

- Replacing general-purpose host languages.
- JIT-first or speculative runtime behavior.
- Implicit floating-point fallback semantics.

## Core Semantic Pillars

- Canonical compilation: identical input must produce identical IR/bytecode/provenance.
- Axion-safe behavior: undefined/overflow-sensitive operations are explicit and trapped.
- Auditability: all compiled artifacts carry reproducible metadata and hashing.

## Current Type Surface (implemented baseline)

- Primitive scalars: `i2`, `i8`, `i16`, `i32`, `T81BigInt`, `T81Float`, `T81Fraction`, `bool`
- Structural core types: `Option[T]`, `Result[T, E]`
- Collection/math types: `Vector[T]`, `Matrix[T]`, `Tensor[T, ...]`, `Graph[T]`
- User-defined structural types: `record`, `enum`, generic aliases via `type`

## Determinism Rules (implemented baseline)

- Stable lexical ordering for module traversal.
- Stable symbol IDs and deterministic name mangling.
- No host-time entropy without explicit capability import.

## Frontend Surface (implemented baseline)

- Declarations: `module`, `import`, `fn`, `let`, `var`, `type`, `record`, `enum`
- Function annotations: `@effect`, `@tier(n)` (parsed and semantically validated)
- Structural annotations: `@schema(n)`, `@module(path)` on `record` and `enum`
- Expressions: arithmetic, comparison, logical `&&`/`||` (deterministic precedence), `match`, vector literals
