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

## Initial Type Surface (draft)

- `trit`: one balanced ternary digit.
- `tryte<N>`: fixed-width balanced ternary scalar.
- `bool3`: ternary predicate domain.
- `tensor3<shape, qspec>`: quantized tensor view.

## Determinism Rules (draft)

- Stable lexical ordering for module traversal.
- Stable symbol IDs and deterministic name mangling.
- No host-time entropy without explicit capability import.
