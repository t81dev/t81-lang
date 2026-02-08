# t81-lang

T81Lang is the deterministic, balanced-ternary DSL in the `t81dev` ecosystem.
It compiles source code into TISC IR and HanoiVM bytecode, with Axion safety checks and reproducible execution guarantees.

This repository is the canonical home for language-specific work that was previously embedded in `t81-foundation`.

## Status

Scaffold phase. This repository now includes:

- language and compiler scope documents
- cross-repo alignment map against all visible `t81dev` repositories
- phased implementation roadmap and delivery criteria
- scripts to sync ecosystem inventory from GitHub
- repository split plan from `t81-foundation` with migration checkpoints

## Start Here

- `docs/spec.md`
- `docs/architecture/ecosystem-alignment.md`
- `docs/architecture/compiler-architecture.md`
- `docs/architecture/repository-split-plan.md`
- `docs/architecture/compatibility-matrix.md`
- `docs/spec/language-scope.md`
- `docs/migration/migrated-from-t81-foundation.tsv`
- `CONTRIBUTING.md`
- `ROADMAP.md`
- `STATUS.md`

## Near-Term Deliverables

1. Parser + AST with deterministic canonical formatting.
2. Semantic analysis with explicit ternary types and effect boundaries.
3. TISC IR lowering with reproducible hashes.
4. HanoiVM bytecode emission with conformance tests.
5. Axion policy hooks at compile time and runtime handoff.

## Repo Layout

- `src/`: parser, lexer, AST, semantic analysis, and codegen modules.
- `examples/`: `.t81` programs and demos.
- `tests/`: language test suites.
- `tests/syntax/`: grammar/parser-focused tests.
- `tests/semantics/`: type and semantic rule tests.
- `tests/roundtrip/`: source/IR/bytecode determinism tests.
- `docs/`: language documentation and architecture notes.
- `docs/spec.md`: living language specification entrypoint.
- `std/`: standard library modules (reserved for upcoming implementations).
- `tools/`: REPL/formatter/linter/playground stubs and helpers.
- `CONTRIBUTING.md`: contributor workflow and scope boundaries.

Additional migration/interop paths currently present:

- `include/t81/frontend/`: migrated frontend headers.
- `include/t81/tisc/`: minimal language-owned TISC contract headers.
- `spec/`: migrated language specification and RFC documents.
- `scripts/`: ecosystem sync and migration automation.

## Ownership Boundary

- `t81-lang` owns grammar, parser, AST, type system, codegen-to-TISC, language RFCs, language tests, and language examples.
- `t81-vm` owns TISC execution semantics, HanoiVM runtime behavior, and host/runtime integration.
- `t81-foundation` remains the upstream normative specification source during migration.
- Shared contract changes require synchronized updates in both repositories with explicit compatibility notes.

## Why A Dedicated Repository

- Focused scope for language contributors and PL research workflows.
- Better discoverability for users specifically seeking a ternary programming language.
- Independent language versioning (`v0.x`) without coupling to runtime release cadence.
- Clear ecosystem maturity signal: language is a first-class project, not an embedded subfolder.

## Migration Workflow

- Initial migration source: local `t81-foundation` checkout.
- Canonical migration script: `scripts/migrate-from-foundation.sh`.
- Migration traceability manifest: `docs/migration/migrated-from-t81-foundation.tsv`.

## Ecosystem Source

This scaffold is grounded in the public repositories under:

- `https://github.com/t81dev`

Use `scripts/sync-ecosystem-context.sh` to refresh local inventory snapshots.
