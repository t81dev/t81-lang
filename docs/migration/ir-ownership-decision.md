# IR Ownership Decision

Snapshot date: 2026-02-08

Tracking issue: `t81-lang#3`

## Decision

`t81/tisc/ir.hpp` and directly related compiler-facing interfaces remain language-owned in `t81-lang` for the current migration phase.

Current language-owned contract surface:

- `include/t81/tisc/ir.hpp`
- `include/t81/tisc/program.hpp`
- `include/t81/tisc/opcodes.hpp`
- `include/t81/tisc/type_alias.hpp`
- `include/t81/tisc/pretty_printer.hpp`

## Rationale

- These headers are actively consumed by frontend, semantic analysis, and IR-lowering flows in this repository.
- Keeping them local avoids adding a premature shared-package dependency while runtime parity is still in progress.
- Runtime compatibility remains protected by cross-repo gates (`scripts/check-vm-compat.py`) against `t81-vm` contract artifacts.

## Extraction Trigger To Shared Contract Package

Revisit this decision only when all conditions are true:

1. VM parity P0 and P1 slices are complete in `t81-vm`.
2. `t81-lang` runtime-coupled manifest shrinks to a small integration subset.
3. At least two repos require the same IR headers without language-frontend internals.
4. Versioning/governance for a shared package is defined and automated.

Until then, IR ownership stays in `t81-lang` with explicit compatibility checks to `t81-vm`.
