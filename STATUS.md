# STATUS

Snapshot date: 2026-02-08

## Current Phase

Repository foundation complete; migration execution in progress.

## Completed

- Baseline project documentation scaffold.
- Cross-repo alignment draft covering all public `t81dev` repositories.
- Initial compiler architecture and language scope draft.
- GitHub API sync script for ecosystem metadata.
- Initial split rationale captured: language ownership separated from runtime ownership.
- Initial migration import completed from local `t81-foundation`:
  - frontend headers/sources
  - language `.t81` examples
  - language docs/spec/RFCs
  - language-focused tests and vectors
- Migration automation script added: `scripts/migrate-from-foundation.sh`.
- Migration traceability manifest added: `docs/migration/migrated-from-t81-foundation.tsv`.

## In Progress

- Select reference implementation language/toolchain for compiler frontend.
- Define first executable milestone: parser + AST + deterministic formatting.
- Dependency isolation plan so migrated language tests can run without bundling full runtime internals.
- Compatibility matrix enforcement between emitted artifacts and `t81-foundation` runtime expectations.

## Blockers

- No blocker for docs/planning track.
- Implementation kickoff requires a final choice of build stack (e.g. Rust/C++/Python hybrid).
- Migration execution depends on confirming which source-of-truth files remain mirrored in `t81-foundation` versus moved fully.
- Some migrated tests currently depend on runtime/CLI headers still owned by `t81-foundation`; decoupling is pending.

## Exit Criteria For Foundation Phase

- Milestone M0 accepted.
- CI skeleton added for lint/test/docs validation.
- First runnable `t81-lang parse` command merged.
- Repository split checklist accepted and migration backlog prioritized.
- Compatibility matrix adopted and referenced by both repos.
