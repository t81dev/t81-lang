# Repository Split Plan: `t81-foundation` -> `t81-lang`

Snapshot date: 2026-02-08

## Objective

Move all language-specific concerns from `t81-foundation` into `t81-lang` while keeping runtime semantics stable and integration contracts explicit.

## Scope Moving To `t81-lang`

- Grammar and syntax definitions.
- Lexer/parser and AST implementation.
- Semantic analysis and type/effect checking.
- TISC IR lowering owned by language frontend.
- Language RFCs and syntax evolution process.
- Language tests and language-focused examples.
- Language user documentation and onboarding pages.

## Scope Staying In `t81-foundation`

- HanoiVM runtime and bytecode execution behavior.
- TISC op semantics and runtime validation.
- Axion runtime safety kernel implementation.
- Core deterministic numerics and system integration.

## Migration Inventory Template

Use this table during extraction from `t81-foundation`:

| Asset Category | Current Path (`t81-foundation`) | Target Path (`t81-lang`) | Move Type | Owner |
| :--- | :--- | :--- | :--- | :--- |
| Grammar docs | `...` | `docs/spec/...` | move | language |
| Parser code | `...` | `src/t81_lang/parser/...` | move | language |
| Examples | `examples/*.t81` | `examples/*.t81` | move or mirror | language |
| Runtime adapter | `...` | `src/t81_lang/codegen/...` | split | shared |
| Conformance tests | `tests/...` | `tests/conformance/...` | split | shared |

## Execution Sequence

1. Freeze language-facing interfaces in `t81-foundation` for one migration window.
2. Land `t81-lang` repository skeleton and governance docs.
3. Move language assets in small batches (docs, examples, parser, tests).
4. Add forwarding links in `t81-foundation` to canonical `t81-lang` locations.
5. Add compatibility matrix validating compiler output against HanoiVM expectations.
6. Cut first independent `t81-lang` pre-release tag (example: `v0.1.0-alpha`).

## Automation

- Repeatable migration sync script: `scripts/migrate-from-foundation.sh`.
- File-level migration traceability: `docs/migration/migrated-from-t81-foundation.tsv`.
- Dependency gap tracking: `docs/migration/dependency-gaps.md`.

## Compatibility Contract

- `t81-lang` emits versioned TISC IR and bytecode manifest metadata.
- `t81-foundation` publishes accepted compiler artifact versions.
- Any breaking change in compiler output format requires:
  - new format version,
  - migration note,
  - conformance tests passing in both repositories.

## Contributor Experience Changes

- Language contributors can work entirely in `t81-lang` for syntax, semantics, diagnostics, and examples.
- Runtime contributors stay in `t81-foundation` unless changing compiler-runtime contracts.
- Cross-repo PR links are required for contract-impacting changes.

## Acceptance Criteria

- No canonical language source remains only in `t81-foundation`.
- `t81-foundation` docs link to `t81-lang` as language source of truth.
- Reproducibility and Axion safety claims remain unchanged after extraction.
- Existing hello-world and baseline sample programs compile and execute via new repo boundaries.
