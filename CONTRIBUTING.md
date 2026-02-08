# CONTRIBUTING

## Scope

Contribute language-specific changes here: grammar, parser, AST, semantics, diagnostics, codegen-to-TISC, language docs, and language examples.

Runtime VM behavior and Axion execution internals remain in `t81-foundation` unless a cross-repo contract update is required.

## Development Workflow

1. Open/confirm an issue or RFC note for behavior changes.
2. Add or update tests before/with implementation.
3. Update spec docs (`docs/spec.md` and detailed references) for semantic changes.
4. Keep outputs deterministic (ordering, diagnostics, metadata).

## Test Lanes

- Language-only tests: parser/frontend/semantic suites in this repository.
- Integration tests: suites requiring runtime/CLI contracts from `t81-foundation`.

## Migration Context

Some files are migrated from `t81-foundation` and tracked in:

- `docs/migration/migrated-from-t81-foundation.tsv`

Use `scripts/migrate-from-foundation.sh` when refreshing migrated assets.
